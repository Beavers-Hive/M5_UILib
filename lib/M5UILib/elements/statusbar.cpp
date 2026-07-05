#include "elements/statusbar.h"
#include "page.h"
#include <vector>
#include <Arduino.h>

namespace uilib {

    StatusBar::StatusBar(Page* parent_page)
        : Panel({0, 0}, 320, 18, 0x1082, parent_page) { // Y:0, W:320, H:18, Color: dark gray
        setStrokeWidth(0); // 枠線はカスタムで下に引くため、Panelの標準枠線はオフ
    }

    void StatusBar::addChild(Element* child) {
        if (!child) return;
        Panel::addChild(child);
        layoutChildren();
    }

    void StatusBar::layoutChildren() {
        if (!parent_page) return;

        // 子要素をアライメントごとに分類
        std::vector<Element*> left_elems;
        std::vector<Element*> center_elems;
        std::vector<Element*> right_elems;

        for (auto* child : children) {
            if (!child->isVisible()) continue;

            switch (child->getAlign()) {
                case Align::LEFT:
                    left_elems.push_back(child);
                    break;
                case Align::CENTER:
                    center_elems.push_back(child);
                    break;
                case Align::RIGHT:
                    right_elems.push_back(child);
                    break;
            }
        }

        int32_t bar_h = height;

        // 1. 左寄せの配置（追加された順に左から右へ）
        int32_t curr_left_x = position.x + padding_left;
        for (auto* child : left_elems) {
            int32_t bx, by, bw, bh;
            child->getBounds(bx, by, bw, bh);

            int32_t target_y = position.y + (bar_h - bh) / 2;

            // 位置アライメントに基づく補正オフセットを適用
            int32_t dx = child->getPosition().x - bx;
            int32_t dy = child->getPosition().y - by;

            child->setPosition({curr_left_x + dx, target_y + dy});

            curr_left_x += bw + spacing;
        }

        // 2. 右寄せの配置（追加された順に右から左へ）
        int32_t curr_right_x = position.x + width - padding_right;
        for (auto* child : right_elems) {
            int32_t bx, by, bw, bh;
            child->getBounds(bx, by, bw, bh);

            int32_t target_y = position.y + (bar_h - bh) / 2;
            int32_t target_x = curr_right_x - bw;

            // 位置アライメントに基づく補正オフセットを適用
            int32_t dx = child->getPosition().x - bx;
            int32_t dy = child->getPosition().y - by;

            child->setPosition({target_x + dx, target_y + dy});

            curr_right_x -= (bw + spacing);
        }

        // 3. 中央寄せの配置（全中央要素を合算した幅のブロックを中央揃えにして配置）
        int32_t total_center_w = 0;
        for (auto* child : center_elems) {
            int32_t bx, by, bw, bh;
            child->getBounds(bx, by, bw, bh);
            total_center_w += bw;
        }
        if (!center_elems.empty()) {
            total_center_w += spacing * (center_elems.size() - 1);
        }

        int32_t curr_center_x = position.x + (width - total_center_w) / 2;
        for (auto* child : center_elems) {
            int32_t bx, by, bw, bh;
            child->getBounds(bx, by, bw, bh);

            int32_t target_y = position.y + (bar_h - bh) / 2;

            // 位置アライメントに基づく補正オフセットを適用
            int32_t dx = child->getPosition().x - bx;
            int32_t dy = child->getPosition().y - by;

            child->setPosition({curr_center_x + dx, target_y + dy});

            curr_center_x += bw + spacing;
        }
    }

    void StatusBar::movePosition(const point& diff) {
        Panel::movePosition(diff);
        layoutChildren();
    }

    void StatusBar::setPosition(const point& new_pos) {
        point diff = new_pos - position;
        movePosition(diff);
    }

    void StatusBar::draw() {
        if (!parent_page || !visible) return;

        auto canvas = parent_page->getCanvas();
        if (!canvas) return;

        // 1. Panelの背景と枠線を描画（子要素もこの中で自動描画されます）
        Panel::draw();

        // 2. ステータスバー下部の境界線のカスタム描画
        // Y=17 に境界線を引くことで他の画面要素と境界を明示します
        canvas->drawFastHLine(position.x, position.y + height - 1, width, 0x2945); // やや明るいグレー枠線 (RGB565)
    }

    void StatusBar::update_impl(bool force_flush) {
        // パネル配下の子要素を全てアップデート
        Panel::update_impl(force_flush);
    }
}
