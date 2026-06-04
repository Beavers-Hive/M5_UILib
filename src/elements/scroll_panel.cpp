#include <M5Unified.h>
#include <M5GFX.h>
#include "elements/scroll_panel.h"
#include "page.h"

namespace uilib {
    ScrollPanel::ScrollPanel(const point& pos, int32_t w, int32_t h, color_t bg_col, Page* parent_page)
        : Panel(pos, w, h, bg_col, parent_page) {
        last_update_time = millis();
    }

    void ScrollPanel::addChild(Element* child) {
        if (!child) return;
        
        // Panel側のchildrenに追加
        Panel::addChild(child);

        // ローカルでの管理リストに追加
        ScrollItem item;
        item.element = child;
        item.local_position = child->getPosition();
        scroll_items.push_back(item);

        // 初期状態でスクロールオフセットを適用
        child->setPosition(item.local_position - scroll_offset);
        
        updateMaxScroll();
    }

    void ScrollPanel::draw() {
        if (!parent_page || !visible) return;
        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // 背景塗りつぶし
        canvas->fillRect(bx, by, bw, bh, bg_color);

        // 枠線描画
        if (stroke_width > 0) {
            for (int32_t i = 0; i < stroke_width; ++i) {
                canvas->drawRect(bx + i, by + i, bw - 2 * i, bh - 2 * i, border_color);
            }
        }

        // クリップ矩形を設定（枠線の内側）
        canvas->setClipRect(bx + stroke_width, by + stroke_width, bw - 2 * stroke_width, bh - 2 * stroke_width);

        // 子要素を描画
        for (auto& item : scroll_items) {
            if (item.element->isVisible()) {
                item.element->draw();
            }
        }

        // --- スクロールバーの描画 ---
        // クリップ矩形が有効なため、描画座標は枠線の内側に正しく収まります
        int32_t win_w = bw - 2 * stroke_width;
        int32_t win_h = bh - 2 * stroke_width;

        // 1. 縦スクロールバーの描画
        if ((scroll_dir == ScrollDirection::VERTICAL || scroll_dir == ScrollDirection::BOTH) && max_scroll.y > 0) {
            int32_t content_h = max_scroll.y + win_h;
            int32_t bar_h = win_h * win_h / content_h;
            if (bar_h < 8) bar_h = 8; // 最低長さ 8px
            
            int32_t scrollable_y_space = win_h - bar_h - 2 * scrollbar_margin;
            float ratio_y = static_cast<float>(scroll_offset.y) / max_scroll.y;
            
            int32_t bar_x = bx + bw - stroke_width - scrollbar_margin - scrollbar_thickness;
            int32_t bar_y = by + stroke_width + scrollbar_margin + static_cast<int32_t>(ratio_y * scrollable_y_space);

            canvas->fillRoundRect(bar_x, bar_y, scrollbar_thickness, bar_h, scrollbar_thickness / 2, scrollbar_color);
        }

        // 2. 横スクロールバーの描画
        if ((scroll_dir == ScrollDirection::HORIZONTAL || scroll_dir == ScrollDirection::BOTH) && max_scroll.x > 0) {
            int32_t content_w = max_scroll.x + win_w;
            int32_t bar_w = win_w * win_w / content_w;
            if (bar_w < 8) bar_w = 8; // 最低長さ 8px
            
            int32_t scrollable_x_space = win_w - bar_w - 2 * scrollbar_margin;
            float ratio_x = static_cast<float>(scroll_offset.x) / max_scroll.x;
            
            int32_t bar_x = bx + stroke_width + scrollbar_margin + static_cast<int32_t>(ratio_x * scrollable_x_space);
            int32_t bar_y = by + bh - stroke_width - scrollbar_margin - scrollbar_thickness;

            canvas->fillRoundRect(bar_x, bar_y, bar_w, scrollbar_thickness, scrollbar_thickness / 2, scrollbar_color);
        }

        // クリップ解除
        canvas->clearClipRect();
    }

    void ScrollPanel::update_impl(bool force_flush) {
        if (force_flush) {
            touch_active = false;
            drag_active = false;
            velocity = {0, 0};
            scroll_offset = {0, 0};
            updateMaxScroll();
            for (auto& item : scroll_items) {
                item.element->setPosition(item.local_position);
                item.element->update(true);
            }
            return;
        }

        if (!enabled) {
            touch_active = false;
            drag_active = false;
            velocity = {0, 0};
            return;
        }

        // フォーカスチェック: 他の要素（メッセージボックス等）がフォーカスを得ている場合はスクロール処理を行わない
        if (parent_page && parent_page->getFocusedElement() != nullptr) {
            bool belongs_to_focus = false;
            Element* curr = this;
            while (curr != nullptr) {
                if (curr == parent_page->getFocusedElement()) {
                    belongs_to_focus = true;
                    break;
                }
                curr = curr->getParentElement();
            }
            if (!belongs_to_focus) {
                touch_active = false;
                drag_active = false;
                velocity = {0, 0};
                for (auto& item : scroll_items) {
                    item.element->cancelPress();
                }
                return;
            }
        }

        updateMaxScroll();

        auto detail = M5.Touch.getDetail(0);
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);
        bool inside = (detail.x >= bx && detail.x < bx + bw && detail.y >= by && detail.y < by + bh);

        uint32_t now = millis();
        float dt = (now - last_update_time) / 1000.0f;
        if (dt <= 0.0f || dt > 0.1f) dt = 0.016f; // 安全値
        last_update_time = now;

