#ifndef UILIB_ELEMENTS_TEXT_ELEMENT_H
#define UILIB_ELEMENTS_TEXT_ELEMENT_H

#include "../element.h"
#include <M5GFX.h>

/**
 * @file text_element.h
 * @brief テキスト属性（フォント、文字色、背景色、スケール）を管理する中間基底クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class TextElement
     * @brief フォント設定、カラー設定、サイズスケールなどのテキスト描画用アトリビュートを管理する抽象中間基底クラス
     */
    class TextElement : public Element {
    protected:
        font_t font = &fonts::lgfxJapanGothicP_12; ///< 使用するフォントデータへのポインタ
        color_t color = WHITE;                  ///< テキストの描画色
        color_t bg_color = BLACK;               ///< テキストの背景色
        float scale = 1.0f;                     ///< テキストの描画スケール倍率

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        TextElement() = default;

        /**
         * @brief 初期属性を指定するコンストラクタ
         */
        TextElement(font_t font, color_t color, color_t bg_color, float scale);

        /**
         * @brief フォントを設定します。変更時は再描画要求が立ちます。
         * @param new_font 新しいフォントデータ
         */
        virtual void setFont(font_t new_font);

        /**
         * @brief 現在設定されているフォントを取得します
         * @return フォントデータへのポインタ
         */
        font_t getFont() const;

        /**
         * @brief 文字色を設定します。変更時は再描画要求が立ちます。
         * @param new_color 新しい文字色
         */
        virtual void setColor(color_t new_color);

        /**
         * @brief 現在の文字色を取得します
         * @return 現在の文字色
         */
        color_t getColor() const;

        /**
         * @brief 背景色を設定します。変更時は再描画要求が立ちます。
         * @param new_bg_color 新しい背景色
         */
        virtual void setBackground(color_t new_bg_color);

        /**
         * @brief 現在の背景色を取得します
         * @return 現在の背景色
         */
        color_t getBackground() const;

        /**
         * @brief 文字倍率（スケール）を設定します。変更時は再描画要求が立ちます。
         * @param new_scale 新しいスケール倍率（例: 2.0f）
         */
        virtual void setScale(float new_scale);

        /**
         * @brief 現在の文字倍率（スケール）を取得します
         * @return 現在のスケール倍率
         */
        float getScale() const;
    };
}

#endif // UILIB_ELEMENTS_TEXT_ELEMENT_H
