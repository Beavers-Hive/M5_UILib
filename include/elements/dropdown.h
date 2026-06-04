#ifndef UILIB_ELEMENTS_DROPDOWN_H
#define UILIB_ELEMENTS_DROPDOWN_H

#include "interactive_element.h"
#include <M5GFX.h>
#include <vector>
#include <functional>

/**
 * @file dropdown.h
 * @brief ドロップダウンUI要素クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class Dropdown
     * @brief タップで項目リストを展開し、スクロール選択可能なドロップダウンUI要素
     */
    class Dropdown : public InteractiveElement {
    private:
        std::vector<String> items;          ///< 選択肢のテキストリスト
        int selected_index = -1;            ///< 現在選択されている項目のインデックス (-1は未選択)
        bool is_open = false;               ///< リストが展開されている状態か
        int32_t width = 120;                ///< 通常時（閉じた状態）の幅
        int32_t height = 30;                ///< 通常時（閉じた状態）の高さ
        font_t font;                        ///< 使用するフォントデータへのポインタ
        color_t color = WHITE;              ///< テキストおよび枠線の色
        color_t bg_color = BLACK;           ///< ボックス背景色
        float scale = 1.0f;                 ///< 描画スケール

        // スクロールおよびタッチ判定用の内部変数
        float scroll_y = 0.0f;              ///< ピクセル単位の垂直スクロール量
        bool drag_active = false;           ///< ドラッグ（スワイプ）中かどうかのフラグ
        int32_t drag_start_y = 0;           ///< ドラッグ開始時の画面タッチY座標
        float drag_start_scroll_y = 0.0f;   ///< ドラッグ開始時のscroll_y値
        bool touch_pressed = false;         ///< タッチ中かどうか
        bool prev_pressed = false;          ///< 直前のタッチ状態
        int32_t touch_start_x = 0;          ///< タッチ開始時の画面X座標
        int32_t touch_start_y = 0;          ///< タッチ開始時の画面Y座標
        uint32_t touch_start_time = 0;      ///< タッチ開始時刻 (ミリ秒)

        // 展開したリストのレイアウト情報
        int32_t list_x = 0;                 ///< リストの左上描画X座標
        int32_t list_y = 0;                 ///< リストの左上描画Y座標
        int32_t list_w = 0;                 ///< リストの幅
        int32_t list_h = 0;                 ///< リストの高さ
        int32_t item_height = 24;           ///< 1項目の高さ
        int32_t max_visible_items = 4;      ///< 最大同時表示項目数

        std::function<void(int index)> on_change_callback = nullptr; ///< 選択値変更時のコールバック

        /** @brief 画面端検知を行い、リストの展開領域（x, y, w, h）を動的に計算します */
        void calculateListBounds();

        /** @brief リストの最大スクロール可能ピクセル数を取得します */
        float getMaxScrollY() const;

        /** @brief ドロップダウンボタンの境界領域を計算します */
        void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;
    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        Dropdown() = default;

        /**
         * @brief 詳細な初期値を指定するコンストラクタ
         * @param items ドロップダウンの選択肢一覧
         * @param position 通常時の左上座標
         * @param width ドロップダウンの幅
         * @param height 通常時の高さ
         * @param initial_index 初期選択インデックス (デフォルトは -1: 未選択)
         * @param color テキストおよび枠線の色 (デフォルトは WHITE)
         * @param bg_color 背景色 (デフォルトは BLACK)
         * @param scale 描画スケール (デフォルトは 1.0f)
         * @param parent_page 所属する親ページ (デフォルトは nullptr)
         * @param font 使用するフォント (デフォルトは &fonts::lgfxJapanGothicP_12)
         */
        Dropdown(const std::vector<String>& items, const point& position, int32_t width, int32_t height,
                 int initial_index = -1, color_t color = WHITE, color_t bg_color = BLACK, float scale = 1.0f,
                 Page* parent_page = nullptr, font_t font = &fonts::lgfxJapanGothicP_12);

        /**
         * @brief ドロップダウンを描画します（開状態の時はリスト部分も描画します）
         */
        virtual void draw() override;

        /**
         * @brief タッチ検出、リスト開閉、スクロール、項目タップ選択を行います
         */
        virtual void update_impl(bool force_flush = false) override;

        /**
         * @brief 現在の選択インデックスを取得します
         * @return 選択インデックス (未選択の場合は -1)
         */
        int getSelectedIndex() const { return selected_index; }

        /**
         * @brief 選択インデックスを設定します（変更時は再描画要求が立ちます）
         * @param index 設定するインデックス
         */
        void setSelectedIndex(int index);

        /**
         * @brief 現在選択されている項目のテキストを取得します
         * @return 選択テキスト（未選択時は空文字）
         */
        String getSelectedText() const;

        /**
         * @brief 選択項目変更時のコールバック関数を登録します
         * @param callback 登録するコールバック関数 (引数として新しい選択インデックスを受け取ります)
         */
        void setOnChangeCallback(std::function<void(int index)> callback) { on_change_callback = callback; }
    };
}

#endif /* UILIB_ELEMENTS_DROPDOWN_H */
