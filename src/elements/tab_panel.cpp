#include <M5Unified.h>
#include <M5GFX.h>
#include "elements/tab_panel.h"
#include "page.h"

namespace uilib {
    TabPanel::TabPanel(const point& pos, int32_t w, int32_t h, color_t bg_col, Page* parent_page)
        : Panel(pos, w, h, bg_col, parent_page) {
    }

    void TabPanel::addTab(const String& name, Panel* panel) {
        if (!panel) return;

        TabItem item;
        item.name = name;
        item.content_panel = panel;
        tabs.push_back(item);

        // Panel側の子要素リストにも登録（Pageへの自動 update などのルーティング用）
        Panel::addChild(panel);
        panel->setParentPage(this->parent_page);

        // 新しい子パネルのサイズと位置を TabPanel のコンテンツ領域に合わせる
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        int32_t content_x = bx + stroke_width;
        int32_t content_y = by + tab_bar_height;
        int32_t content_w = bw - 2 * stroke_width;
        int32_t content_h = bh - tab_bar_height - stroke_width;

        panel->setPosition({content_x, content_y});
        panel->setDimensions(content_w, content_h);

        // 初期選択状態の管理
        if (tabs.size() == 1) {
            selected_index = 0;
            panel->setVisible(true);
            panel->setEnabled(true);
        } else {
            panel->setVisible(false);
            panel->setEnabled(false);
        }

        flush_required = true;
    }

    void TabPanel::selectTab(int32_t index) {
        if (index < 0 || index >= (int32_t)tabs.size()) return;
        if (selected_index == index) return;

        deactivateTab(selected_index);

        selected_index = index;
        if (tabs[selected_index].content_panel) {
            tabs[selected_index].content_panel->setVisible(true);
            tabs[selected_index].content_panel->setEnabled(true);
            // 切り替わった瞬間に子パネル内を一度強制フラッシュする
            tabs[selected_index].content_panel->update(true);
        }

        flush_required = true;
    }

    void TabPanel::deactivateTab(int32_t index) {
        if (index < 0 || index >= (int32_t)tabs.size()) return;

        Panel* panel = tabs[index].content_panel;
        if (panel) {
            panel->setVisible(false);
            panel->setEnabled(false);
            
            // パネル自身および内包する子要素のタッチ・プレス状態を強制キャンセル
            panel->cancelPress();
            for (auto* child : panel->getChildren()) {
                child->cancelPress();
            }
        }
    }

    void TabPanel::draw() {
        if (!parent_page || !visible) return;
        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // 1. パネル全体の背景（コンテンツエリア）
        canvas->fillRect(bx, by, bw, bh, bg_color);

        // 外枠線（タブバーを除いた枠線を描画する）
        if (stroke_width > 0) {
            for (int32_t i = 0; i < stroke_width; ++i) {
                // コンテンツ部分の枠線（下部・左側・右側）
                canvas->drawRect(bx + i, by + tab_bar_height - i, bw - 2 * i, bh - tab_bar_height + i, border_color);
            }
        }

        if (tabs.empty()) return;

        // 2. タブバー全体の背景描画
        canvas->fillRect(bx + stroke_width, by + stroke_width, bw - 2 * stroke_width, tab_bar_height - stroke_width, bar_bg_color);

        // 各タブの幅を均等分割して算出
        int32_t available_w = bw - 2 * stroke_width;
        int32_t tab_w = available_w / tabs.size();

        for (int32_t i = 0; i < (int32_t)tabs.size(); ++i) {
            int32_t tx = bx + stroke_width + i * tab_w;
            int32_t ty = by + stroke_width;
            int32_t th = tab_bar_height - stroke_width;
            int32_t tw = (i == (int32_t)tabs.size() - 1) ? (available_w - i * tab_w) : tab_w; // 端数補正

            bool active = (i == selected_index);
            color_t current_tab_bg = active ? active_tab_color : inactive_tab_color;

            // タブの背景描画
            canvas->fillRect(tx, ty, tw, th, current_tab_bg);

            // タブ間の境界線を描画
            if (i > 0) {
                canvas->drawFastVLine(tx, ty, th, border_color);
            }

            // アクティブなタブには上部または下部に強調用の境界線を描く
            if (active && stroke_width > 0) {
                canvas->fillRect(tx, ty, tw, stroke_width, active_border_color);
            }

            // タブ名のテキスト描画
            canvas->setFont(tab_font);
            canvas->setTextSize(font_scale);
            canvas->setTextColor(text_color);

            int32_t text_w = canvas->textWidth(tabs[i].name);
            int32_t text_h = canvas->fontHeight() * font_scale;

            int32_t text_x = tx + (tw - text_w) / 2;
            int32_t text_y = ty + (th - text_h) / 2;

            // 文字がはみ出ないようにクリッピング設定をしてから描画
            canvas->setClipRect(tx + 2, ty, tw - 4, th);
            canvas->drawString(tabs[i].name, text_x, text_y);
            canvas->clearClipRect();
        }

        // 3. アクティブなタブコンテンツの描画
        if (selected_index >= 0 && selected_index < (int32_t)tabs.size()) {
            Panel* active_panel = tabs[selected_index].content_panel;
            if (active_panel && active_panel->isVisible()) {
                active_panel->draw();
            }
        }
    }

