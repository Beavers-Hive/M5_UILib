#include <M5Unified.h>
#include <M5GFX.h>
#include <vector>

#include "utils.h"
#include "elements/dropdown.h"
#include "page.h"

namespace uilib {
    Dropdown::Dropdown(const std::vector<String>& items, const point& position, int32_t width, int32_t height,
                       int initial_index, color_t color, color_t bg_color, float scale,
                       Page* parent_page, font_t font)
    : InteractiveElement(), items(items), selected_index(initial_index), width(width), height(height),
      font(font), color(color), bg_color(bg_color), scale(scale) {
        this->position = position;
        this->parent_page = parent_page;
    }

    void Dropdown::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        bw = width;
        bh = height;
        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void Dropdown::calculateListBounds() {
        if (!parent_page) return;
        auto canvas = parent_page->getCanvas();
        int32_t screen_h = canvas->height();

        canvas->setFont(font);
        canvas->setTextSize(scale);
        item_height = (canvas->fontHeight() + 8) * scale;
        if (item_height < 20) item_height = 20;

        int num_items = items.size();
        int visible_count = (num_items < max_visible_items) ? num_items : max_visible_items;
        list_h = item_height * visible_count;
        list_w = width;
        
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);
        list_x = bx;

        // 画面下端をはみ出すかチェック
        if (position.y + height + list_h > screen_h) {
            // 上向きに展開
            list_y = position.y - list_h;
        } else {
            // 下向きに展開
            list_y = position.y + height;
        }
    }

    float Dropdown::getMaxScrollY() const {
        int num_items = items.size();
        if (num_items <= max_visible_items) return 0.0f;
        return (num_items - max_visible_items) * item_height;
    }

    void Dropdown::setSelectedIndex(int index) {
        if (index >= -1 && index < (int)items.size()) {
            if (selected_index != index) {
                selected_index = index;
                flush_required = true;
                if (on_change_callback) {
                    on_change_callback(selected_index);
                }
            }
        }
    }

    String Dropdown::getSelectedText() const {
        if (selected_index >= 0 && selected_index < (int)items.size()) {
            return items[selected_index];
        }
        return "";
    }

    void Dropdown::draw() {
        if (!parent_page) return;
        auto canvas = parent_page->getCanvas();

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // カラーの決定
        color_t current_border = enabled ? color : 0x7BEF;
        color_t current_bg = enabled ? bg_color : 0x39C7;
        color_t current_text = enabled ? color : 0x7BEF;

        // 1. 通常時のドロップダウンボタン描画
        canvas->fillRect(bx, by, bw, bh, current_bg);
        canvas->drawRect(bx, by, bw, bh, current_border);

        // テキスト表示
        canvas->setFont(font);
        canvas->setTextSize(scale);
        canvas->setTextColor(current_text);
        
        int32_t fh = canvas->fontHeight() * scale;
        int32_t text_y = by + (bh - fh) / 2;

        String displayText = getSelectedText();
        if (displayText.length() == 0) {
            displayText = "Select...";
            canvas->setTextColor(0x7BEF); // プレースホルダー色はグレー
        }
        
        // テキストがドロップダウンボタンの幅を超えないように切り捨て描画するためのクリップ
        canvas->setClipRect(bx + 4, by, bw - 24, bh);
        canvas->drawString(displayText, bx + 4, text_y);
        canvas->clearClipRect();

        // 矢印（▼）の描画
        canvas->setTextColor(current_text);
        int32_t arrow_cx = bx + bw - 12;
        int32_t arrow_cy = by + bh / 2;
        // 小さな下向き三角形
        canvas->fillTriangle(arrow_cx - 4, arrow_cy - 2, arrow_cx + 4, arrow_cy - 2, arrow_cx, arrow_cy + 3, current_text);

        // 2. リスト（展開時）の描画
        if (is_open) {
            calculateListBounds();
            
            // リストの背景と枠線を描画
            canvas->fillRect(list_x, list_y, list_w, list_h, bg_color);
            canvas->drawRect(list_x, list_y, list_w, list_h, color);

            // クリップ矩形を設定してスクロール表示
            canvas->setClipRect(list_x + 1, list_y + 1, list_w - 2, list_h - 2);

            int start_idx = (int)(scroll_y / item_height);
            int num_items = items.size();
            
            for (int i = start_idx; i < num_items; ++i) {
                int32_t item_y = list_y + (i * item_height) - (int)scroll_y;
                if (item_y >= list_y + list_h) break;

                // アイテムの背景（選択中またはドラッグ中のハイライト）
                bool is_item_selected = (i == selected_index);
                if (is_item_selected) {
                    canvas->fillRect(list_x + 1, item_y, list_w - 2, item_height, 0x03E0); // 緑系のハイライト
                }

                canvas->setFont(font);
                canvas->setTextSize(scale);
                canvas->setTextColor(is_item_selected ? WHITE : color);
                canvas->drawString(items[i], list_x + 6, item_y + (item_height - fh) / 2);

                // 区切り線
                if (i < num_items - 1) {
                    canvas->drawFastHLine(list_x + 1, item_y + item_height - 1, list_w - 2, 0x39E7); // 細いグレー線
                }
            }

            canvas->clearClipRect();

            // スクロールバーの描画（スクロールが必要な場合）
            float max_scroll = getMaxScrollY();
            if (max_scroll > 0) {
                int32_t bar_w = 6;
                int32_t bar_x = list_x + list_w - bar_w - 1;
                
                // スクロールバーの背景線
                canvas->fillRect(bar_x, list_y + 1, bar_w, list_h - 2, 0x18C3); // 濃いグレー
                
                // つまみ部分の計算
                float visible_ratio = (float)list_h / (item_height * items.size());
                int32_t thumb_h = (int32_t)(list_h * visible_ratio);
                if (thumb_h < 10) thumb_h = 10; // 最低サイズ
                
                float scroll_ratio = scroll_y / max_scroll;
                int32_t thumb_y = list_y + 1 + (int32_t)((list_h - 2 - thumb_h) * scroll_ratio);
                
                canvas->fillRect(bar_x, thumb_y, bar_w, thumb_h, color);
            }
        }
    }

    void Dropdown::update_impl(bool force_flush) {
        if (!enabled) {
            is_open = false;
            drag_active = false;
            touch_pressed = false;
            prev_pressed = false;
            touch_started_here = false;
            return;
        }

        if (force_flush) {
            is_open = false;
            drag_active = false;
            touch_pressed = false;
            prev_pressed = false;
            touch_started_here = false;
            scroll_y = 0.0f;
            ignore_touch = false;
            
            auto detail = M5.Touch.getDetail(0);
            if (detail.isPressed()) {
                ignore_touch = true;
            }
            return;
        }

        if (!parent_page) return;

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
                touch_pressed = false;
                drag_active = false;
                prev_pressed = false;
                touch_started_here = false;
                return;
            }
        }

        auto detail = M5.Touch.getDetail(0);

        if (ignore_touch) {
            if (!detail.isPressed()) {
                ignore_touch = false;
            }
            touch_pressed = false;
            drag_active = false;
            return;
        }

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        bool inside_button = (detail.x >= bx && detail.x < bx + bw && detail.y >= by && detail.y < by + bh);

        if (!is_open) {
            // 閉じた状態でのタップ検出
            if (detail.wasPressed()) {
                touch_started_here = inside_button;
            }
            if (!detail.isPressed()) {
                touch_started_here = false;
            }

            if (!touch_started_here) {
                touch_pressed = false;
                prev_pressed = detail.isPressed();
                return;
            }

            bool raw_state = detail.isPressed() && inside_button;

            if (raw_state && !prev_pressed) {
                // タッチされた瞬間
                touch_pressed = true;
                touch_start_time = millis();
            } else if (!detail.isPressed() && prev_pressed && touch_pressed) {
                // 離された瞬間（クリック）
                is_open = true;
                scroll_y = 0.0f;
                // 最初は選択項目が見える位置に少しスクロール
                if (selected_index >= 0) {
                    float target_scroll = selected_index * item_height - (list_h - item_height) / 2.0f;
                    float max_scroll = getMaxScrollY();
                    if (target_scroll < 0) target_scroll = 0;
                    if (target_scroll > max_scroll) target_scroll = max_scroll;
                    scroll_y = target_scroll;
                }
                parent_page->setFocusedElement(this);
                flush_required = true;
                touch_pressed = false;
            }

            if (!detail.isPressed()) {
                touch_pressed = false;
            }
            prev_pressed = detail.isPressed();
        } else {
            // 開いた状態でのタッチ処理
            calculateListBounds();
            bool inside_list = (detail.x >= list_x && detail.x < list_x + list_w && detail.y >= list_y && detail.y < list_y + list_h);

            if (detail.wasPressed()) {
                touch_started_here = inside_button || inside_list;
            }
            if (!detail.isPressed()) {
                touch_started_here = false;
            }

            if (!touch_started_here) {
                touch_pressed = false;
                drag_active = false;
                return;
            }

            if (detail.isPressed()) {
                if (!touch_pressed) {
                    // タッチされた瞬間
                    touch_pressed = true;
                    touch_start_x = detail.x;
                    touch_start_y = detail.y;
                    touch_start_time = millis();

                    if (inside_list) {
                        drag_active = true;
                        drag_start_y = detail.y;
                        drag_start_scroll_y = scroll_y;
                    }
                } else {
                    // ドラッグ中
                    if (drag_active) {
                        int32_t diff_y = detail.y - drag_start_y;
                        float target_scroll = drag_start_scroll_y - diff_y;
                        float max_scroll = getMaxScrollY();
                        
                        if (target_scroll < 0) target_scroll = 0;
                        if (target_scroll > max_scroll) target_scroll = max_scroll;

                        if (scroll_y != target_scroll) {
                            scroll_y = target_scroll;
                            flush_required = true;
                        }
                    }
                }
            } else {
                // 離された瞬間
                if (touch_pressed) {
                    touch_pressed = false;
                    drag_active = false;

                    // 移動距離の判定
                    int32_t dist_x = abs(detail.x - touch_start_x);
                    int32_t dist_y = abs(detail.y - touch_start_y);
                    uint32_t duration = millis() - touch_start_time;

                    // ドラッグ中であっても移動量が小さければタップとみなして閉じる（スワイプ時は閉じない）
                    if (dist_x < 8 && dist_y < 8 && duration < 500) {
                        if (inside_list) {
                            // リスト項目がタップされた
                            int32_t click_local_y = detail.y - list_y + (int)scroll_y;
                            int clicked_idx = click_local_y / item_height;
                            if (clicked_idx >= 0 && clicked_idx < (int)items.size()) {
                                setSelectedIndex(clicked_idx);
                            }
                        }
                        // タップが行われたためリストを閉じる（リスト内タップ、またはリスト外タップ）
                        is_open = false;
                        parent_page->setFocusedElement(nullptr);
                        flush_required = true;
                    }
                }
            }
        }
    }
}
