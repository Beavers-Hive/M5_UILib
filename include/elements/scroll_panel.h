#ifndef UILIB_SCROLL_PANEL_H
#define UILIB_SCROLL_PANEL_H

#include "panel.h"
#include <vector>

/**
 * @file scroll_panel.h
 * @brief スワイプスクロールおよびはみ出しクリッピングに対応したスクロールパネルコンテナクラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @enum ScrollDirection
     * @brief スクロール可能な方向を指定する列挙型
     */
    enum class ScrollDirection {
        VERTICAL,      ///< 垂直方向のみスクロール可能
        HORIZONTAL,    ///< 水平方向のみスクロール可能
        BOTH           ///< 縦横両方向スクロール可能
    };

    /**
     * @class ScrollPanel
     * @brief 子要素を格納し、境界サイズを超える領域をスワイプ操作でスクロール表示できるコンテナクラス
     */
    class ScrollPanel : public Panel {
    private:
        struct ScrollItem {
            Element* element;
            point local_position;
        };

        std::vector<ScrollItem> scroll_items;  ///< スクロール子要素とローカル座標のペアリスト
        point scroll_offset = {0, 0};         ///< 現在のスクロールオフセット値
        point max_scroll = {0, 0};            ///< スクロール可能な最大量
        ScrollDirection scroll_dir = ScrollDirection::BOTH; ///< スクロール方向の設定 (デフォルトはBOTH)

        // タッチ操作およびドラッグ制御用
        bool touch_active = false;
        bool drag_active = false;
        point touch_start_pos;
        point touch_start_scroll;
        uint32_t touch_start_time = 0;

        // 慣性スクロール（フリック）用
        point velocity = {0, 0};
        uint32_t last_update_time = 0;

        // スクロールバー表示用設定
        color_t scrollbar_color = 0xBDD7;     ///< スクロールバーの色 (デフォルトは明るいグレー)
        int32_t scrollbar_thickness = 4;      ///< スクロールバーの太さ
        int32_t scrollbar_margin = 2;         ///< スクロールバーの枠からのマージン

        /** @brief 子要素の配置範囲からスクロール可能最大量を算出します */
        void updateMaxScroll();

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        ScrollPanel() : Panel() {}

        /**
         * @brief スクロールパネルを構築します
         * @param pos パネルの左上座標
         * @param w パネルの幅
         * @param h パネルの高さ
         * @param bg_col 背景色 (デフォルトは BLACK)
         * @param parent_page 所属する親ページへのポインタ (デフォルトは nullptr)
         */
        ScrollPanel(const point& pos, int32_t w, int32_t h, color_t bg_col = BLACK, Page* parent_page = nullptr);

        /**
         * @brief パネルの背景、枠線、およびクリッピング矩形を適用して子要素を描画します
         */
        void draw() override;

        /**
         * @brief スワイプスクロール判定、フリック慣性計算、および子要素の座標更新とupdate()の委譲を行います
         */
        void update_impl(bool force_flush = false) override;

        /**
         * @brief スクロールパネルに子要素を追加します (Page側には直接登録しないことでクリッピング描画を可能にします)
         * @param child 追加する子要素へのポインタ
         */
        void addChild(Element* child) override;

        /**
         * @brief パネル自体の位置を相対移動し、子要素のローカル座標も連動させます
         */
        void movePosition(const point& diff) override;

        /**
         * @brief パネル自体の位置を設定し、子要素のローカル座標も連動させます
         */
        void setPosition(const point& new_pos) override;

        /**
         * @brief スクロール位置を手動設定します
         */
        void setScrollOffset(const point& offset);

        /**
         * @brief 現在のスクロール位置を取得します
         */
        point getScrollOffset() const { return scroll_offset; }

        /**
         * @brief スクロール可能な最大値を取得します
         */
        point getMaxScroll() const { return max_scroll; }

        /**
         * @brief スクロール可能な方向を設定します
         */
        void setScrollDirection(ScrollDirection dir) { scroll_dir = dir; }

        /**
         * @brief 現在のスクロール可能な方向を取得します
         */
        ScrollDirection getScrollDirection() const { return scroll_dir; }

        // スクロールバー外観の設定
        void setScrollbarColor(color_t col) { scrollbar_color = col; flush_required = true; }
        color_t getScrollbarColor() const { return scrollbar_color; }
        void setScrollbarThickness(int32_t t) { scrollbar_thickness = t; flush_required = true; }
        int32_t getScrollbarThickness() const { return scrollbar_thickness; }
        void setScrollbarMargin(int32_t m) { scrollbar_margin = m; flush_required = true; }
        int32_t getScrollbarMargin() const { return scrollbar_margin; }
    };
}

#endif // UILIB_SCROLL_PANEL_H
