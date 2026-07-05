#include "elements/bluetooth_icon.h"
#include "page.h"
#include <Arduino.h>

namespace uilib {

    BluetoothIcon::BluetoothIcon(const point& position, Page* parent_page)
        : Element() {
        this->position = position;
        this->parent_page = parent_page;
        this->align = Align::RIGHT; // デフォルトはステータスバーに合わせ右寄せ
    }

    void BluetoothIcon::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
        if (!parent_page) {
            bx = by = bw = bh = 0;
            return;
        }

        bw = width;
        bh = height;

        bx = position.x + getAlignXOffset(bw);
        by = position.y;
    }

    void BluetoothIcon::draw() {
        if (!parent_page || !visible) return;

        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        color_t col = connected ? active_color : inactive_color;

        // Bluetoothシンボルマークの描画
        // 中心点
        int32_t cx = bx + bw / 2;
        int32_t cy = by + bh / 2;

        // 1. 縦の支柱 (cx, cy - 6) から (cx, cy + 6)
        canvas->drawLine(cx, cy - 6, cx, cy + 6, col);

        // 2. 左上から右下への対角線 (cx - 3, cy - 3) から (cx + 3, cy + 3)
        canvas->drawLine(cx - 3, cy - 3, cx + 3, cy + 3, col);

        // 3. 右下から下部支柱へ (cx + 3, cy + 3) から (cx, cy + 6)
        canvas->drawLine(cx + 3, cy + 3, cx, cy + 6, col);

        // 4. 上部支柱から右上へ (cx, cy - 6) から (cx + 3, cy - 3)
        canvas->drawLine(cx, cy - 6, cx + 3, cy - 3, col);

        // 5. 右上から左下への対角線 (cx + 3, cy - 3) から (cx - 3, cy + 3)
        canvas->drawLine(cx + 3, cy - 3, cx - 3, cy + 3, col);

        // 6. 切断時の斜線 (赤い斜め線)
        if (!connected) {
            // 左上から右下へ斜線を引き、未接続状態を示す
            canvas->drawLine(bx - 1, by - 1, bx + bw, by + bh, slash_color);
            canvas->drawLine(bx, by - 1, bx + bw + 1, by + bh, slash_color); // 太く見せるために2本の線
        }
    }

    void BluetoothIcon::update_impl(bool force_flush) {
        // Bluetooth状態は手動(setConnected)でのみ変更するため何もしない
    }
}
