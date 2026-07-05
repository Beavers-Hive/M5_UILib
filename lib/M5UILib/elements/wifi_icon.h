#ifndef UILIB_ELEMENTS_WIFI_ICON_H
#define UILIB_ELEMENTS_WIFI_ICON_H

#include "../element.h"
#include <M5GFX.h>

namespace uilib {
    /**
     * @class WiFiIcon
     * @brief WiFiの接続ステータスをアイコンで表示するUI要素
     */
    class WiFiIcon : public Element {
    private:
        int32_t update_interval = 3000;          ///< 更新周期 (ミリ秒)。-1 で自動更新なし
        uint32_t last_check_time = 0;            ///< 最後に状態を更新した時間 (ミリ秒)
        bool connected = false;                  ///< 接続状態キャッシュ
        color_t active_color = WHITE;            ///< 接続時のアイコン描画色
        color_t inactive_color = 0x4A69;         ///< 未接続時のアイコン描画色
        color_t slash_color = RED;               ///< 切断時斜線の描画色
        int32_t width = 14;                      ///< アイコン描画幅
        int32_t height = 12;                     ///< アイコン描画高

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        WiFiIcon() : Element() {}

        /**
         * @brief WiFiIconを構築します
         * @param position 基準位置
         * @param parent_page 所属する親ページ
         */
        WiFiIcon(const point& position, Page* parent_page = nullptr);

        void draw() override;
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;
        void update_impl(bool force_flush = false) override;

        /**
         * @brief WiFi接続状態を手動で強制チェック・更新します
         */
        void forceUpdate();

        /**
         * @brief 自動接続確認の更新頻度を設定します
         * @param interval_ms 更新周期 (ミリ秒)。-1 を渡すと自動更新を停止します
         */
        void setUpdateInterval(int32_t interval_ms) { update_interval = interval_ms; }

        /**
         * @brief 現在の更新頻度設定を取得します
         */
        int32_t getUpdateInterval() const { return update_interval; }

        /**
         * @brief 接続状態のアクティブ色を設定します
         */
        void setActiveColor(color_t col) { active_color = col; flush_required = true; }

        /**
         * @brief 未接続時の色を設定します
         */
        void setInactiveColor(color_t col) { inactive_color = col; flush_required = true; }

        /**
         * @brief 接続状態フラグの現在値（キャッシュ）を直接取得します
         */
        bool isConnected() const { return connected; }
    };
}

#endif // UILIB_ELEMENTS_WIFI_ICON_H
