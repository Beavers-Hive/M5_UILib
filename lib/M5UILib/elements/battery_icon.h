#ifndef UILIB_ELEMENTS_BATTERY_ICON_H
#define UILIB_ELEMENTS_BATTERY_ICON_H

#include "../element.h"
#include <M5GFX.h>

namespace uilib {

    /**
     * @class BatteryIcon
     * @brief バッテリー残量と充電状態をスマートフォンのステータスバーのように表示するUI要素
     */
    class BatteryIcon : public Element {
    private:
        bool show_percentage = true;             ///< パーセンテージ数値の表示フラグ
        font_t font = &fonts::Font0;             ///< テキスト表示用フォント
        color_t text_color = WHITE;              ///< テキストの文字色
        color_t border_color = WHITE;            ///< バッテリー外枠と端子の色
        int32_t width = 22;                      ///< バッテリー本体の幅 (デフォルト: 22)
        int32_t height = 11;                     ///< バッテリー本体の高さ (デフォルト: 11)
        uint32_t last_check_time = 0;            ///< 最後にハードウェア状態をチェックした時間（ミリ秒）
        int32_t cached_level = -1;               ///< キャッシュされた残量割合 (0-100)
        bool cached_charging = false;            ///< キャッシュされた充電状態

        /**
         * @brief 充電中の雷アイコンを描画します
         */
        void drawLightning(M5Canvas* canvas, int32_t cx, int32_t cy, color_t color);

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        BatteryIcon() : Element() {}

        /**
         * @brief バッテリーアイコン要素を構築します
         * @param position 表示位置 (通常は Align に従う基準位置。デフォルト Align::RIGHT の場合は右端)
         * @param show_pct パーセンテージ表示を有効にするか (デフォルト: true)
         * @param parent_page 所属する親ページ
         */
        BatteryIcon(const point& position, bool show_pct = true, Page* parent_page = nullptr);

        /**
         * @brief バッテリーアイコンを親ページのキャンバスに描画します
         */
        void draw() override;

        /**
         * @brief バッテリーアイコンの表示領域を取得します
         */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

        /**
         * @brief バッテリー状態の更新および再描画要否判定（5秒おきに実機残量をチェック）
         */
        void update_impl(bool force_flush = false) override;

        // --- 設定・カスタマイズメソッド ---

        /**
         * @brief 文字色を設定します
         */
        void setTextColor(color_t col) { text_color = col; flush_required = true; }

        /**
         * @brief 文字色を取得します
         */
        color_t getTextColor() const { return text_color; }

        /**
         * @brief バッテリー外枠の色を設定します
         */
        void setBorderColor(color_t col) { border_color = col; flush_required = true; }

        /**
         * @brief バッテリー外枠の色を取得します
         */
        color_t getBorderColor() const { return border_color; }

        /**
         * @brief パーセンテージ数値表示の有無を設定します
         */
        void setShowPercentage(bool show) { show_percentage = show; flush_required = true; }

        /**
         * @brief パーセンテージ数値表示が有効かどうかを取得します
         */
        bool isShowingPercentage() const { return show_percentage; }

        /**
         * @brief バッテリー本体の寸法を設定します
         */
        void setDimensions(int32_t w, int32_t h) { width = w; height = h; flush_required = true; }
    };
}

#endif // UILIB_ELEMENTS_BATTERY_ICON_H
