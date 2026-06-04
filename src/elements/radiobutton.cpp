#include <M5Unified.h>
#include <M5GFX.h>

#include "utils.h"
#include "elements/radiobutton.h"
#include "page.h"

namespace uilib {
    // ==========================================
    // RadioButtonGroup の実装
    // ==========================================

    void RadioButtonGroup::addButton(RadioButton* button) {
        if (!button) return;
        buttons.push_back(button);
        button->group = this;
        // 新しく追加されたボタンの選択状態を現在のグループ選択値と同期
        button->setSelected(button->getValue() == selected_value);
    }

    void RadioButtonGroup::selectValue(int value) {
        if (selected_value != value) {
            selected_value = value;
            for (auto* btn : buttons) {
                btn->setSelected(btn->getValue() == value);
            }
            if (on_change_callback) {
                on_change_callback(value);
            }
        }
    }

    // ==========================================
    // RadioButton の実装
    // ==========================================

    RadioButton::RadioButton(const String& text, const point& position, int value,
                             color_t color, color_t bg_color, float scale,
                             Page* parent_page, font_t font)
    : InteractiveTextElement(text, position, font, color, bg_color, scale, parent_page), value(value) {}

    void RadioButton::setSelected(bool is_selected) {
        if (selected != is_selected) {
            selected = is_selected;
            flush_required = true;
        }
    }

    void RadioButton::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (!parent_page) {
            bx = by = bw = bh = 0;
            return;
        }
        auto canvas = parent_page->getCanvas();
        canvas->setFont(font);
        canvas->setTextSize(scale);

        int32_t fh = canvas->fontHeight() * scale;
        int32_t circle_size = fh;
        if (circle_size < 16) circle_size = 16; // 最低の当たり判定領域サイズ

        int32_t text_w = canvas->textWidth(text);

        bw = circle_size + 8 + text_w;
        bh = (circle_size > fh) ? circle_size : fh;
        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void RadioButton::draw() {
        if (!parent_page) return;
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        auto canvas = parent_page->getCanvas();
        canvas->setFont(font);
        canvas->setTextSize(scale);
        int32_t fh = canvas->fontHeight() * scale;
        int32_t circle_size = fh;
        if (circle_size < 16) circle_size = 16;

        int32_t r = circle_size / 2;
        int32_t cy = by + bh / 2;
        int32_t cx = bx + r;

        // カラーの決定
        color_t outline_color = enabled ? color : 0x7BEF;
        color_t current_bg = enabled ? bg_color : 0x39C7;
        color_t text_color = enabled ? color : 0x7BEF;

        // 背景円の塗りつぶし
        canvas->fillCircle(cx, cy, r, current_bg);
        // 外枠円の描画
        canvas->drawCircle(cx, cy, r, outline_color);

        // 選択中なら内部にドットを描画
        if (selected) {
            color_t inner_color = enabled ? GREEN : 0x7BEF;
            canvas->fillCircle(cx, cy, r - 4, inner_color);
        }

        // テキストラベルの描画
        canvas->setTextColor(text_color);
        canvas->drawString(text, bx + circle_size + 8, by + (bh - fh) / 2);
    }

    void RadioButton::update_impl(bool force_flush) {
        update_touch_state(force_flush);

        if (wasReleased()) {
            if (group) {
                group->selectValue(value);
            } else {
                setSelected(true);
            }
        }
    }
}
