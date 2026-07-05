#ifndef UILIB_ELEMENTS_STATUSBAR_H
#define UILIB_ELEMENTS_STATUSBAR_H

#include "elements/panel.h"

namespace uilib {
    /**
     * @class StatusBar
     * @brief 複数のUI要素を位置アライメント（左右中央）に応じて自動整列させるステータスバーレイアウトコンテナ
     */
    class StatusBar : public Panel {
    private:
        int32_t padding_left = 6;                ///< 左側のインサイドパディング
        int32_t padding_right = 6;               ///< 右側のインサイドパディング
        int32_t spacing = 6;                     ///< 要素間の隙間（スペーシング）

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        StatusBar() : Panel() {}

        /**
         * @brief ステータスバーを構築します (標準サイズは 320x18)
         * @param parent_page 所属する親ページ
         */
        StatusBar(Page* parent_page = nullptr);

        /**
         * @brief ステータスバーと底部の枠線、および子要素を描画します
         */
        void draw() override;

        /**
         * @brief 子要素の配置アライメントを解析し、左右中央に自動配置します
         */
        void layoutChildren();

        /**
         * @brief 子要素を追加し、レイアウトを再計算します
         * @param child 追加するUI要素
         */
        void addChild(Element* child) override;

        // --- 位置変更のオーバーライド（位置が変わった際も再整列する） ---
        void movePosition(const point& diff) override;
        void setPosition(const point& new_pos) override;

        /**
         * @brief ステータスバーとすべての子要素の状態更新処理
         */
        void update_impl(bool force_flush = false) override;

        // --- レイアウトカスタマイズメソッド ---
        void setPadding(int32_t left, int32_t right) { padding_left = left; padding_right = right; layoutChildren(); }
        void setSpacing(int32_t sp) { spacing = sp; layoutChildren(); }
    };
}

#endif // UILIB_ELEMENTS_STATUSBAR_H
