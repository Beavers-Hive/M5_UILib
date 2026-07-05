#include "elements/time_display.h"
#include "page.h"
#include <M5Unified.h>
#include <Arduino.h>

namespace uilib {

    TimeDisplay::TimeDisplay(const point& position, Page* parent_page)
        : Element() {
        this->position = position;
        this->parent_page = parent_page;
        this->align = Align::CENTER; // デフォルトはステータスバーらしく中央寄せ
        updateTime();
    }

    void TimeDisplay::updateTime() {
        auto t = M5.Rtc.getTime();
        snprintf(time_str, sizeof(time_str), "%02d:%02d", t.hours, t.minutes);
    }

    void TimeDisplay::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (!parent_page) {
            bx = by = bw = bh = 0;
            return;
        }

        auto canvas = parent_page->getCanvas();
        canvas->setFont(font);
        canvas->setTextSize(1.0f);

        // 時刻が変化した時のサイズ変更を防ぐため、常に "00:00" 相当の幅を固定として返します
        bw = canvas->textWidth("00:00");
        bh = canvas->fontHeight();

        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void TimeDisplay::draw() {
        if (!parent_page || !visible) return;

        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        canvas->setFont(font);
        canvas->setTextSize(1.0f);
        canvas->setTextColor(text_color);

        // 文字列の幅を再計算して中央揃えで描画
        int32_t tw = canvas->textWidth(time_str);
        int32_t tx = bx + (bw - tw) / 2;
        canvas->drawString(time_str, tx, by);
    }

    void TimeDisplay::update_impl(bool force_flush) {
        // 時刻は頻繁に読み出す必要はないため、10秒おきにチェックします
        if (force_flush || last_check_time == 0 || millis() - last_check_time >= 10000) {
            char prev_time[6];
            memcpy(prev_time, time_str, 6);
            updateTime();

            if (strcmp(prev_time, time_str) != 0) {
                flush_required = true;
            }
            last_check_time = millis();
        }
    }
}
