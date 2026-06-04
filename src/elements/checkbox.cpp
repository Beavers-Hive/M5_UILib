#include <M5Unified.h>
#include <M5GFX.h>

#include "utils.h"
#include "elements/checkbox.h"
#include "page.h"

namespace uilib {
    Checkbox::Checkbox(const String& text, const point& position, bool initial_state,
                       color_t color, color_t bg_color, float scale,
                       Page* parent_page, font_t font)
    : InteractiveTextElement(text, position, font, color, bg_color, scale, parent_page), checked(initial_state) {}

    void Checkbox::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (!parent_page) {
            bx = by = bw = bh = 0;
            return;
        }
        auto canvas = parent_page->getCanvas();
        canvas->setFont(font);
        canvas->setTextSize(scale);

        int32_t fh = canvas->fontHeight() * scale;
        int32_t box_size = fh;
        if (box_size < 16) box_size = 16; // タッチターゲットとしての最低サイズ

        int32_t text_w = canvas->textWidth(text);

        bw = box_size + 8 + text_w;
        bh = (box_size > fh) ? box_size : fh;
        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void Checkbox::draw() {
        if (!parent_page) return;
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        auto canvas = parent_page->getCanvas();
        canvas->setFont(font);
        canvas->setTextSize(scale);
        int32_t fh = canvas->fontHeight() * scale;
        int32_t box_size = fh;
        if (box_size < 16) box_size = 16;

        // カラーの決定
        color_t border_color = enabled ? color : 0x7BEF;
        color_t current_bg = enabled ? bg_color : 0x39C7;
        color_t text_color = enabled ? color : 0x7BEF;

        // ボックス外枠と背景の描画
        int32_t box_y = by + (bh - box_size) / 2;
        canvas->drawRect(bx, box_y, box_size, box_size, border_color);
        canvas->fillRect(bx + 1, box_y + 1, box_size - 2, box_size - 2, current_bg);

        // チェック状態のときV字チェックマークを描画
        if (checked) {
            color_t check_color = enabled ? GREEN : 0x7BEF;
            int32_t x1 = bx + 3;
            int32_t y1 = box_y + box_size / 2;
            int32_t x2 = bx + box_size / 2 - 1;
            int32_t y2 = box_y + box_size - 4;
            int32_t x3 = bx + box_size - 3;
            int32_t y3 = box_y + 4;

            canvas->drawLine(x1, y1, x2, y2, check_color);
            canvas->drawLine(x2, y2, x3, y3, check_color);
            
            // 線の太さを出すための補正描画
            canvas->drawLine(x1, y1 + 1, x2, y2 + 1, check_color);
            canvas->drawLine(x2, y2 + 1, x3, y3 + 1, check_color);
            canvas->drawLine(x1, y1 - 1, x2, y2 - 1, check_color);
            canvas->drawLine(x2, y2 - 1, x3, y3 - 1, check_color);
        }

        // テキストラベルの描画
        canvas->setTextColor(text_color);
        canvas->drawString(text, bx + box_size + 8, by + (bh - fh) / 2);
    }

    void Checkbox::update_impl(bool force_flush) {
        update_touch_state(force_flush);

        if (wasReleased()) {
            checked = !checked;
            flush_required = true;
            if (on_change_callback) {
                on_change_callback(checked);
            }
        }
    }

    void Checkbox::setChecked(bool is_checked) {
        if (checked != is_checked) {
            checked = is_checked;
            flush_required = true;
        }
    }
}