        // 1. タッチ入力によるスクロール処理
        if (detail.isPressed()) {
            if (!touch_active) {
                if (inside) {
                    touch_active = true;
                    drag_active = false;
                    touch_start_pos = {detail.x, detail.y};
                    touch_start_scroll = scroll_offset;
                    touch_start_time = now;
                    velocity = {0, 0};
                }
            } else {
                int32_t dx = detail.x - touch_start_pos.x;
                int32_t dy = detail.y - touch_start_pos.y;

                if (scroll_dir == ScrollDirection::VERTICAL) {
                    dx = 0;
                } else if (scroll_dir == ScrollDirection::HORIZONTAL) {
                    dy = 0;
                }

                // 一定ドット以上の移動でドラッグスクロールモードへ移行
                bool should_drag = false;
                if (scroll_dir == ScrollDirection::VERTICAL) {
                    should_drag = (abs(dy) > 8);
                } else if (scroll_dir == ScrollDirection::HORIZONTAL) {
                    should_drag = (abs(dx) > 8);
                } else {
                    should_drag = (abs(dx) > 8 || abs(dy) > 8);
                }

                if (!drag_active && should_drag) {
                    drag_active = true;
                    // 子要素のタッチ状態をキャンセルする
                    for (auto& item : scroll_items) {
                        item.element->cancelPress();
                    }
                }

                if (drag_active) {
                    point target_scroll = touch_start_scroll - point(dx, dy);
                    
                    // スクリュー量のクランプ
                    if (target_scroll.x < 0) target_scroll.x = 0;
                    if (target_scroll.x > max_scroll.x) target_scroll.x = max_scroll.x;
                    if (target_scroll.y < 0) target_scroll.y = 0;
                    if (target_scroll.y > max_scroll.y) target_scroll.y = max_scroll.y;

                    if (scroll_offset != target_scroll) {
                        // 慣性移動のための速度算出
                        velocity.x = (target_scroll.x - scroll_offset.x) / dt;
                        velocity.y = (target_scroll.y - scroll_offset.y) / dt;
                        
                        scroll_offset = target_scroll;
                        flush_required = true;
                    }
                }
            }
        } else {
            // タッチが離された
            if (touch_active) {
                touch_active = false;
                drag_active = false;
            }

            // 2. 慣性スクロール（フリック）の減衰処理
            float vel_len = velocity.length();
            if (vel_len > 10.0f) {
                // 最大速度制限
                if (vel_len > 800.0f) {
                    velocity = velocity.normalized() * 800.0f;
                }

                if (scroll_dir == ScrollDirection::VERTICAL) {
                    velocity.x = 0;
                } else if (scroll_dir == ScrollDirection::HORIZONTAL) {
                    velocity.y = 0;
                }

                scroll_offset += point(velocity.x * dt, velocity.y * dt);
                
                // 範囲外クランプと停止
                bool clamped = false;
                if (scroll_offset.x < 0) { scroll_offset.x = 0; clamped = true; }
                if (scroll_offset.x > max_scroll.x) { scroll_offset.x = max_scroll.x; clamped = true; }
                if (scroll_offset.y < 0) { scroll_offset.y = 0; clamped = true; }
                if (scroll_offset.y > max_scroll.y) { scroll_offset.y = max_scroll.y; clamped = true; }

                if (clamped) {
                    velocity = {0, 0};
                } else {
                    velocity *= 0.92f; // 摩擦減衰
                }
                flush_required = true;
            } else {
                velocity = {0, 0};
            }
        }

        // 子要素の位置をオフセットに応じて再設定
        for (auto& item : scroll_items) {
            item.element->setPosition(item.local_position - scroll_offset);
        }

        // 子要素の update を個別に呼び出し
        for (auto& item : scroll_items) {
            // スクロールドラッグ中の場合は、子要素に強制的にタッチキャンセルをかけ続ける
            if (drag_active) {
                item.element->cancelPress();
            }
            
            if (item.element->update()) {
                flush_required = true;
            }
        }
    }

    void ScrollPanel::updateMaxScroll() {
        int32_t content_max_x = 0;
        int32_t content_max_y = 0;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        for (auto& item : scroll_items) {
            // 一時的にローカル座標に戻して境界領域を算出する
            point current_pos = item.element->getPosition();
            item.element->setPosition(item.local_position);
            
            int32_t cbx, cby, cbw, cbh;
            item.element->getBounds(cbx, cby, cbw, cbh);
            
            // 位置をスクロールオフセット適用座標へ戻す
            item.element->setPosition(current_pos);

            int32_t right = cbx + cbw - bx;
            int32_t bottom = cby + cbh - by;

            if (right > content_max_x) content_max_x = right;
            if (bottom > content_max_y) content_max_y = bottom;
        }

        max_scroll.x = content_max_x - (bw - 2 * stroke_width);
        max_scroll.y = content_max_y - (bh - 2 * stroke_width);

        if (max_scroll.x < 0) max_scroll.x = 0;
        if (max_scroll.y < 0) max_scroll.y = 0;
    }

    void ScrollPanel::movePosition(const point& diff) {
        Panel::movePosition(diff);
        for (auto& item : scroll_items) {
            item.local_position += diff;
            item.element->setPosition(item.local_position - scroll_offset);
        }
    }

    void ScrollPanel::setPosition(const point& new_pos) {
        point diff = new_pos - position;
        movePosition(diff);
    }

    void ScrollPanel::setScrollOffset(const point& offset) {
        point target = offset;
        updateMaxScroll();
        if (target.x < 0) target.x = 0;
        if (target.x > max_scroll.x) target.x = max_scroll.x;
        if (target.y < 0) target.y = 0;
        if (target.y > max_scroll.y) target.y = max_scroll.y;

        if (scroll_offset != target) {
            scroll_offset = target;
            flush_required = true;
            for (auto& item : scroll_items) {
                item.element->setPosition(item.local_position - scroll_offset);
            }
        }
    }
}
