#include "elements/wifi_icon.h"
#include "page.h"
#include <WiFi.h>
#include <Arduino.h>

namespace uilib {

    WiFiIcon::WiFiIcon(const point& position, Page* parent_page)
        : Element() {
        this->position = position;
        this->parent_page = parent_page;
        this->align = Align::RIGHT; // デフォルトはステータスバーに合わせ右寄せ
        forceUpdate();
    }

    void WiFiIcon::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (!parent_page) {
            bx = by = bw = bh = 0;
            return;
        }

        bw = width;
        bh = height;

        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void WiFiIcon::forceUpdate() {
        // WiFiが接続されているかをチェック
        // WiFi.status() が WL_CONNECTED の場合のみ接続中とみなす
        bool current_state = (WiFi.status() == WL_CONNECTED);
        if (current_state != connected) {
            connected = current_state;
            flush_required = true;
        }
    }

    void WiFiIcon::draw() {
        if (!parent_page || !visible) return;

        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        color_t col = connected ? active_color : inactive_color;

        // WiFiマークの描画 (上向き同心円アーク)
        // アークの中心点 (アイコンの下部中央)
        int32_t cx = bx + bw / 2;
        int32_t cy = by + bh - 2;

        // 1. 最下部の中央ドット
        canvas->fillCircle(cx, cy, 1, col);

        // 2. 内側アーク (半径 3-4, 225度から315度)
        canvas->drawArc(cx, cy, 3, 4, 225, 315, col);

        // 3. 中間アーク (半径 6-7, 225度から315度)
        canvas->drawArc(cx, cy, 6, 7, 225, 315, col);

        // 4. 外側アーク (半径 9-10, 225度から315度)
        canvas->drawArc(cx, cy, 9, 10, 225, 315, col);

        // 5. 切断時の斜線 (赤い斜め線)
        if (!connected) {
            // 左上から右下へ斜線を引き、未接続をわかりやすく示す
            canvas->drawLine(bx - 1, by - 1, bx + bw, by + bh, slash_color);
            canvas->drawLine(bx, by - 1, bx + bw + 1, by + bh, slash_color); // 太く見せるために2本の線
        }
    }

    void WiFiIcon::update_impl(bool force_flush) {
        if (update_interval >= 0) {
            if (force_flush || last_check_time == 0 || millis() - last_check_time >= (uint32_t)update_interval) {
                forceUpdate();
                last_check_time = millis();
            }
        }
    }
}
