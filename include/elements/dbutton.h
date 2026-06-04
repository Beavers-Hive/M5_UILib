#ifndef UILIB_ELEMENTS_DBUTTON_H
#define UILIB_ELEMENTS_DBUTTON_H

#include "interactive_text_element.h"
#include <M5GFX.h>

/**
 * @file dbutton.h
 * @brief デジタルボタンUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class DButton
     * @brief 画面上にボタン（枠線およびテキスト）を描画するためのUI要素
     */
    class DButton : public InteractiveTextElement {
    private:
        int32_t margin_x = 0;             ///< 背景矩形の横方向マージン
        int32_t margin_y = 0;             ///< 背景矩形の縦方向マージン
        int32_t radius = 0;               ///< 背景矩形の角丸半径

        color_t pressed_color = WHITE;     ///< 押されている時のテキスト・枠線色
        color_t pressed_bg_color = BLACK;  ///< 押されている時の背景色

        /**
         * @brief ボタンの背景矩形の座標・サイズを取得します
         */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        DButton() = default;

        /**
         * @brief 詳細な初期値を指定するコンストラクタ
         * @param text ボタンに表示する文字列
         * @param position 初期配置する座標位置
         * @param color 文字・枠線の色（デフォルトはWHITE）
         * @param bg_color 背景色（デフォルトはBLACK）
         * @param scale 描画倍率（デフォルトは1.0f）
         * @param parent_page 所属する親ページ（デフォルトはnullptr）
         * @param font 使用するフォントデータ（デフォルトは &fonts::lgfxJapanGothicP_12）
         * @param margin_x 背景矩形の横マージン（デフォルトは0）
         * @param radius 背景矩形の角丸半径（デフォルトは0）
         * @param pressed_color 押されている時の文字・枠線色（デフォルトは-1: 未指定）
         * @param pressed_bg_color 押されている時の背景色（デフォルトは-1: 未指定）
         * @param margin_y 背景矩形の縦マージン（デフォルトは-1: margin_xと同じにする）
         */
        DButton(const String& text, const point& position, color_t color = WHITE, color_t bg_color = BLACK, float scale = 1.0f, Page* parent_page = nullptr, font_t font = &fonts::lgfxJapanGothicP_12, int32_t margin_x = 0, int32_t radius = 0, color_t pressed_color = -1, color_t pressed_bg_color = -1, int32_t margin_y = -1);

        /**
         * @brief 親ページのキャンバス上にボタン（枠線およびテキスト）を描画します
         */
        void draw() override;

        /**
         * @brief ボタン固有の更新処理を行います
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief 背景矩形のマージンを縦横一括で設定します。変更時は再描画要求が立ちます。
         * @param new_margin 新しいマージン値
         */
        void setMargin(int32_t new_margin) { margin_x = margin_y = new_margin; flush_required = true; }

        /**
         * @brief 背景矩形の縦横マージンを個別に設定します。変更時は再描画要求が立ちます。
         * @param mx 新しい横マージン値
         * @param my 新しい縦マージン値
         */
        void setMargin(int32_t mx, int32_t my) { margin_x = mx; margin_y = my; flush_required = true; }

        /**
         * @brief 背景矩形の横マージンを設定します。変更時は再描画要求が立ちます。
         * @param mx 新しい横マージン値
         */
        void setMarginX(int32_t mx) { margin_x = mx; flush_required = true; }

        /**
         * @brief 背景矩形の横マージンを取得します
         * @return 現在の横マージン値
         */
        int32_t getMarginX() const { return margin_x; }

        /**
         * @brief 背景矩形の縦マージンを設定します。変更時は再描画要求が立ちます。
         * @param my 新しい縦マージン値
         */
        void setMarginY(int32_t my) { margin_y = my; flush_required = true; }

        /**
         * @brief 背景矩形の縦マージンを取得します
         * @return 現在の縦マージン値
         */
        int32_t getMarginY() const { return margin_y; }

        /**
         * @brief 背景矩形の角丸半径を設定します。変更時は再描画要求が立ちます。
         * @param new_radius 新しい角丸半径
         */
        void setRadius(int32_t new_radius) { radius = new_radius; flush_required = true; }

        /**
         * @brief 現在の背景矩形の角丸半径を取得します
         * @return 現在の角丸半径
         */
        int32_t getRadius() const { return radius; }

        /**
         * @brief 押されている時のテキスト・枠線色を設定します
         * @param new_color 新しい文字色
         */
        void setPressedColor(color_t new_color) { pressed_color = new_color; }

        /**
         * @brief 押されている時のテキスト・枠線色を取得します
         * @return 現在の文字色
         */
        color_t getPressedColor() const { return pressed_color; }

        /**
         * @brief 押されている時の背景色を設定します
         * @param new_bg_color 新しい背景色
         */
        void setPressedBackgroundColor(color_t new_bg_color) { pressed_bg_color = new_bg_color; }

        /**
         * @brief 押されている時の背景色を取得します
         * @return 現在の背景色
         */
        color_t getPressedBackgroundColor() const { return pressed_bg_color; }
    };
}

#endif // UILIB_ELEMENTS_DBUTTON_H
