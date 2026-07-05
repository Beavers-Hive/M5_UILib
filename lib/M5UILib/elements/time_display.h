#ifndef UILIB_ELEMENTS_TIME_DISPLAY_H
#define UILIB_ELEMENTS_TIME_DISPLAY_H

#include "../element.h"
#include <M5GFX.h>

namespace uilib {
    /**
     * @class TimeDisplay
     * @brief RTCから時刻を自動取得し、ステータスバー等に配置可能な時刻表示UI要素
     */
    class TimeDisplay : public Element {
    private:
        font_t font = &fonts::Font0;             ///< 時刻表示用フォント
        color_t text_color = WHITE;              ///< 文字色
        uint32_t last_check_time = 0;            ///< 最後に時刻を更新した時間 (ミリ秒)
        char time_str[6] = "00:00";              ///< 現在のフォーマット済み時間文字列

        /**
         * @brief RTCから現在時刻を取得し表示用文字列を更新します
         */
        void updateTime();

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        TimeDisplay() : Element() {}

        /**
         * @brief 時刻表示要素を構築します
         * @param position 表示位置 (ステータスバーなどの親の自動整列により上書きされます)
         * @param parent_page 所属する親ページ
         */
        TimeDisplay(const point& position, Page* parent_page = nullptr);

        /**
         * @brief 時刻を親ページのキャンバスに描画します
         */
        void draw() override;

        /**
         * @brief 表示領域を取得します
         */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

        /**
         * @brief 状態更新処理（10秒おきにRTC時刻を確認して再描画フラグを設定）
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief 時刻表示の文字色を設定します
         */
        void setTextColor(color_t col) { text_color = col; flush_required = true; }

        /**
         * @brief 現在の文字色を取得します
         */
        color_t getTextColor() const { return text_color; }
    };
}

#endif // UILIB_ELEMENTS_TIME_DISPLAY_H
