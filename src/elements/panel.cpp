#include <M5Unified.h>
#include "elements/panel.h"
#include "page.h"

namespace uilib {
	Panel::Panel(const point& pos, int32_t w, int32_t h, color_t bg_col, Page* parent_page)
		: Element(), width(w), height(h), bg_color(bg_col) {
		this->position = pos;
		this->parent_page = parent_page;
	}

	void Panel::addChild(Element* child) {
		if (!child) return;
		children.push_back(child);
		child->setParentElement(this);
		// 初期状態をパネルに同期
		child->setVisible(this->visible);
		child->setEnabled(this->enabled);
	}

	void Panel::draw() {
		if (!parent_page || !visible) return;
		auto canvas = parent_page->getCanvas();
		if (!canvas) return;

		int32_t bx, by, bw, bh;
		getBounds(bx, by, bw, bh);

		// 背景塗りつぶし
		canvas->fillRect(bx, by, bw, bh, bg_color);

		// 枠線描画
		if (stroke_width > 0) {
			for (int32_t i = 0; i < stroke_width; ++i) {
				canvas->drawRect(bx + i, by + i, bw - 2 * i, bh - 2 * i, border_color);
			}
		}

		// 子要素を描画
		for (auto* child : children) {
			if (child->isVisible()) {
				child->draw();
			}
		}
	}

	void Panel::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
		bw = width;
		bh = height;
		bx = position.x + getAlignXOffset(bw);
		by = position.y;
	}

	void Panel::movePosition(const point& diff) {
		Element::movePosition(diff);
		for (auto* child : children) {
			child->movePosition(diff);
		}
	}

	void Panel::setPosition(const point& new_pos) {
		point diff = new_pos - position;
		movePosition(diff);
	}

	void Panel::setVisible(bool is_visible) {
		Element::setVisible(is_visible);
		for (auto* child : children) {
			child->setVisible(is_visible);
		}
	}

	void Panel::setEnabled(bool is_enabled) {
		Element::setEnabled(is_enabled);
		for (auto* child : children) {
			child->setEnabled(is_enabled);
		}
	}

	void Panel::update_impl(bool force_flush) {
		if (enabled) {
			for (auto* child : children) {
				if (child->update(force_flush)) {
					flush_required = true;
				}
			}
		}
	}

	void Panel::cancelPress() {
		Element::cancelPress();
		for (auto* child : children) {
			child->cancelPress();
		}
	}
}
