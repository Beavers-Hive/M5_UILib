#include "elements/keyboard.h"
#include "page.h"
#include <Arduino.h>

namespace uilib {

    static const char* const LOWER_KEYS[] = {
        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
        "a", "s", "d", "f", "g", "h", "j", "k", "l",
        "Shift", "z", "x", "c", "v", "b", "n", "m", "BS",
        "123", "Space", "Enter"
    };

    static const char* const UPPER_KEYS[] = {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
        "A", "S", "D", "F", "G", "H", "J", "K", "L",
        "shift", "Z", "X", "C", "V", "B", "N", "M", "BS",
        "123", "Space", "Enter"
    };

    static const char* const SYMBOL_KEYS[] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "-", "/", ":", ";", "(", ")", "$", "&", "@",
        "#+=", ".", ",", "?", "!", "'", "\"", "_", "BS",
        "abc", "Space", "Enter"
    };

    static const char* const SYMBOL2_KEYS[] = {
        "[", "]", "{", "}", "#", "%", "^", "*", "+", "=",
        "_", "\\", "|", "~", "<", ">", "\u00a5", "\u20ac", "\u00a3", // Yen, Euro, Pound
        "123", ".", ",", "?", "!", "'", "\"", "`", "BS",
        "abc", "Space", "Enter"
    };

    KeyButton::KeyButton() : DButton() {}

    void KeyButton::setFixedSize(int32_t w, int32_t h) {
        fixed_width = w;
        fixed_height = h;
        flush_required = true;
    }

    void KeyButton::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (fixed_width > 0 && fixed_height > 0) {
            bx = position.x;
            by = position.y;
            bw = fixed_width;
            bh = fixed_height;
        } else {
            DButton::getBounds(bx, by, bw, bh);
        }
    }

    void KeyButton::draw() {
        if (!parent_page || !visible) return;
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        color_t current_bg = enabled ? (pressed_state ? getPressedBackgroundColor() : getBackground()) : 0x39C7;
        color_t current_fg = enabled ? (pressed_state ? getPressedColor() : getColor()) : 0x7BEF;

        int32_t r = getRadius();
        if (r > 0) {
            canvas->fillRoundRect(bx, by, bw, bh, r, current_bg);
        } else {
            canvas->fillRect(bx, by, bw, bh, current_bg);
        }

        canvas->setTextColor(current_fg);
        canvas->setFont(getFont());
        canvas->setTextSize(getScale());

        int32_t tw = canvas->textWidth(text);
        int32_t th = canvas->fontHeight() * getScale();
        int32_t tx = bx + (bw - tw) / 2;
        int32_t ty = by + (bh - th) / 2;
        canvas->drawString(text, tx, ty);
    }

    Keyboard::Keyboard(const point& pos, Page* parent_page)
        : Panel(pos, 320, 120, 0x1082, parent_page) { // 0x1082 is dark gray

        setStrokeWidth(1);
        setBorderColor(Color(80, 80, 80));

        int32_t spacing_x = 2;
        int32_t spacing_y = 4;
        int32_t padding_top = 4;
        int32_t padding_left = 6;
        int32_t row_h = 25;
        int32_t key_w = 29;

        for (int i = 0; i < 31; ++i) {
            int32_t kx = 0, ky = 0, kw = key_w, kh = row_h;

            if (i < 10) {
                // Row 1: 10 keys
                kx = pos.x + padding_left + i * (key_w + spacing_x);
                ky = pos.y + padding_top;
            } else if (i < 19) {
                // Row 2: 9 keys
                int32_t row2_padding = (320 - (9 * key_w + 8 * spacing_x)) / 2;
                kx = pos.x + row2_padding + (i - 10) * (key_w + spacing_x);
                ky = pos.y + padding_top + row_h + spacing_y;
            } else if (i < 28) {
                // Row 3: Shift, 7 keys, BS
                ky = pos.y + padding_top + 2 * (row_h + spacing_y);
                if (i == 19) {
                    kx = pos.x + 6;
                    kw = 44;
                } else if (i == 27) {
                    kx = pos.x + 269;
                    kw = 45;
                } else {
                    kx = pos.x + 52 + (i - 20) * (key_w + spacing_x);
                }
            } else {
                // Row 4: Mode, Space, Enter
                ky = pos.y + padding_top + 3 * (row_h + spacing_y);
                if (i == 28) {
                    kx = pos.x + 8;
                    kw = 60;
                } else if (i == 29) {
                    kx = pos.x + 70;
                    kw = 150;
                } else {
                    kx = pos.x + 222;
                    kw = 90;
                }
            }

            // Normal styling colors
            color_t bg_col = 0x2945; // Medium gray
            color_t fg_col = WHITE;
            color_t pr_bg = 0x4A69; // Lighter gray for press
            color_t pr_fg = WHITE;

            bool is_special = (i == 19 || i == 27 || i == 28 || i == 30);
            if (is_special) {
                bg_col = 0x18C3; // Darker gray
                pr_bg = 0x2945;
                if (i == 30) {
                    // Enter is blue
                    bg_col = 0x2497;
                    pr_bg = 0x3D7E;
                }
            }

            // Initialize this specific KeyButton:
            keys[i].setParentPage(parent_page);
            keys[i].setParentElement(this);
            keys[i].setPosition({kx, ky});
            keys[i].setFixedSize(kw, kh);
            keys[i].setRadius(4);
            keys[i].setBackground(bg_col);
            keys[i].setColor(fg_col);
            keys[i].setPressedBackgroundColor(pr_bg);
            keys[i].setPressedColor(pr_fg);
            keys[i].setFont(&fonts::lgfxJapanGothicP_12);
            keys[i].setScale(1.0f);

            // Add child to the Panel container
            Panel::addChild(&keys[i]);
        }

        // Initialize button labels
        updateKeyLabels();
    }

    void Keyboard::update_impl(bool force_flush) {
        // First update all key buttons
        Panel::update_impl(force_flush);

        // Check if any key was released and handle it
        for (int i = 0; i < 31; ++i) {
            if (keys[i].wasReleased()) {
                handleKeyPress(i);
                break; // Handle one release per loop to be safe
            }
        }
    }

    void Keyboard::updateKeyLabels() {
        const char* const* layout = nullptr;
        switch (mode) {
            case KeyboardMode::LOWER:   layout = LOWER_KEYS; break;
            case KeyboardMode::UPPER:   layout = UPPER_KEYS; break;
            case KeyboardMode::SYMBOL:  layout = SYMBOL_KEYS; break;
            case KeyboardMode::SYMBOL2: layout = SYMBOL2_KEYS; break;
        }

        for (int i = 0; i < 31; ++i) {
            keys[i].setText(layout[i]);
        }
        flush_required = true;
    }

    void Keyboard::handleKeyPress(int index) {
        String key_val = keys[index].getText();

        if (key_val == "Shift" || key_val == "shift") {
            if (mode == KeyboardMode::LOWER) {
                mode = KeyboardMode::UPPER;
            } else {
                mode = KeyboardMode::LOWER;
            }
            updateKeyLabels();
        } else if (key_val == "123") {
            mode = KeyboardMode::SYMBOL;
            updateKeyLabels();
        } else if (key_val == "abc") {
            mode = KeyboardMode::LOWER;
            updateKeyLabels();
        } else if (key_val == "#+=") {
            mode = KeyboardMode::SYMBOL2;
            updateKeyLabels();
        } else if (key_val == "BS") {
            removeLastChar(text_buffer);
            setText(text_buffer);
        } else if (key_val == "Space") {
            text_buffer += " ";
            setText(text_buffer);
        } else if (key_val == "Enter") {
            if (on_submit_callback) {
                on_submit_callback(text_buffer);
            }
        } else {
            text_buffer += key_val;
            setText(text_buffer);
        }
    }

    void Keyboard::removeLastChar(String& s) {
        if (s.isEmpty()) return;
        int len = s.length();
        int i = len - 1;
        // Skip UTF-8 continuation bytes
        while (i > 0 && (s[i] & 0xC0) == 0x80) {
            i--;
        }
        s = s.substring(0, i);
    }

    void Keyboard::setText(const String& text) {
        if (text_buffer != text) {
            text_buffer = text;
            flush_required = true;
        }

        // Keep targets in sync
        if (target_label) {
            target_label->set(text_buffer);
        }
        if (target_interactive_text) {
            target_interactive_text->setText(text_buffer);
        }

        if (on_change_callback) {
            on_change_callback(text_buffer);
        }
    }

    void Keyboard::clear() {
        setText("");
    }

    void Keyboard::setMode(KeyboardMode new_mode) {
        if (mode != new_mode) {
            mode = new_mode;
            updateKeyLabels();
        }
    }
}
