#include <M5Unified.h>
#include <M5GFX.h>
#include <cmath>

#include "utils.h"
#include "elements/slider.h"
#include "page.h"

namespace uilib {
    Slider::Slider(const point& position, const point& size,
                   float min_val, float max_val, float step_val, float init_val,
                   color_t active_col, Page* parent_page)
    : InteractiveElement(), size(size), min_value(min_val), max_value(max_val), step_size(step_val),
      active_color(active_col), inactive_color(0x7BEF), thumb_color(0xFFFF) {
        this->position = position;
        this->parent_page = parent_page;
        this->value = roundValue(init_val);
    }

    float Slider::roundValue(float val) const {
        if (val < min_value) return min_value;
        if (val > max_value) return max_value;

        // 最も近いステップ値に丸める
        float steps = roundf((val - min_value) / step_size);
        float rounded = min_value + steps * step_size;

        if (rounded > max_value) return max_value;
        if (rounded < min_value) return min_value;
        return rounded;
    }

    void Slider::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        bw = size.x;
        bh = size.y;
        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void Slider::draw() {
        if (!parent_page) return;
        auto canvas = parent_page->getCanvas();

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // つまみの半径を決定 (スライダー高さの半分、最小でも5pxとする)
        int32_t thumb_radius = bh / 2;
        if (thumb_radius < 5) thumb_radius = 5;

        int32_t track_height = 6;
        int32_t track_y = by + (bh - track_height) / 2;

        // 現在の値に応じた比率を算出 (0.0f - 1.0f)
        float ratio = 0.0f;
        if (max_value > min_value) {
            ratio = (value - min_value) / (max_value - min_value);
        }

        // つまみがスライド可能な領域の幅 (左右端につまみの半径分のマージンを設ける)
        int32_t usable_width = bw - 2 * thumb_radius;
        int32_t cx = bx + thumb_radius + ratio * usable_width;

        // 有効・無効状態によるカラーの決定
        color_t current_active_color = enabled ? active_color : 0x39C7;
        color_t current_inactive_color = enabled ? inactive_color : 0x2104;
        color_t current_thumb_color = enabled ? thumb_color : 0x7BEF;

        // トラックの描画
        // 左側 (選択中トラック)
        if (cx > bx + thumb_radius) {
            canvas->fillRoundRect(bx + thumb_radius - 2, track_y, cx - (bx + thumb_radius) + 2, track_height, track_height / 2, current_active_color);
        }
        // 右側 (未選択トラック)
        if (cx < bx + bw - thumb_radius) {
            canvas->fillRoundRect(cx, track_y, (bx + bw - thumb_radius) - cx + 2, track_height, track_height / 2, current_inactive_color);
        }

        // つまみ（丸）の描画（ドラッグ中は少しサイズを大きくする）
        int32_t draw_radius = (enabled && pressed_state) ? (thumb_radius + 2) : thumb_radius;
        canvas->fillCircle(cx, by + bh / 2, draw_radius, current_thumb_color);
    }

    void Slider::update_impl(bool force_flush) {
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

        if (force_flush) {
            pressed_state = false;
            temp_raw_state = false;
            touch_started_here = false;
            last_state_change_time = millis();
            ignore_touch = false;
            
            auto detail = M5.Touch.getDetail(0);
            if (detail.isPressed()) {
                ignore_touch = true;
            }
            return;
        }

        if (!enabled) {
            pressed_state = false;
            temp_raw_state = false;
            touch_started_here = false;
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

        // タッチ開始の判定範囲
        bool inside = (detail.x >= bx && detail.x < bx + bw && detail.y >= by && detail.y < by + bh);
        
        if (detail.wasPressed()) {
            touch_started_here = inside;
        }
        if (!detail.isPressed()) {
            touch_started_here = false;
        }

        if (!touch_started_here) {
            if (pressed_state) {
                pressed_state = false;
                flush_required = true;
            }
            temp_raw_state = false;
            last_state_change_time = millis();
            return;
        }

        // 指が離された場合はドラッグ状態を解除
        if (!detail.isPressed()) {
            if (pressed_state) {
                pressed_state = false;
                flush_required = true;
            }
            temp_raw_state = false;
            last_state_change_time = millis();
            return;
        }

        bool raw_state = detail.isPressed() && inside && touch_started_here;

        // デバウンスによるドラッグ開始判定
        if (!pressed_state) {
            if (raw_state != temp_raw_state) {
                temp_raw_state = raw_state;
                last_state_change_time = millis();
            }

            if (millis() - last_state_change_time >= DEBOUNCE_TIME_MS) {
                if (raw_state) {
                    pressed_state = true;
                    flush_required = true;
                }
            }
        }

        // ドラッグ中の値更新処理
        if (pressed_state) {
            int32_t thumb_radius = bh / 2;
            if (thumb_radius < 5) thumb_radius = 5;

            int32_t usable_width = bw - 2 * thumb_radius;
            if (usable_width <= 0) usable_width = 1;

            // タッチ座標からスライダー値比率を求める (0.0f - 1.0fにクランプ)
            float ratio = (float)(detail.x - bx - thumb_radius) / usable_width;
            if (ratio < 0.0f) ratio = 0.0f;
            if (ratio > 1.0f) ratio = 1.0f;

            // 比率をスライダー値に変換して丸め
            float raw_val = min_value + ratio * (max_value - min_value);
            float stepped_val = roundValue(raw_val);

            if (value != stepped_val) {
                value = stepped_val;
                flush_required = true;
                if (on_value_change_callback) {
                    on_value_change_callback(value);
                }
            }
        }
    }

    void Slider::setValue(float new_value) {
        float rounded = roundValue(new_value);
        if (value != rounded) {
            value = rounded;
            flush_required = true;
            if (on_value_change_callback) {
                on_value_change_callback(value);
            }
        }
    }

    void Slider::setRange(float min_val, float max_val) {
        if (min_value != min_val || max_value != max_val) {
            min_value = min_val;
            max_value = max_val;
            setValue(value); // 新しい範囲内でクランプして再設定
        }
    }

    void Slider::setStep(float step_val) {
        if (step_size != step_val && step_val > 0.0f) {
            step_size = step_val;
            setValue(value); // 新しいステップ間隔に丸めて再設定
        }
    }
}
