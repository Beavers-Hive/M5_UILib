#ifndef UILIB_ELEMENTS_CHECKBOX_H
#define UILIB_ELEMENTS_CHECKBOX_H

#include "interactive_text_element.h"
#include <M5GFX.h>
#include <functional>

/**
 * @file checkbox.h
 * @brief チェックボックスUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class Checkbox
     * @brief 画面上でON/OFF状態（チェック）を切り替える四角いボックスとテキストラベルのUI要素
     */
    class Checkbox : public InteractiveTextElement {
    private:
        bool checked = false;               ///< チェック状態 (true: ON, false: OFF)

        /** @brief 値変更時に呼び出されるコールバック関数 */
        std::function<void(bool checked)> on_change_callback = nullptr;

        /** @brief チェックボックス全体の当たり判定および描画境界を計算します */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        Checkbox() = default;

        /**
         * @brief 詳細な初期値を指定するコンストラクタ
         * @param text チェックボックスの横に表示するラベル文字列
         * @param position ボックスの左上座標
         * @param initial_state 初期状態 (trueでチェックON)
         * @param color テキストおよびボックス枠線の色 (デフォルトは WHITE)
         * @param bg_color ボックスの背景色 (デフォルトは BLACK)
         * @param scale 描画スケール (デフォルトは 1.0f)
         * @param parent_page 所属する親ページ (デフォルトは nullptr)
         * @param font 使用するフォント (デフォルトは &fonts::lgfxJapanGothicP_12)
         */
        Checkbox(const String& text, const point& position, bool initial_state = false,
                 color_t color = WHITE, color_t bg_color = BLACK, float scale = 1.0f,
                 Page* parent_page = nullptr, font_t font = &fonts::lgfxJapanGothicP_12);

        /**
         * @brief チェックボックスを描画します
         */
        void draw() override;

        /**
         * @brief タッチ検出およびトグル処理を行い、状態を更新します
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief チェック状態を取得します
         * @return trueでチェックON、falseでチェックOFF
         */
        bool isChecked() const { return checked; }

        /**
         * @brief チェック状態を設定します（変更時は再描画要求が立ちます）
         * @param is_checked 設定するチェック状態
         */
        void setChecked(bool is_checked);

        /**
         * @brief チェック状態変化時のコールバック関数を登録します
         * @param callback 登録するコールバック関数 (引数として変更後の checked 状態を受け取ります)
         */
        void setOnChangeCallback(std::function<void(bool checked)> callback) { on_change_callback = callback; }
    };
}

#endif /* UILIB_ELEMENTS_CHECKBOX_H */
