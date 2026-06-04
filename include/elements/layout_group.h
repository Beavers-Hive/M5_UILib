#ifndef UILIB_LAYOUT_GROUP_H
#define UILIB_LAYOUT_GROUP_H

#include "panel.h"

/**
 * @file layout_group.h
 * @brief 子要素を自動的に整列配置するレイアウトグループクラス（VBox / HBox）および共通基底クラスを定義するヘッダーファイル
 */

namespace uilib {
	/**
	 * @class LayoutGroup
	 * @brief 子要素の自動配置をサポートする抽象レイアウトグループ基底クラス
	 */
	class LayoutGroup : public Panel {
	protected:
		int32_t spacing = 4;                      ///< 要素間の隙間
		int32_t padding_left = 4;                 ///< 左側の内側余白
		int32_t padding_right = 4;                ///< 右側の内側余白
		int32_t padding_top = 4;                  ///< 上側の内側余白
		int32_t padding_bottom = 4;               ///< 下側の内側余白

	public:
		/**
		 * @brief デフォルトコンストラクタ
		 */
		LayoutGroup() : Panel() {}

		/**
		 * @brief レイアウトグループを構築します
		 */
		LayoutGroup(const point& pos, int32_t w, int32_t h, color_t bg_col = BLACK, Page* parent_page = nullptr)
			: Panel(pos, w, h, bg_col, parent_page) {}

		/**
		 * @brief 子要素を自動整列する純粋仮想関数
		 */
		virtual void layoutChildren() = 0;

		/**
		 * @brief 子要素を追加し、レイアウトを更新します
		 */
		void addChild(Element* child) override {
			Panel::addChild(child);
			layoutChildren();
		}

		// --- 位置変更時のレイアウト更新オーバーライド ---

		void movePosition(const point& diff) override {
			Panel::movePosition(diff);
			layoutChildren();
		}

		void setPosition(const point& new_pos) override {
			Panel::setPosition(new_pos);
			layoutChildren();
		}

		// --- レイアウトパラメータ設定 ---

		virtual void setSpacing(int32_t s) { spacing = s; layoutChildren(); }
		int32_t getSpacing() const { return spacing; }

		virtual void setPadding(int32_t left, int32_t right, int32_t top, int32_t bottom) {
			padding_left = left;
			padding_right = right;
			padding_top = top;
			padding_bottom = bottom;
			layoutChildren();
		}
	};

	/**
	 * @class VBox
	 * @brief 子要素を垂直方向（上から下）に自動整列するレイアウトグループクラス
	 */
	class VBox : public LayoutGroup {
	public:
		/**
		 * @brief デフォルトコンストラクタ
		 */
		VBox() : LayoutGroup() {}

		/**
		 * @brief VBoxレイアウトグループを構築します
		 */
		VBox(const point& pos, int32_t w, int32_t h, color_t bg_col = BLACK, Page* parent_page = nullptr)
			: LayoutGroup(pos, w, h, bg_col, parent_page) {}

		/**
		 * @brief 子要素を垂直に自動整列します
		 */
		void layoutChildren() override;
	};

	/**
	 * @class HBox
	 * @brief 子要素を水平方向（左から右）に自動整列するレイアウトグループクラス
	 */
	class HBox : public LayoutGroup {
	public:
		/**
		 * @brief デフォルトコンストラクタ
		 */
		HBox() : LayoutGroup() {}

		/**
		 * @brief HBoxレイアウトグループを構築します
		 */
		HBox(const point& pos, int32_t w, int32_t h, color_t bg_col = BLACK, Page* parent_page = nullptr)
			: LayoutGroup(pos, w, h, bg_col, parent_page) {}

		/**
		 * @brief 子要素を水平に自動整列します
		 */
		void layoutChildren() override;
	};
}

#endif // UILIB_LAYOUT_GROUP_H
