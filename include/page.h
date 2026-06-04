#ifndef UILIB_PAGE_H
#define UILIB_PAGE_H

#include <vector>
#include <M5Unified.h>
#include "element.h"

/**
 * @file page.h
 * @brief UIページ（要素のコンテナ・描画キャンバス管理）を定義するヘッダーファイル
 */

namespace uilib {
	/**
	 * @class Page
	 * @brief 複数のUI要素（Element）を保持・管理し、描画画面（キャンバス）の更新を統括するクラス
	 */
	class Page {
	public:
		virtual void init() {}
		virtual void loop() {}

		/**
		 * @brief ページを初期化し、描画用キャンバス（スプライト）を作成します
		 * @param size キャンバスのサイズ（幅、高さ）
		 * @param bg_color 背景色
		 * @param gfx 描画対象となるLovyanGFXインスタンス（例: &M5.Lcd）
		 * @param color_depth カラー深度（ビット数、デフォルトは16）
		 */
		void init(const point& size, color_t bg_color, LovyanGFX* gfx, int16_t color_depth = 16);

		/**
		 * @brief ページにUI要素を追加します
		 * @param element 追加するUI要素へのポインタ（ライフサイクルは呼び出し元が管理）
		 */
		void addElement(Element* element);

		/**
		 * @brief 各要素の状態を更新し、変更があった場合は再描画と画面転送を行います
		 * 
		 * ループ内で定期的に呼び出す必要があります。
		 */
		void update();

		/**
		 * @brief ページを初回読み込みし、全要素の強制的な描画と画面転送を行います
		 */
		void load();

		/**
		 * @brief 内部のM5Canvasインスタンスへのポインタを取得します
		 * @return M5Canvasへのポインタ
		 */
		M5Canvas* getCanvas(){
			return canvas;
		}

		/**
		 * @brief キャンバスのメモリを解放し、リソースをクリーンアップします
		 */
		void finalize(){
			if(canvas){
				canvas->deleteSprite();
				delete canvas;
				canvas = nullptr;
			}
		}

		/** @brief ページにループ内で呼び出される関数を設定します */
		void setLoopFunction(std::function<void()> func);

		/** @brief ページのアクティブ状態を設定します */
		void setActive(bool isActive) { active = isActive; }

		/** @brief ページのアクティブ状態を取得します */
		bool isActive() const { return active; }

		/** @brief フォーカスされているUI要素を設定します */
		void setFocusedElement(Element* elem) { focused_element = elem; flush_required = true; }

		/** @brief 現在フォーカスされているUI要素を取得します */
		Element* getFocusedElement() const { return focused_element; }

		/**
		 * @brief ページの背景色を設定します
		 * @param color 新しい背景色
		 */
		void setBackgroundColor(color_t color);

		/**
		 * @brief 現在のページの背景色を取得します
		 * @return 現在の背景色
		 */
		color_t getBackgroundColor() const { return bg_color; }
	private:
		friend class Element;

		/** @brief ページに登録されているUI要素のリスト */
		std::vector<Element*> elements;

		/** @brief ダブルバッファリング描画用のキャンバス（スプライト） */
		M5Canvas* canvas = nullptr;

		/** @brief 現在フォーカスされているUI要素 */
		Element* focused_element = nullptr;

		/** @brief ページの背景色 */
		color_t bg_color = BLACK;

		/** @brief ページ全体の再描画要求フラグ */
		bool flush_required = true;

		/** @brief ページにループ内で呼び出される関数 */
		std::function<void()> loop_function;

		/** @brief ページが現在アクティブかどうかのフラグ */
		bool active = false;
	};
}

#endif // UILIB_PAGE_H