# M5UILib

M5Stack Core2 向けに設計された、LovyanGFX（M5GFX）ベースのインタラクティブな軽量UIライブラリです。  
ダブルバッファリング（M5Canvas）によるチラツキのない滑らかな描画、自動整列（VBox/HBox/Grid）、モーダルダイアログ、スクロールパネルなどの高度なUIレイアウト、およびページ管理機能を提供します。

---

## 主な特徴

- **ダブルバッファリング**: M5Canvasを用いたバックバッファ描画により、表示更新時のチラツキがありません。
- **堅牢なタッチ検出**: チャタリング防止（デバウンス）に加え、ドラッグ時のスライドイン（他要素からのタッチ横取り）を防止する仕組みを搭載。
- **レイアウトグループ**: VBox, HBox, Grid により、要素のサイズや余白に合わせた自動整列・配置が簡単に行えます。
- **モーダルフォーカス**: MessageBox 等の表示中に背景要素の入力を遮断するモーダルロック機能。
- **ページ管理（PageManager）**: 複数の画面（Page）の登録、および状態更新・遷移をシンプルに管理。

---

## 機能一覧

| 機能 | 概要 | 対応状況 |
| :--- | :--- | :---: |
| **Label** | 複数行表示（`\n`）およびアライメント（左寄せ/中央/右寄せ）対応のテキスト | ◯ |
| **DButton** | 角丸半径やマージンを自由に設定可能なボタン（有効/無効化対応） | ◯ |
| **ToggleSwitch** | スライド型ON/OFFトグルスイッチ | ◯ |
| **Slider** | 値のステップ丸め（step）に対応したスライダー | ◯ |
| **CheckBox** | チェック状態（ON/OFF）を切り替えるボックス | ◯ |
| **RadioButton** | RadioButtonGroup 内で排他選択を行うラジオボタン | ◯ |
| **DropDown** | スクロール表示・フォーカス制御に対応したドロップダウンリスト | ◯ |
| **Shape** | 矩形・円形などの各種基本図形およびカスタム描画対応の図形要素 | ◯ |
| **Panel** | 背景塗りつぶし・枠線描画が可能なレイアウト用パネル | ◯ |
| **LayoutGroup** | VBox（垂直整列）、HBox（水平整列）による自動配置コンテナ | ◯ |
| **Grid** | 指定した行・列数にUI要素を等間隔で配置するグリッドコンテナ | ◯ |
| **ScrollPanel** | 自動フェードスクロールバー付きの縦横スワイプスクロールパネル | ◯ |
| **TabPanel** | 複数の子コンテナをタブで切り替えるタブ管理パネル | ◯ |
| **MessageBox** | 自動改行メッセージとOK/CANCELボタンを備えたモーダルダイアログ | ◯ |
| **Image** | 画像ファイル（BMP, PNGなど）の描画表示 | × |
| **ProgressBar / Gauge** | 進捗バーや円形メーターによる数値の可視化 | × |
| **Chart / Graph** | センサーデータ等の時系列変化を示す折れ線/棒グラフ | × |
| **Keyboard** | 数値や文字列を入力するためのオンスクリーンキーボード | 〇 |
| **ListView** | テキストやアイコンを一覧表示するスクロール対応リスト | × |
| **Theme System** | アプリ全体のカラーやフォントを一括管理・切り替えるテーマ機能 | × |
| **Gesture Detection** | 長押しやダブルタップなどのジェスチャー検出 | × |
| **Transition Animation** | ページ遷移時のスライドやフェードイン効果 | × |
| **Battery Icon** | バッテリー残量と充電状態を示すアイコン | × |

---

## 簡単な使い方（クイックスタート）

以下は、`M5UILib` を使用して画面（Page）を構築し、ラベルとボタンを配置する最もシンプルなコード例です。

### 1. ページの定義とUI要素の配置

`uilib::Page` クラスを継承し、`init()` および `loop()` をオーバーライドして画面を構成します。

```cpp
#include <M5Unified.h>
#include <M5GFX.h>
#include <M5UILib.h>

// PageManager の名前空間エイリアスを設定
namespace PageManager = uilib::PageManager;

// ホーム画面クラスの定義
class PageHome : public uilib::Page {
private:
    uilib::Label label_title;
    uilib::DButton btn1;

public:
    void init() override {
        // 1. ページの初期化（画面サイズ、背景色、描画先のLovyanGFX）
        uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd);

        // 2. 各UI要素の初期化
        // タイトルラベル (中央揃え)
        label_title = uilib::Label("Hello M5UILib", {160, 30}, WHITE, BLACK, 1.0f, this, &fonts::lgfxJapanGothic_20);
        label_title.setAlign(uilib::Align::CENTER);

        // デジタルボタン (DButton)
        btn1 = uilib::DButton("Click Me", {100, 100}, WHITE, BLUE, 1.0f, this, &fonts::efontJA_14, 20, 4);

        // 3. UI要素をページに追加登録
        addElement(&label_title);
        addElement(&btn1);

        // 4. ページを PageManager に登録（ID: 0）
        PageManager::add(this, 0);
    }

    void loop() override {
        // ボタンがタップされて離された瞬間のイベントを処理
        if (btn1.wasReleased()) {
            Serial.println("Button was released!");
            // 必要に応じて PageManager::set(遷移先ID) で画面遷移が可能
        }
    }
};

// ページのインスタンスを生成
PageHome page_home;
```

### 2. メインの setup() と loop() の実装

`setup()` で各ページの初期化を行い、`loop()` の中で定期的にタッチ状態（`M5.update()`）とページ状態（`PageManager::update()`）を更新します。

```cpp
void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(115200);
    delay(1000);

    // ページの初期化処理
    page_home.init();

    // 初期表示するページ（ID: 0）を設定
    PageManager::set(0);
    Serial.println("Initialization Completed.");
}

void loop() {
    // M5Stackの内部状態（タッチなど）を更新
    M5.update();

    // 物理ボタン（BtnB）でホーム画面（ID: 0）に戻る処理
    if (M5.BtnB.wasReleased()) {
        PageManager::set(0);
    }

    // 現在アクティブなページの更新処理を呼び出す
    PageManager::update();
}
```
