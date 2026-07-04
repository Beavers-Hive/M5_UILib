#ifndef UILIB_ELEMENTS_KEYBOARD_H
#define UILIB_ELEMENTS_KEYBOARD_H

#include "dbutton.h"
#include "label.h"
#include "panel.h"
#include <functional>

namespace uilib {

/**
 * @enum KeyboardMode
 * @brief キーボードの表示モード
 */
enum class KeyboardMode {
  LOWER,  ///< 英小文字モード
  UPPER,  ///< 英大文字モード
  SYMBOL, ///< 記号・数字モード1
  SYMBOL2 ///< 記号・数字モード2
};

/**
 * @class KeyButton
 * @brief 固定サイズとテキスト中央揃えをサポートするキーボード専用ボタンクラス
 */
class KeyButton : public DButton {
private:
  int32_t fixed_width = 0;
  int32_t fixed_height = 0;

public:
  KeyButton();
  void setFixedSize(int32_t w, int32_t h);
  void getBounds(int32_t &bx, int32_t &by, int32_t &bw, int32_t &bh) override;
  void draw() override;
};

/**
 * @class Keyboard
 * @brief 画面下部で操作可能なソフトウェアキーボードクラス
 */
class Keyboard : public Panel {
private:
  KeyButton keys[31];                      ///< 31個のキーボタン
  KeyboardMode mode = KeyboardMode::LOWER; ///< 現在の表示モード
  String text_buffer = "";                 ///< 入力中の文字列バッファ

  Label *target_label = nullptr; ///< 入力先のラベル要素 (任意)
  InteractiveTextElement *target_interactive_text =
      nullptr; ///< 入力先のテキスト要素 (任意)

  std::function<void(const String &)> on_change_callback =
      nullptr; ///< テキスト変更時コールバック
  std::function<void(const String &)> on_submit_callback =
      nullptr; ///< Enter決定時コールバック

  /**
   * @brief 各キーのラベル文字列を現在のモードに合わせて更新します
   */
  void updateKeyLabels();

  /**
   * @brief キーが押されたときの処理を行います
   * @param index 押されたキー of index (0~30)
   */
  void handleKeyPress(int index);

  /**
   * @brief UTF-8の文字境界を考慮して、末尾の1文字を削除します
   */
  void removeLastChar(String &s);

public:
  /**
   * @brief デフォルトコンストラクタ
   */
  Keyboard() : Panel() {}

  /**
   * @brief キーボードを構築します (標準サイズは 320x120)
   * @param pos 配置座標 (通常は {0, 120})
   * @param parent_page 所属する親ページ
   */
  Keyboard(const point &pos, Page *parent_page = nullptr);

  /**
   * @brief キーボードと全キーの状態更新処理
   */
  void update_impl(bool force_flush = false) override;

  /**
   * @brief 入力同期先となるLabel要素を設定します
   */
  void setTargetLabel(Label *label) { target_label = label; }

  /**
   * @brief 入力同期先となるInteractiveTextElement要素を設定します
   */
  void setTargetInteractiveText(InteractiveTextElement *elem) {
    target_interactive_text = elem;
  }

  /**
   * @brief テキスト変更時のコールバック関数を設定します
   */
  void setOnChangeCallback(std::function<void(const String &)> cb) {
    on_change_callback = cb;
  }

  /**
   * @brief 決定(Enter)時のコールバック関数を設定します
   */
  void setOnSubmitCallback(std::function<void(const String &)> cb) {
    on_submit_callback = cb;
  }

  /**
   * @brief 入力中のテキスト文字列を取得します
   */
  String getText() const { return text_buffer; }

  /**
   * @brief 外部からテキスト文字列を設定します
   */
  void setText(const String &text);

  /**
   * @brief 入力中のテキストバッファをクリアします
   */
  void clear();

  /**
   * @brief キーボードの表示モードを取得します
   */
  KeyboardMode getMode() const { return mode; }

  /**
   * @brief キーボードの表示モードを設定します
   */
  void setMode(KeyboardMode new_mode);
};
} // namespace uilib

#endif // UILIB_ELEMENTS_KEYBOARD_H
