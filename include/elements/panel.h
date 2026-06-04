#ifndef UILIB_PANEL_H
#define UILIB_PANEL_H

#include <vector>
#include "../element.h"

/**
 * @file panel.h
 * @brief 複数のUI要素を子要素として保持し、一括管理および背景枠線描画を行うパネルクラスを定義するヘッダーファイル
 */

namespace uilib {
	/**
	 * @class Panel
	 * @brief 子要素のコンテナ機能と自身の背景・枠線描画を提供するパネルクラス
	 */
	class Panel : public Element {
	protected:
		std::vector<Element*> children;          ///< 子要素リスト
		int32_t width;                           ///< パネルの幅
		int32_t height;                          ///< パネルの高さ
		color_t bg_color;                        ///< パネルの背景色
		int32_t stroke_width = 0;               ///< 枠線の太さ（0で枠線なし）
		Color border_color = Color(255, 255, 255); ///< 枠線の色

		/**
		 * @brief パネルの更新処理（子要素の状態変更を親ページに伝播）
		 */
		void update_impl(bool force_flush = false) override;

	public:
		/**
		 * @brief デフォルトコンストラクタ
		 */
		Panel() : Element(), width(0), height(0), bg_color(BLACK) {}

		/**
		 * @brief パネルを構築します
		 * @param pos グローバル座標系での配置座標
		 * @param w パネルの幅
		 * @param h パネルの高さ
		 * @param bg_col 背景色（デフォルトはBLACK）
		 * @param parent_page 所属する親ページ（デフォルトはnullptr）
		 */
		Panel(const point& pos, int32_t w, int32_t h, color_t bg_col = BLACK, Page* parent_page = nullptr);

		/**
		 * @brief パネルの背景と枠線を描画します
		 */
		void draw() override;

		/**
		 * @brief パネル自体の描画領域を取得します
		 */
		void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

		/**
		 * @brief パネルに子要素を追加します。未登録の場合は親ページへの追加も同時に行います。
		 * @param child 追加するUI要素へのポインタ
		 */
		virtual void addChild(Element* child);

		/**
		 * @brief 登録されているすべての子要素を取得します
		 */
		const std::vector<Element*>& getChildren() const { return children; }

		// --- 要素状態の一括同期オーバーライド ---

		void movePosition(const point& diff) override;
		void setPosition(const point& new_pos) override;
		void setVisible(bool is_visible) override;
		void setEnabled(bool is_enabled) override;
		void cancelPress() override;

		// --- パネル固有の設定メソッド ---

		void setBgColor(color_t col) { bg_color = col; flush_required = true; }
		color_t getBgColor() const { return bg_color; }

		void setStrokeWidth(int32_t sw) { stroke_width = sw; flush_required = true; }
		int32_t getStrokeWidth() const { return stroke_width; }

		void setBorderColor(Color col) { border_color = col; flush_required = true; }
		Color getBorderColor() const { return border_color; }

		void setDimensions(int32_t w, int32_t h) { width = w; height = h; flush_required = true; }
	};
}

#endif // UILIB_PANEL_H
