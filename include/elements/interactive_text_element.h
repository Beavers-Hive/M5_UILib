#ifndef UILIB_ELEMENTS_INTERACTIVE_TEXT_ELEMENT_H
#define UILIB_ELEMENTS_INTERACTIVE_TEXT_ELEMENT_H

#include "interactive_element.h"
#include <M5GFX.h>

/**
 * @file interactive_text_element.h
 * @brief タッチ操作可能で、かつテキスト表示を行うUI要素の中間基底クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class InteractiveTextElement
     * @brief タッチ判定（InteractiveElement）とテキスト情報（フォント、カラー、スケール、文字列）を併せ持つ中間基底クラス
     */
    class InteractiveTextElement : public InteractiveElement {
    protected:
        String text;                              ///< 表示・保持する文字列
        font_t font = &fonts::lgfxJapanGothicP_12; ///< 使用するフォントデータへのポインタ
        color_t color = WHITE;                    ///< テキスト・枠線のメイン描画色
        color_t bg_color = BLACK;                 ///< 要素の背景色
        float scale = 1.0f;                       ///< テキストの描画スケール倍率

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        InteractiveTextElement() = default;

        /**
         * @brief 初期値を指定するコンストラクタ
         */
        InteractiveTextElement(const String& text, const point& position, font_t font = &fonts::lgfxJapanGothicP_12,
                               color_t color = WHITE, color_t bg_color = BLACK, float scale = 1.0f,
                               Page* parent_page = nullptr);

        /**
         * @brief 表示文字列（テキスト）を設定します。変更時は再描画要求が立ちます。
         * @param new_text 設定する新しい文字列
         */
        virtual void setText(const String& new_text);

        /**
         * @brief 現在の表示文字列を取得します
         * @return 現在の文字列
         */
        virtual String getText() const;

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

#endif // UILIB_ELEMENTS_INTERACTIVE_TEXT_ELEMENT_H
