#include "elements/interactive_text_element.h"

namespace uilib {
    InteractiveTextElement::InteractiveTextElement(const String& text, const point& position, font_t font,
                                                   color_t color, color_t bg_color, float scale,
                                                   Page* parent_page)
        : InteractiveElement(), text(text), font(font), color(color), bg_color(bg_color), scale(scale) {
        this->position = position;
        this->parent_page = parent_page;
    }

    void InteractiveTextElement::setText(const String& new_text) {
        if (text != new_text) {
            text = new_text;
            flush_required = true;
        }
    }

    String InteractiveTextElement::getText() const {
        return text;
    }

    void InteractiveTextElement::setFont(font_t new_font) {
        if (font != new_font) {
            font = new_font;
            flush_required = true;
        }
    }

    font_t InteractiveTextElement::getFont() const {
        return font;
    }

    void InteractiveTextElement::setColor(color_t new_color) {
        if (color != new_color) {
            color = new_color;
            flush_required = true;
        }
    }

    color_t InteractiveTextElement::getColor() const {
        return color;
    }

    void InteractiveTextElement::setBackground(color_t new_bg_color) {
        if (bg_color != new_bg_color) {
            bg_color = new_bg_color;
            flush_required = true;
        }
    }

    color_t InteractiveTextElement::getBackground() const {
        return bg_color;
    }

    void InteractiveTextElement::setScale(float new_scale) {
        if (scale != new_scale) {
            scale = new_scale;
            flush_required = true;
        }
    }

    float InteractiveTextElement::getScale() const {
        return scale;
    }
}
