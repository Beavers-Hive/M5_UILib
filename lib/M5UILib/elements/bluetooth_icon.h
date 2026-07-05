#ifndef UILIB_ELEMENTS_BLUETOOTH_ICON_H
#define UILIB_ELEMENTS_BLUETOOTH_ICON_H

#include "../element.h"
#include <M5GFX.h>

namespace uilib {
    /**
     * @class BluetoothIcon
     * @brief Bluetoothの接続ステータスをアイコンで表示するUI要素
     */
    class BluetoothIcon : public Element {
    private:
        bool connected = false;                  ///< 接続状態フラグ
        color_t active_color = WHITE;            ///< 接続時のアイコン描画色
        color_t inactive_color = 0x4A69;         ///< 未接続時のアイコン描画色
        color_t slash_color = RED;               ///< 切断時斜線の描画色
        int32_t width = 10;                      ///< アイコン描画幅
        int32_t height = 14;                     ///< アイコン描画高

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        BluetoothIcon() : Element() {}

        /**
         * @brief BluetoothIconを構築します
         * @param position 基準位置
         * @param parent_page 所属する親ページ
         */
        BluetoothIcon(const point& position, Page* parent_page = nullptr);

        void draw() override;
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;
        void update_impl(bool force_flush = false) override;

        /**
         * @brief Bluetoothの接続状態を設定します
         */
        void setConnected(bool conn) {
            if (connected != conn) {
                connected = conn;
                flush_required = true;
            }
        }

        /**
         * @brief 現在の接続状態を取得します
         */
        bool isConnected() const { return connected; }

        /**
         * @brief 接続状態のアクティブ色を設定します
         */
        void setActiveColor(color_t col) { active_color = col; flush_required = true; }

        /**
         * @brief 未接続時の色を設定します
         */
        void setInactiveColor(color_t col) { inactive_color = col; flush_required = true; }
    };
}

#endif // UILIB_ELEMENTS_BLUETOOTH_ICON_H
