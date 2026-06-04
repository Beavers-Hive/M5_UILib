#ifndef UILIB_GRID_H
#define UILIB_GRID_H

#include "layout_group.h"

/**
 * @file grid.h
 * @brief 子要素を格子状（マトリクス）に自動整列配置するグリッドレイアウトクラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class Grid
     * @brief 子要素を行方向・列方向（格子状）に自動整列するレイアウトグループクラス
     */
    class Grid : public LayoutGroup {
    private:
        int32_t rows = 1;                         ///< 行数
        int32_t cols = 1;                         ///< 列数
        int32_t col_spacing = 4;                  ///< 列間の隙間
        int32_t row_spacing = 4;                  ///< 行間の隙間

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        Grid() : LayoutGroup() {}

        /**
         * @brief Gridレイアウトグループを構築します
         * @param pos グローバル座標系での配置座標
         * @param w グリッドの幅
         * @param h グリッドの高さ
         * @param rows 行数
         * @param cols 列数
         * @param bg_col 背景色（デフォルトはBLACK）
         * @param parent_page 所属する親ページ（デフォルトはnullptr）
         */
        Grid(const point& pos, int32_t w, int32_t h, int32_t rows, int32_t cols, color_t bg_col = BLACK, Page* parent_page = nullptr);

        /**
         * @brief 子要素を格子状に自動整列します
         */
        void layoutChildren() override;

        // --- レイアウトパラメータ設定 ---

        /** @brief 行数を設定し、再整列します */
        void setRows(int32_t r) { rows = r > 0 ? r : 1; layoutChildren(); }
        int32_t getRows() const { return rows; }

        /** @brief 列数を設定し、再整列します */
        void setCols(int32_t c) { cols = c > 0 ? c : 1; layoutChildren(); }
        int32_t getCols() const { return cols; }

        /** @brief 隙間（列間、行間）を設定し、再整列します */
        void setSpacing(int32_t col_s, int32_t row_s) {
            col_spacing = col_s;
            row_spacing = row_s;
            layoutChildren();
        }
        int32_t getColSpacing() const { return col_spacing; }
        int32_t getRowSpacing() const { return row_spacing; }
    };
}

#endif // UILIB_GRID_H
