#include <M5GFX.h>
#include <M5Unified.h>
#include "elements/interactive_element.h"
#include "page.h"

namespace uilib {
    void InteractiveElement::update_touch_state(bool force_flush) {
        released_flag = false;

        // 親ページのフォーカス管理と連携（他の要素がフォーカスを独占している場合は何もしない）
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
                pressed_state = false;
                temp_raw_state = false;
                touch_started_here = false;
                return;
            }
        }

        if (!enabled) {
            pressed_state = false;
            temp_raw_state = false;
            touch_started_here = false;
            return;
        }

        if (force_flush) {
            pressed_state = false;
            temp_raw_state = false;
            touch_started_here = false;
            last_state_change_time = millis();
            press_start_time = 0;
            press_count = 0;
            last_press_duration_ms = 0;
            last_press_count = 0;
            last_release_time = millis();

            auto detail = M5.Touch.getDetail(0);
            ignore_touch = detail.isPressed();
            return;
        }

        if (!parent_page) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        auto detail = M5.Touch.getDetail(0);

        if (ignore_touch) {
            if (!detail.isPressed()) {
                ignore_touch = false;
            }
            pressed_state = false;
            temp_raw_state = false;
            touch_started_here = false;
            last_state_change_time = millis();
            return;
        }

        bool inside = (detail.x >= bx && detail.x < bx + bw && detail.y >= by && detail.y < by + bh);
        
        if (detail.wasPressed()) {
            touch_started_here = inside;
        }
        if (!detail.isPressed()) {
            touch_started_here = false;
        }

        bool raw_state = detail.isPressed() && inside && touch_started_here;
        bool prev_pressed = pressed_state;

        if (raw_state != temp_raw_state) {
            temp_raw_state = raw_state;
            last_state_change_time = millis();
        }

        if (millis() - last_state_change_time >= DEBOUNCE_TIME_MS) {
            if (raw_state != pressed_state) {
                pressed_state = raw_state;
                released_flag = prev_pressed && !pressed_state;
                if (released_flag) {
                    last_release_time = millis();
                }
                flush_required = true;
            }
        }

        if (pressed_state) {
            if (!prev_pressed) {
                press_start_time = millis();
                press_count = 0;
            } else {
                press_count++;
            }
            last_press_duration_ms = millis() - press_start_time;
            last_press_count = press_count;
        }
    }

    bool InteractiveElement::isPressed() const {
        return pressed_state;
    }

    bool InteractiveElement::wasReleased() const {
        return released_flag;
    }

    uint32_t InteractiveElement::getPressDurationMs() const {
        if (pressed_state) {
            return millis() - press_start_time;
        }
        return last_press_duration_ms;
    }

    float InteractiveElement::getPressDurationSec() const {
        return getPressDurationMs() / 1000.0f;
    }

    uint32_t InteractiveElement::getPressCount() const {
        if (pressed_state) {
            return press_count;
        }
        return last_press_count;
    }

    void InteractiveElement::cancelPress() {
        if (pressed_state || temp_raw_state) {
            pressed_state = false;
            temp_raw_state = false;
            flush_required = true;
        }
    }

    void InteractiveElement::setEnabled(bool is_enabled) {
        if (enabled != is_enabled) {
            Element::setEnabled(is_enabled);
            if (!enabled) {
                cancelPress();
                released_flag = false;
            }
        }
    }
}
