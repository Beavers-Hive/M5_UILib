#ifndef UILIB_ELEMENTS_LABEL_H
#define UILIB_ELEMENTS_LABEL_H

#include "text_element.h"
#include <M5GFX.h>

/**
 * @file label.h
 * @brief ラベルUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
	/**
	 * @class Label
	 * @brief 画面上に任意の文字列（テキスト）を描画するためのUI要素
	 */
	class Label : public TextElement {
	private:
		String text;                      ///< 描画するテキスト
		int32_t margin = 0;               ///< 背景矩形のマージン
		int32_t radius = 0;               ///< 背景矩形の角丸半径

	public:
		/**
		 * @brief デフォルトコンストラクタ
		 */
		Label() = default;

		/**
		 * @brief 詳細な初期値を指定するコンストラクタ
		 * @param text 初期表示する文字列
		 * @param position 初期配置する座標位置
		 * @param color 文字色（デフォルトはWHITE）
		 * @param bg_color 背景色（デフォルトはBLACK）
		 * @param scale 描画倍率（デフォルトは1.0f）
		 * @param parent_page 所属する親ページ（デフォルトはnullptr）
		 * @param font 使用するフォントデータ（デフォルトは &fonts::lgfxJapanGothicP_12）
		 * @param margin 背景矩形のマージン（デフォルトは0）
		 * @param radius 背景矩形の角丸半径（デフォルトは0）
		 */
		Label(const String& text, const point& position, color_t color = WHITE, color_t bg_color = BLACK, float scale = 1.0f, Page* parent_page = nullptr, font_t font = &fonts::lgfxJapanGothicP_12, int32_t margin = 0, int32_t radius = 0);

		/**
		 * @brief 親ページのキャンバス上に文字列を描画します
		 */
		void draw() override;

		/**
		 * @brief ラベルの描画領域（境界）を取得します
		 */
		void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

		/**
		 * @brief ラベル固有の更新処理を行います（空実装）
		 */
		void update_impl(bool force_flush = false) override {}

		/**
		 * @brief 表示テキストを設定します。変更時は再描画要求（flush_required）が立ちます。
		 * @param new_text 新しい文字列
		 */
		void set(const String& new_text);

		/**
		 * @brief 現在の表示テキストを取得します
		 * @return 現在の文字列
		 */
		String get() const;

		/**
		 * @brief 背景矩形のマージンを設定します。変更時は再描画要求が立ちます。
		 * @param new_margin 新しいマージン値
		 */
		void setMargin(int32_t new_margin);

		/**
		 * @brief 現在の背景矩形のマージンを取得します
		 * @return 現在のマージン値
		 */
		int32_t getMargin() const;

		/**
		 * @brief 背景矩形の角丸半径を設定します。変更時は再描画要求が立ちます。
		 * @param new_radius 新しい角丸半径
		 */
		void setRadius(int32_t new_radius);

		/**
		 * @brief 現在の背景矩形の角丸半径を取得します
		 * @return 現在の角丸半径
		 */
		int32_t getRadius() const;
	};
}

#endif // UILIB_ELEMENTS_LABEL_H