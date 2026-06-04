#ifndef UILIB_ELEMENTS_TOGGLESWITCH_H
#define UILIB_ELEMENTS_TOGGLESWITCH_H

#include "interactive_element.h"
#include <M5GFX.h>
#include <functional>

/**
 * @file toggleswitch.h
 * @brief トグルスイッチUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class ToggleSwitch
     * @brief 画面上でON/OFFを切り替えるカプセル形状のスイッチUI要素
     */
    class ToggleSwitch : public InteractiveElement {
    private:
        point size;                       ///< スイッチのサイズ (幅, 高さ)
        bool state = false;                ///< 現在のON/OFF状態 (true = ON, false = OFF)
        color_t on_color = 0x07E0;        ///< ON状態のトラック背景色 (緑: RGB565)
        color_t off_color = 0x7BEF;       ///< OFF状態のトラック背景色 (ダークグレー: RGB565)
        color_t thumb_color = 0xFFFF;     ///< スライドつまみ（サム）の色 (白: RGB565)

        /** @brief トグルされたときに実行されるコールバック関数 (引数に新状態のboolを取る) */
        std::function<void(bool)> on_toggle_callback = nullptr;

        /** @brief スイッチの当たり判定・描画境界を計算します */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        ToggleSwitch() = default;

        /**
         * @brief 詳細な初期値を指定するコンストラクタ
         * @param position 初期配置する座標位置
         * @param size スイッチの幅・高さ（デフォルトは {60, 30}）
         * @param initial_state 初期状態（デフォルトは false: OFF）
         * @param on_color ON時のトラック色（デフォルトは緑: 0x07E0）
         * @param off_color OFF時のトラック色（デフォルトはダークグレー: 0x7BEF）
         * @param thumb_color つまみの色（デフォルトは白: 0xFFFF）
         * @param parent_page 所属する親ページ（デフォルトはnullptr）
         */
        ToggleSwitch(const point& position, const point& size = {60, 30}, bool initial_state = false,
                     color_t on_color = 0x07E0, color_t off_color = 0x7BEF, color_t thumb_color = 0xFFFF,
                     Page* parent_page = nullptr);

        /**
         * @brief スイッチ（トラックと丸いつまみ）を描画します
         */
        void draw() override;

        /**
         * @brief タッチ検出およびデバウンス処理を行い、状態を更新します
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief スイッチがONかどうかを取得します
         * @return ONならtrue、OFFならfalse
         */
        bool isOn() const { return state; }

        /**
         * @brief スイッチのON/OFF状態を設定します（描画更新フラグも立ちます）
         * @param new_state 設定する新しい状態
         */
        void setState(bool new_state);

        /**
         * @brief トグル時のコールバック関数を登録します
         * @param callback 登録するコールバック関数 (引数として新しいstateのboolを受け取ります)
         */
        void setOnToggleCallback(std::function<void(bool state)> callback) { on_toggle_callback = callback; }
    };
}

#endif /* UILIB_ELEMENTS_TOGGLESWITCH_H */