    void TabPanel::update_impl(bool force_flush) {
        if (force_flush) {
            touch_active = false;
            if (selected_index >= 0 && selected_index < (int32_t)tabs.size()) {
                Panel* active_panel = tabs[selected_index].content_panel;
                if (active_panel) {
                    active_panel->update(true);
                }
            }
            return;
        }

        if (!enabled) {
            touch_active = false;
            return;
        }

        // フォーカスチェック: 他の要素（メッセージボックス等）がフォーカスを得ている場合はタッチ処理を行わない
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
                if (selected_index >= 0 && selected_index < (int32_t)tabs.size()) {
                    Panel* active_panel = tabs[selected_index].content_panel;
                    if (active_panel) {
                        active_panel->cancelPress();
                    }
                }
                return;
            }
        }

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        auto detail = M5.Touch.getDetail(0);
        bool pressed = detail.isPressed();

        // 1. タブバー領域のタッチ判定
        bool inside_bar = (detail.x >= bx && detail.x < bx + bw &&
                           detail.y >= by && detail.y < by + tab_bar_height);

        if (pressed) {
            if (!touch_active && detail.wasPressed()) {
                if (inside_bar && !tabs.empty()) {
                    touch_active = true;

                    // タップされた位置から選択されたタブを算出
                    int32_t relative_x = detail.x - (bx + stroke_width);
                    int32_t available_w = bw - 2 * stroke_width;
                    int32_t tab_w = available_w / tabs.size();

                    int32_t clicked_index = relative_x / tab_w;
                    if (clicked_index >= (int32_t)tabs.size()) {
                        clicked_index = tabs.size() - 1;
                    }
                    if (clicked_index < 0) {
                        clicked_index = 0;
                    }

                    selectTab(clicked_index);
                }
            }
        } else {
            touch_active = false;
        }

        // 2. アクティブな子パネルの update を委譲
        if (selected_index >= 0 && selected_index < (int32_t)tabs.size()) {
            Panel* active_panel = tabs[selected_index].content_panel;
            // タブバーのタップ処理中は、子パネルのタッチ処理をキャンセルさせ続ける
            if (touch_active && active_panel) {
                active_panel->cancelPress();
                for (auto* child : active_panel->getChildren()) {
                    child->cancelPress();
                }
            }

            if (active_panel && active_panel->update()) {
                flush_required = true;
            }
        }
    }

    void TabPanel::movePosition(const point& diff) {
        Panel::movePosition(diff);

        // 管理している全タブ用パネルの位置を相対移動させる
        for (auto& item : tabs) {
            if (item.content_panel) {
                item.content_panel->movePosition(diff);
            }
        }
    }

    void TabPanel::setPosition(const point& new_pos) {
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        point diff = new_pos - position;
        Panel::movePosition(diff);

        // 親座標に基づいて全子パネルの絶対座標を再配置
        int32_t new_bx = position.x + getAlignXOffset(bw);
        int32_t new_by = position.y;

        int32_t content_x = new_bx + stroke_width;
        int32_t content_y = new_by + tab_bar_height;
        int32_t content_w = bw - 2 * stroke_width;
        int32_t content_h = bh - tab_bar_height - stroke_width;

        for (auto& item : tabs) {
            if (item.content_panel) {
                item.content_panel->setPosition({content_x, content_y});
                item.content_panel->setDimensions(content_w, content_h);
            }
        }
    }
}
