#ifndef UILIB_MESSAGE_BOX_H
#define UILIB_MESSAGE_BOX_H

#include "panel.h"
#include "label.h"
#include "dbutton.h"
#include <functional>

namespace uilib {
    /**
     * @enum MessageBoxType
     * @brief メッセージボックスのボタン配置タイプ
     */
    enum class MessageBoxType {
        OK,             ///< 「OK」ボタンのみ
        OK_CANCEL,      ///< 「OK」と「キャンセル」ボタン
        YES_NO          ///< 「はい」と「いいえ」ボタン
    };

    /**
     * @enum MessageBoxResult
     * @brief メッセージボックスで選択された結果
     */
    enum class MessageBoxResult {
        OK,
        CANCEL,
        YES,
        NO
    };

    /**
     * @class MessageBox
     * @brief モーダルダイアログ（ポップアップメッセージボックス）を表現するクラス
     */
    class MessageBox : public Panel {
    private:
        Label label_title;
        Label label_message;
        DButton btn_primary;    // OK or Yes
        DButton btn_secondary;  // Cancel or No

        MessageBoxType box_type = MessageBoxType::OK;
        std::function<void(MessageBoxResult)> result_callback = nullptr;

        void setupLayout();

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        MessageBox() : Panel() {}

        /**
         * @brief メッセージボックスを構築します
         */
        MessageBox(const point& pos, int32_t w, int32_t h, color_t bg_col = 0x2104, Page* parent_page = nullptr);

        /**
         * @brief メッセージボックスを表示します
         * @param title ダイアログのタイトル
         * @param message 表示するメッセージ本文
         * @param type ボタンの配置タイプ
         * @param cb ボタン押下時のコールバック関数
         */
        void show(const String& title, const String& message, MessageBoxType type, std::function<void(MessageBoxResult)> cb);

        /**
         * @brief メッセージボックスを非表示にします（フォーカス解除）
         */
        void hide();

        /**
         * @brief ダイアログ内部のボタン更新チェックなどを行います
         */
        void update_impl(bool force_flush = false) override;
    };
}

#endif // UILIB_MESSAGE_BOX_H
