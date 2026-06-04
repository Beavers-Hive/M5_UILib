#ifndef UILIB_ELEMENTS_SLIDER_H
#define UILIB_ELEMENTS_SLIDER_H

#include "interactive_element.h"
#include <M5GFX.h>
#include <functional>

/**
 * @file slider.h
 * @brief スライダーUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class Slider
     * @brief 画面上で値を直感的にスライド操作できるバー状のUI要素
     */
    class Slider : public InteractiveElement {
    private:
        point size;                         ///< スライダー全体の描画領域サイズ (幅, 高さ)
        float min_value = 0.0f;             ///< 最小値
        float max_value = 100.0f;           ///< 最大値
        float step_size = 1.0f;             ///< 値の増減ステップ（刻み値）
        float value = 0.0f;                 ///< 現在の値

        color_t active_color = 0x07E0;      ///< 選択中（左側）のトラック色 (デフォルト: 緑: 0x07E0)
        color_t inactive_color = 0x7BEF;    ///< 未選択（右側）のトラック色 (デフォルト: ダークグレー: 0x7BEF)
        color_t thumb_color = 0xFFFF;       ///< つまみ（丸）の色 (デフォルト: 白: 0xFFFF)

        /** @brief 値変更時に呼び出されるコールバック関数 */
        std::function<void(float state)> on_value_change_callback = nullptr;

        /** @brief 値を最小・最大・ステップに適合するように丸めて制限します */
        float roundValue(float val) const;

        /** @brief スライダーの当たり判定・描画境界を計算します */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;
    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        Slider() = default;

        /**
         * @brief 詳細な初期値を指定するコンストラクタ
         * @param position 配置する座標位置（左上座標）
         * @param size スライダー全体の領域サイズ（デフォルトは {120, 20}）
         * @param min_val 最小値（デフォルトは 0.0f）
         * @param max_val 最大値（デフォルトは 100.0f）
         * @param step_val ステップ刻み値（デフォルトは 1.0f）
         * @param init_val 初期値（デフォルトは 0.0f）
         * @param active_col アクティブ（左側）のトラック色（デフォルトは緑: 0x07E0）
         * @param parent_page 所属する親ページ（デフォルトは nullptr）
         */
        Slider(const point& position, const point& size = {120, 20},
               float min_val = 0.0f, float max_val = 100.0f, float step_val = 1.0f, float init_val = 0.0f,
               color_t active_col = 0x07E0, Page* parent_page = nullptr);

        /**
         * @brief スライダー（トラックと丸いつまみ）を描画します
         */
        void draw() override;

        /**
         * @brief ドラッグ追従およびデバウンス判定を行い、値を更新します
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief 現在のスライダーの値を取得します
         * @return 現在の値
         */
        float getValue() const { return value; }

        /**
         * @brief プログラムからスライダーの値を設定します
         * @param new_value 設定する新しい値 (最小/最大/ステップに丸められます)
         */
        void setValue(float new_value);

        /**
         * @brief スライダーの値の範囲を再設定します
         * @param min_val 最小値
         * @param max_val 最大値
         */
        void setRange(float min_val, float max_val);

        /**
         * @brief スライダーのステップ幅を再設定します
         * @param step_val ステップ幅
         */
        void setStep(float step_val);

        /**
         * @brief 値変更時のコールバック関数を登録します
         * @param callback 登録するコールバック関数 (変更後の引数 float を受け取ります)
         */
        void setOnValueChangeCallback(std::function<void(float val)> callback) { on_value_change_callback = callback; }
    };
}

#endif /* UILIB_ELEMENTS_SLIDER_H */
