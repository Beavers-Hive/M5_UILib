#ifndef UILIB_ELEMENTS_INTERACTIVE_ELEMENT_H
#define UILIB_ELEMENTS_INTERACTIVE_ELEMENT_H

#include "../element.h"
#include <M5GFX.h>

/**
 * @file interactive_element.h
 * @brief タッチ操作およびチャタリング防止（デバウンス）を処理する中間基底クラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class InteractiveElement
     * @brief タッチ判定やデバウンス、プレス時間の管理などのインタラクティブ機能を処理する抽象中間基底クラス
     */
    class InteractiveElement : public Element {
    protected:
        bool pressed_state = false;          ///< 現在押されている（タッチされている）か
        bool released_flag = false;          ///< 直前に離されたか
        bool temp_raw_state = false;         ///< デバウンス用のテンポラリ生状態
        uint32_t last_state_change_time = 0;   ///< 最後に状態が変化した時刻
        bool ignore_touch = false;           ///< ページ遷移時のタッチ引き継ぎ無視フラグ
        bool touch_started_here = false;     ///< タッチがこの要素内で開始されたかのフラグ
        uint32_t last_release_time = 0;      ///< 最後に離された時刻 (ミリ秒)

        uint32_t press_start_time = 0;       ///< 押し始めた時間 (ミリ秒)
        uint32_t press_count = 0;            ///< 押し続けられているアップデート回数
        uint32_t last_press_duration_ms = 0; ///< 最後に押されていた時間 (ミリ秒)
        uint32_t last_press_count = 0;       ///< 最後に押されていたアップデート回数

        /**
         * @brief タッチ操作状態を更新し、デバウンス処理を行います。
         * 
         * 派生クラスの update_impl() で呼び出すことを想定しています。
         * @param force_flush trueを指定すると強制的にタッチ状態をリセットし、初期状態にします。
         */
        void update_touch_state(bool force_flush = false);

    public:
        /**
         * @brief デフォルトコンストラクタ
         */
        InteractiveElement() = default;

        /**
         * @brief 要素が現在押されているかどうかを取得します
         * @return 押されている場合はtrue、そうでない場合はfalse
         */
        bool isPressed() const;

        /**
         * @brief 要素が直前に離されたかどうかを取得します
         * @return 離された瞬間である場合はtrue、そうでない場合はfalse
         */
        bool wasReleased() const;

        /**
         * @brief 要素が押し続けられている時間（ミリ秒）を取得します。離されている場合は最後のプレス時間を返します。
         * @return プレス時間（ミリ秒）
         */
        uint32_t getPressDurationMs() const;

        /**
         * @brief 要素が押し続けられている時間（秒）を取得します。離されている場合は最後のプレス時間を返します。
         * @return プレス時間（秒）
         */
        float getPressDurationSec() const;

        /**
         * @brief 要素が押し続けられているアップデート回数（カウント）を取得します。離されている場合は最後のプレスカウントを返します。
         * @return プレスカウント
         */
        uint32_t getPressCount() const;

        /**
         * @brief タッチ状態を強制的にキャンセルします（スクロール等で親にフォーカスが移った場合に使用）
         */
        virtual void cancelPress() override;

        /**
         * @brief 有効・無効状態を設定します（無効時は操作状態を安全にクリア）
         */
        virtual void setEnabled(bool is_enabled) override;
    };
}

#endif // UILIB_ELEMENTS_INTERACTIVE_ELEMENT_H
