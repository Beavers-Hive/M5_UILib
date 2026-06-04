#include <M5GFX.h>

#include "utils.h"
#include "elements/dbutton.h"
#include "page.h"

namespace uilib {
    DButton::DButton(const String& text, const point& position, color_t color, color_t bg_color, float scale, Page* parent_page, font_t font, int32_t margin_x, int32_t radius, color_t pressed_color, color_t pressed_bg_color, int32_t margin_y)
    : InteractiveTextElement(text, position, font, color, bg_color, scale, parent_page), 
    margin_x(margin_x),
    radius(radius),
    pressed_color(pressed_color == -1 ? color : pressed_color) {
        this->margin_y = (margin_y == -1) ? margin_x : margin_y;
        if (pressed_bg_color == -1) {
            // Auto-calculate pressed background color (make it slightly darker/different)
            if (bg_color == 0) {
                this->pressed_bg_color = 0x39E7; // Dark gray if original is black
            } else {
                // Assuming RGB565 format
                uint16_t r = (bg_color >> 11) & 0x1F;
                uint16_t g = (bg_color >> 5) & 0x3F;
                uint16_t b = bg_color & 0x1F;
                r = (r * 3) / 4;
                g = (g * 3) / 4;
                b = (b * 3) / 4;
                this->pressed_bg_color = (r << 11) | (g << 5) | b;
            }
        } else {
            this->pressed_bg_color = pressed_bg_color;
        }
    }

    void DButton::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (!parent_page) {
            bx = by = bw = bh = 0;
            return;
        }
        auto canvas = parent_page->getCanvas();
        canvas->setFont(font);
        canvas->setTextSize(scale);
        bw = canvas->textWidth(text) + 2 * margin_x;
        bh = canvas->fontHeight() * scale + 2 * margin_y;
        bx = position.x + getAlignXOffset(bw);
        by = position.y - margin_y;
    }

    void DButton::draw(){
        if(!parent_page) return;
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        auto canvas = parent_page->getCanvas();
        color_t current_bg = enabled ? (pressed_state ? pressed_bg_color : bg_color) : 0x39C7;
        color_t current_fg = enabled ? (pressed_state ? pressed_color : color) : 0x7BEF;

        if (radius > 0) {
            canvas->fillRoundRect(bx, by, bw, bh, radius, current_bg);
        } else {
            canvas->fillRect(bx, by, bw, bh, current_bg);
        }
        canvas->setTextColor(current_fg);
        canvas->setFont(font);
        canvas->setTextSize(scale);
        canvas->drawString(text, bx + margin_x, position.y);
    }

    void DButton::update_impl(bool force_flush) {
        update_touch_state(force_flush);
    }
}