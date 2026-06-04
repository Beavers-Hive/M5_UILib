#include "elements/layout_group.h"
#include "page.h"

namespace uilib {

	// ==========================================
	// VBox (垂直レイアウトグループ) の実装
	// ==========================================

	void VBox::layoutChildren() {
		if (!parent_page) return;

		int32_t bx, by, bw, bh;
		getBounds(bx, by, bw, bh);

		int32_t current_y = by + padding_top;

		for (auto* child : children) {
			if (!child->isVisible()) continue;

			int32_t cbx, cby, cbw, cbh;
			child->getBounds(cbx, cby, cbw, cbh);

			// X座標アライメント・マージン補正
			int32_t target_x = bx + padding_left;
			int32_t offset_x = cbx - child->getPosition().x;
			int32_t new_x = target_x - offset_x;

			// Y座標マージン補正
			int32_t offset_y = cby - child->getPosition().y;
			int32_t new_y = current_y - offset_y;

			child->setPosition({ new_x, new_y });

			// 次の要素の配置開始位置Yを更新
			current_y += cbh + spacing;
		}
	}

	// ==========================================
	// HBox (水平レイアウトグループ) の実装
	// ==========================================

	void HBox::layoutChildren() {
		if (!parent_page) return;

		int32_t bx, by, bw, bh;
		getBounds(bx, by, bw, bh);

		int32_t current_x = bx + padding_left;

		for (auto* child : children) {
			if (!child->isVisible()) continue;

			int32_t cbx, cby, cbw, cbh;
			child->getBounds(cbx, cby, cbw, cbh);

			// X座標マージン補正
			int32_t offset_x = cbx - child->getPosition().x;
			int32_t new_x = current_x - offset_x;

			// Y座標アライメント・マージン補正
			int32_t target_y = by + padding_top;
			int32_t offset_y = cby - child->getPosition().y;
			int32_t new_y = target_y - offset_y;

			child->setPosition({ new_x, new_y });

			// 次の要素の配置開始位置Xを更新
			current_x += cbw + spacing;
		}
	}
}
