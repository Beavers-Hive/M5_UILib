#ifndef UILIB_TAB_PANEL_H
#define UILIB_TAB_PANEL_H

#include "panel.h"
#include <vector>

/**
 * @file tab_panel.h
 * @brief タブ選択による表示・操作パネルの切り替えをサポートするコンテナクラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class TabPanel
     * @brief 複数の子パネルをタブで切り替えて排他表示・操作できるコンテナクラス
     */
    class TabPanel : public Panel {
    private:
        struct TabItem {
            String name;
            Panel* content_panel;
        };

        std::vector<TabItem> tabs;              ///< タブ名と子パネルのリスト
        int32_t selected_index = -1;            ///< 現在選択されているアクティブタブのインデックス
        int32_t tab_bar_height = 32;            ///< タブバーの縦幅

        // タブバーの描画カスタム用カラー（必要に応じてカスタマイズ可能にする）
        color_t bar_bg_color = 0x2104;          ///< タブバー背景色（濃いグレー）
        color_t active_tab_color = 0x18C3;      ///< 選択中タブの背景色（明るいグレー）
        color_t inactive_tab_color = 0x0841;    ///< 非選択中タブの背景色（暗いグレー）
        color_t text_color = WHITE;             ///< テキスト表示色
        color_t active_border_color = GREEN;    ///< アクティブタブの上部境界線色

        font_t tab_font = &fonts::efontJA_14;   ///< タブ名表示用フォント
        float font_scale = 1.0f;                ///< フォント描画スケール

        // タッチ操作用
        bool touch_active = false;

        /** @brief タブ切り替え時に非アクティブになったパネルのタッチ状態をキャンセルします */
        void deactivateTab(int32_t index);

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        TabPanel() : Panel() {}

        /**
         * @brief タブパネルを構築します
         */
        TabPanel(const point& pos, int32_t w, int32_t h, color_t bg_col = BLACK, Page* parent_page = nullptr);

        /**
         * @brief タブパネルに新しいタブを追加します
         * @param name タブバーに表示する名前
         * @param panel タブ選択時に表示されるコンテンツパネル
         */
        void addTab(const String& name, Panel* panel);

        /**
         * @brief アクティブなタブをインデックス指定で切り替えます
         * @param index 切り替え先のタブインデックス
         */
        void selectTab(int32_t index);

        /**
         * @brief 現在選択されているタブのインデックスを取得します
         * @return タブインデックス。登録がない場合は -1
         */
        int32_t getSelectedIndex() const { return selected_index; }

        /**
         * @brief タブバーと現在アクティブなタブパネルを描画します
         */
        void draw() override;

        /**
         * @brief タブバー部分のタッチ判定およびアクティブな子パネルの update の委譲を行います
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief パネル自体の位置を相対移動し、登録されているすべてのタブ用子パネルの位置も連動させます
         */
        void movePosition(const point& diff) override;

        /**
         * @brief パネル自体の位置を設定し、登録されているすべてのタブ用子パネルの位置も連動させます
         */
        void setPosition(const point& new_pos) override;

        // タブバーの外観カスタマイズ用セッター
        void setTabBarHeight(int32_t h) { tab_bar_height = h; setPosition(position); }
        void setTabBarColors(color_t bar_bg, color_t active_bg, color_t inactive_bg, color_t txt_col, color_t border_col) {
            bar_bg_color = bar_bg;
            active_tab_color = active_bg;
            inactive_tab_color = inactive_bg;
            text_color = txt_col;
            active_border_color = border_col;
            flush_required = true;
        }
        void setTabFont(font_t font, float scale = 1.0f) { tab_font = font; font_scale = scale; flush_required = true; }
    };
}

#endif // UILIB_TAB_PANEL_H
