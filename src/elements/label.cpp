#include <M5GFX.h>

#include "utils.h"
#include "elements/label.h"
#include "page.h"

namespace uilib {
	Label::Label(const String& text, const point& position, color_t color, color_t bg_color, float scale, Page* parent_page, font_t font, int32_t margin, int32_t radius)
	: TextElement(font, color, bg_color, scale)
	, text(text)
	, margin(margin)
	, radius(radius)
	{
		this->position = position;
		this->parent_page = parent_page;
	}

	void Label::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
		if (!parent_page || text == "") {
			bx = position.x;
			by = position.y;
			bw = 0;
			bh = 0;
			return;
		}
		auto canvas = parent_page->getCanvas();
		canvas->setFont(font);
		canvas->setTextSize(scale);

		// 改行で分割
		std::vector<String> lines;
		int32_t start = 0;
		while (true) {
			int32_t idx = text.indexOf('\n', start);
			if (idx == -1) {
				lines.push_back(text.substring(start));
				break;
			}
			lines.push_back(text.substring(start, idx));
			start = idx + 1;
		}

		int32_t max_w = 0;
		int32_t line_h = canvas->fontHeight() * scale;
		for (const auto& line : lines) {
			int32_t lw = canvas->textWidth(line);
			if (lw > max_w) max_w = lw;
		}
		int32_t total_h = lines.size() * line_h;
		
		bw = max_w + 2 * margin;
		bh = total_h + 2 * margin;
		bx = position.x + getAlignXOffset(bw);
		by = position.y - margin;
	}

	void Label::draw(){
		if(!parent_page || text == "") return;
		auto canvas = parent_page->getCanvas();
		canvas->setFont(font);
		canvas->setTextSize(scale);
		
		// 改行で分割
		std::vector<String> lines;
		int32_t start = 0;
		while (true) {
			int32_t idx = text.indexOf('\n', start);
			if (idx == -1) {
				lines.push_back(text.substring(start));
				break;
			}
			lines.push_back(text.substring(start, idx));
			start = idx + 1;
		}

		int32_t max_w = 0;
		int32_t line_h = canvas->fontHeight() * scale;
		for (const auto& line : lines) {
			int32_t lw = canvas->textWidth(line);
			if (lw > max_w) max_w = lw;
		}
		int32_t total_h = lines.size() * line_h;
		
		int32_t align_offset = getAlignXOffset(max_w + 2 * margin);
		int32_t bx = position.x + align_offset;

		if (radius > 0) {
			canvas->fillRoundRect(bx, position.y - margin, max_w + 2 * margin, total_h + 2 * margin, radius, bg_color);
		} else {
			canvas->fillRect(bx, position.y - margin, max_w + 2 * margin, total_h + 2 * margin, bg_color);
		}
		canvas->setTextColor(color);

		for (size_t i = 0; i < lines.size(); ++i) {
			int32_t lw = canvas->textWidth(lines[i]);
			int32_t line_x = bx + margin;
			if (align == Align::CENTER) {
				line_x = bx + margin + (max_w - lw) / 2;
			} else if (align == Align::RIGHT) {
				line_x = bx + margin + (max_w - lw);
			}
			canvas->drawString(lines[i], line_x, position.y + i * line_h);
		}
	}

	void Label::set(const String& new_text){
		if (text != new_text) {
			text = new_text;
			flush_required = true;
		}
	}

	String Label::get() const{
		return text;
	}

	void Label::setMargin(int32_t new_margin){
		if (margin != new_margin) {
			margin = new_margin;
			flush_required = true;
		}
	}

	int32_t Label::getMargin() const{
		return margin;
	}

	void Label::setRadius(int32_t new_radius){
		if (radius != new_radius) {
			radius = new_radius;
			flush_required = true;
		}
	}

	int32_t Label::getRadius() const{
		return radius;
	}
}