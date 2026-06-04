#include <M5Unified.h>
#include "elements/message_box.h"
#include "page.h"

namespace uilib {
    MessageBox::MessageBox(const point& pos, int32_t w, int32_t h, color_t bg_col, Page* parent_page)
    : Panel(pos, w, h, bg_col, parent_page) {
        visible = false;
        enabled = false;
        stroke_width = 2;
        border_color = Color(WHITE);

        // タイトルと本文
        label_title = Label("Title", {w / 2, 10}, WHITE, bg_col, 1.0f, parent_page, &fonts::lgfxJapanGothic_20);
        label_title.setAlign(Align::CENTER);

        label_message = Label("Message", {w / 2, 45}, WHITE, bg_col, 1.0f, parent_page, &fonts::efontJA_14);
        label_message.setAlign(Align::CENTER);

        // ボタン (margin_y=6 を指定してボタン縦幅を適正な26pxにする)
        btn_primary = DButton("OK", {0, 0}, WHITE, uilib::Color(50, 150, 50), 1.0f, parent_page, &fonts::efontJA_14, 18, 4, -1, -1, 6);
        btn_secondary = DButton("Cancel", {0, 0}, WHITE, uilib::Color(150, 50, 50), 1.0f, parent_page, &fonts::efontJA_14, 18, 4, -1, -1, 6);

        btn_primary.setAlign(Align::CENTER);
        btn_secondary.setAlign(Align::CENTER);

        // 親ポインタを設定するために addChild する
        addChild(&label_title);
        addChild(&label_message);
        addChild(&btn_primary);
        addChild(&btn_secondary);
    }

    void MessageBox::show(const String& title, const String& message, MessageBoxType type, std::function<void(MessageBoxResult)> cb) {
        box_type = type;
        result_callback = cb;

        // 各ボタンのテキストと配置設定 (ダイアログ自体の絶対座標 bx, by にオフセットを適用)
        int32_t bx, by, bw, bh;
        getBounds(bx, by, bw, bh);

        // ラベルの位置を絶対座標に設定
        label_title.setPosition({bx + bw / 2, by + 12});
        label_title.set(title);

        label_message.setPosition({bx + bw / 2, by + 50});
        label_message.set(message);

        // ボタンのY座標 (メッセージボックス下部から 30px 上の絶対座標)
        int32_t btn_y = by + bh - 30;

        if (type == MessageBoxType::OK) {
            btn_primary.setText("OK");
            btn_primary.setPosition({bx + bw / 2, btn_y});
            btn_primary.setVisible(true);
            btn_primary.setEnabled(true);

            btn_secondary.setVisible(false);
            btn_secondary.setEnabled(false);
        }
        else if (type == MessageBoxType::OK_CANCEL) {
            btn_primary.setText("OK");
            btn_primary.setPosition({bx + bw / 2 - 50, btn_y}); // 少しボタン同士の間隔を調整
            btn_primary.setVisible(true);
            btn_primary.setEnabled(true);

            btn_secondary.setText("Cancel");
            btn_secondary.setPosition({bx + bw / 2 + 50, btn_y});
            btn_secondary.setVisible(true);
            btn_secondary.setEnabled(true);
        }
        else if (type == MessageBoxType::YES_NO) {
            btn_primary.setText("Yes");
            btn_primary.setPosition({bx + bw / 2 - 50, btn_y}); // 少しボタン同士の間隔を調整
            btn_primary.setVisible(true);
            btn_primary.setEnabled(true);

            btn_secondary.setText("No");
            btn_secondary.setPosition({bx + bw / 2 + 50, btn_y});
            btn_secondary.setVisible(true);
            btn_secondary.setEnabled(true);
        }

        // 表示化
        setVisible(true);
        setEnabled(true);

        // モーダルフォーカスの獲得
        if (parent_page) {
            parent_page->setFocusedElement(this);
            parent_page->load(); // 即座に再描画と画面転送を実行
        }
    }

    void MessageBox::hide() {
        setVisible(false);
        setEnabled(false);

        // モーダルフォーカスの解除
        if (parent_page && parent_page->getFocusedElement() == this) {
            parent_page->setFocusedElement(nullptr);
            parent_page->load(); // 即座に背景を含めた全画面の再描画を実行
        }
    }

    void MessageBox::update_impl(bool force_flush) {
        if (!visible || !enabled) return;

        // まず、Panelとしての通常の子要素更新処理を実行
        Panel::update_impl(force_flush);

        // ボタンのタップイベントを監視
        if (btn_primary.wasReleased()) {
            MessageBoxResult res = (box_type == MessageBoxType::YES_NO) ? MessageBoxResult::YES : MessageBoxResult::OK;
            if (result_callback) {
                result_callback(res);
            }
            hide();
        }
        else if (btn_secondary.wasReleased() && box_type != MessageBoxType::OK) {
            MessageBoxResult res = (box_type == MessageBoxType::YES_NO) ? MessageBoxResult::NO : MessageBoxResult::CANCEL;
            if (result_callback) {
                result_callback(res);
            }
            hide();
        }
    }
}
