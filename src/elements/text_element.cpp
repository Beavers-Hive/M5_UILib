#include "elements/text_element.h"

namespace uilib {
    TextElement::TextElement(font_t font, color_t color, color_t bg_color, float scale)
        : font(font), color(color), bg_color(bg_color), scale(scale) {}

    void TextElement::setFont(font_t new_font) {
        if (font != new_font) {
            font = new_font;
            flush_required = true;
        }
    }

    font_t TextElement::getFont() const {
        return font;
    }

    void TextElement::setColor(color_t new_color) {
        if (color != new_color) {
            color = new_color;
            flush_required = true;
        }
    }

    color_t TextElement::getColor() const {
        return color;
    }

    void TextElement::setBackground(color_t new_bg_color) {
        if (bg_color != new_bg_color) {
            bg_color = new_bg_color;
            flush_required = true;
        }
    }

    color_t TextElement::getBackground() const {
        return bg_color;
    }

    void TextElement::setScale(float new_scale) {
        if (scale != new_scale) {
            scale = new_scale;
            flush_required = true;
        }
    }

    float TextElement::getScale() const {
        return scale;
    }
}
