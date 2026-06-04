#include "elements/grid.h"
#include "page.h"

namespace uilib {

    Grid::Grid(const point& pos, int32_t w, int32_t h, int32_t rows, int32_t cols, color_t bg_col, Page* parent_page)
        : LayoutGroup(pos, w, h, bg_col, parent_page), rows(rows > 0 ? rows : 1), cols(cols > 0 ? cols : 1) {}

    void Grid::layoutChildren() {
        if (!parent_page) return;

        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // 1つのセルに割り当てられる利用可能な幅と高さを計算
        // spacingとpaddingを差し引いた領域を分割する
        int32_t total_col_spacing = (cols - 1) * col_spacing;
        int32_t total_row_spacing = (rows - 1) * row_spacing;
        
        int32_t cell_w = (bw - padding_left - padding_right - total_col_spacing) / cols;
        int32_t cell_h = (bh - padding_top - padding_bottom - total_row_spacing) / rows;

        if (cell_w <= 0 || cell_h <= 0) return;

        for (size_t i = 0; i < children.size(); ++i) {
            auto* child = children[i];
            if (!child->isVisible()) continue;

            // 行と列のインデックスを計算 (Row-Major)
            int32_t r = i / cols;
            int32_t c = i % cols;
            
            // セル左上の座標を計算
            int32_t cell_x = bx + padding_left + c * (cell_w + col_spacing);
            int32_t cell_y = by + padding_top + r * (cell_h + row_spacing);

            // 子要素の本来のバウンディングボックスを取得
            int32_t cbx, cby, cbw, cbh;
            child->getBounds(cbx, cby, cbw, cbh);

            // 子要素のアライメント・マージンによる補正オフセットを計算
            int32_t offset_x = cbx - child->getPosition().x;
            int32_t offset_y = cby - child->getPosition().y;

            // セル内で子要素をセンタリング（中央配置）する
            int32_t target_x = cell_x + (cell_w - cbw) / 2;
            int32_t target_y = cell_y + (cell_h - cbh) / 2;

            int32_t new_x = target_x - offset_x;
            int32_t new_y = target_y - offset_y;

            child->setPosition({ new_x, new_y });
        }
    }
}
