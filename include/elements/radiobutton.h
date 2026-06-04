#ifndef UILIB_ELEMENTS_RADIOBUTTON_H
#define UILIB_ELEMENTS_RADIOBUTTON_H

#include "interactive_text_element.h"
#include <M5GFX.h>
#include <vector>
#include <functional>

/**
 * @file radiobutton.h
 * @brief ラジオボタンおよびラジオボタングループUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
    class RadioButton;

    /**
     * @class RadioButtonGroup
     * @brief 複数のラジオボタンをグループ化し、いずれか1つのみを選択可能にする管理クラス
     */
    class RadioButtonGroup {
    private:
        int selected_value = -1;                          ///< 現在選択されている値
        std::vector<RadioButton*> buttons;                ///< グループに所属するラジオボタンのリスト
        std::function<void(int value)> on_change_callback = nullptr; ///< 値変更時のコールバック

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        RadioButtonGroup() = default;

        /**
         * @brief ラジオボタンをグループに追加します
         * @param button 追加するラジオボタンへのポインタ
         */
        void addButton(RadioButton* button);

        /**
         * @brief 指定した値を持つラジオボタンを選択し、他を非選択にします
         * @param value 選択するラジオボタンの値
         */
        void selectValue(int value);

        /**
         * @brief 現在選択されているラジオボタンの値を取得します
         * @return 選択値 (未選択の場合は -1)
         */
        int getSelectedValue() const { return selected_value; }

        /**
         * @brief グループ内の選択変更時のコールバック関数を登録します
         * @param callback 登録するコールバック関数 (引数として新しい選択値を受け取ります)
         */
        void setOnChangeCallback(std::function<void(int value)> callback) { on_change_callback = callback; }
    };

    /**
     * @class RadioButton
     * @brief ラジオボタングループに所属し、丸い枠線とテキストで構成される選択肢UI要素
     */
    class RadioButton : public InteractiveTextElement {
    private:
        int value = 0;                      ///< このボタンが持つ個別の選択値
        RadioButtonGroup* group = nullptr;  ///< 所属するラジオボタングループへのポインタ
        bool selected = false;              ///< 現在選択されているかどうかの状態

        friend class RadioButtonGroup;

        /** @brief グループから選択状態を同期設定するためのプライベート関数 */
        void setSelected(bool is_selected);

        /** @brief ボタン全体の当たり判定および描画境界を計算します */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        RadioButton() = default;

        /**
         * @brief 詳細な初期値を指定するコンストラクタ
         * @param text ラジオボタンの横に表示するラベル文字列
         * @param position ボタンの左上座標
         * @param value このボタンが表す固有の値
         * @param color テキストおよび円外枠の色 (デフォルトは WHITE)
         * @param bg_color 背景色 (デフォルトは BLACK)
         * @param scale 描画スケール (デフォルトは 1.0f)
         * @param parent_page 所属する親ページ (デフォルトは nullptr)
         * @param font 使用するフォント (デフォルトは &fonts::lgfxJapanGothicP_12)
         */
        RadioButton(const String& text, const point& position, int value,
                    color_t color = WHITE, color_t bg_color = BLACK, float scale = 1.0f,
                    Page* parent_page = nullptr, font_t font = &fonts::lgfxJapanGothicP_12);

        /**
         * @brief ラジオボタンを描画します
         */
        void draw() override;

        /**
         * @brief タッチ検出およびグループ選択の更新を行い、状態を更新します
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief ボタンが持つ固有値を取得します
         * @return ボタンの値
         */
        int getValue() const { return value; }

        /**
         * @brief ボタンが選択されているかを取得します
         * @return 選択中ならtrue、そうでないならfalse
         */
        bool isSelected() const { return selected; }
    };
}

#endif /* UILIB_ELEMENTS_RADIOBUTTON_H */
