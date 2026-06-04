#include <M5Unified.h>
#include <M5GFX.h>

#include "utils.h"
#include "elements/toggleswitch.h"
#include "page.h"

namespace uilib {
    ToggleSwitch::ToggleSwitch(const point& position, const point& size, bool initial_state,
                               color_t on_color, color_t off_color, color_t thumb_color,
                               Page* parent_page)
    : InteractiveElement(), size(size), state(initial_state), on_color(on_color), off_color(off_color), thumb_color(thumb_color) {
        this->position = position;
        this->parent_page = parent_page;
    }

    void ToggleSwitch::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        bw = size.x;
        bh = size.y;
        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void ToggleSwitch::draw() {
        if (!parent_page) return;
        auto canvas = parent_page->getCanvas();

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // トラックの色を決定（押されている間は25%暗くする、無効時は0x39C7）
        color_t current_track_color = enabled ? (state ? on_color : off_color) : 0x39C7;
        if (enabled && pressed_state) {
            uint16_t r = (current_track_color >> 11) & 0x1F;
            uint16_t g = (current_track_color >> 5) & 0x3F;
            uint16_t b = current_track_color & 0x1F;
            r = (r * 3) / 4;
            g = (g * 3) / 4;
            b = (b * 3) / 4;
            current_track_color = (r << 11) | (g << 5) | b;
        }

        // 角丸トラック（カプセル型）の描画
        int32_t r = bh / 2;
        canvas->fillRoundRect(bx, by, bw, bh, r, current_track_color);

        // つまみ（丸）の描画
        int32_t padding = 2;
        int32_t d = bh - 2 * padding;
        int32_t tr = d / 2;
        int32_t cy = by + bh / 2;
        int32_t cx = 0;
        if (state) {
            cx = bx + bw - padding - tr;
        } else {
            cx = bx + padding + tr;
        }

        color_t current_thumb_color = enabled ? thumb_color : 0x7BEF;
        canvas->fillCircle(cx, cy, tr, current_thumb_color);
    }

    void ToggleSwitch::update_impl(bool force_flush) {
        update_touch_state(force_flush);

        if (wasReleased()) {
            state = !state;
            flush_required = true;
            if (on_toggle_callback) {
                on_toggle_callback(state);
            }
        }
    }

    void ToggleSwitch::setState(bool new_state) {
        if (state != new_state) {
            state = new_state;
            flush_required = true;
        }
    }
}