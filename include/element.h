#ifndef UILIB_ELEMENT_H
#define UILIB_ELEMENT_H

#include "utils.h"
#include <M5GFX.h>

/**
 * @file element.h
 * @brief UI要素の基底クラスを定義するヘッダーファイル
 */

namespace uilib {
	/**
	 * @brief フォントデータの型定義
	 */
	using font_t = const lgfx::v1::IFont*;
	
	/**
	 * @brief 水平配置のアライメント型定義
	 */
	enum class Align {
		LEFT,
		CENTER,
		RIGHT
	};
	
	class Page;

	/**
	 * @class Element
	 * @brief すべてのUI要素（Label, DButton等）の基底となる抽象クラス
	 */
	class Element {
	protected:
		/** @brief 所属する親ページへのポインタ */
		Page* parent_page;

		/** @brief 親要素へのポインタ（コンテナ内のネスト管理用） */
		Element* parent_element = nullptr;

		/** @brief 要素の描画基準位置（左上座標） */
		point position;

		/** @brief 配置アライメント */
		Align align = Align::LEFT;

		/** @brief 要素の表示・非表示フラグ */
		bool visible = true;

		/** @brief 操作の有効・無効化フラグ */
		bool enabled = true;

		/** @brief 再描画（画面更新）が必要であることを示すフラグ */
		bool flush_required = true;

		/**
		 * @brief 指定された幅に対して、アライメントに応じたX軸オフセットを計算します
		 */
		int32_t getAlignXOffset(int32_t w) const {
			switch (align) {
				case Align::CENTER:
					return -w / 2;
				case Align::RIGHT:
					return -w;
				case Align::LEFT:
				default:
					return 0;
			}
		}

		/**
		 * @brief 子クラス固有の更新ロジックを実装する仮想関数
		 */
		virtual void update_impl(bool force_flush = false) {}
	public:
		/**
		 * @brief 親ページを設定します
		 */
		void setParentPage(Page* page) { parent_page = page; }

		/**
		 * @brief 親要素を設定します
		 */
		void setParentElement(Element* parent) { parent_element = parent; }

		/**
		 * @brief 親要素を取得します
		 */
		Element* getParentElement() const { return parent_element; }

		/**
		 * @brief 要素をキャンバスに描画する純粋仮想関数
		 * 
		 * 派生クラスで実装し、親ページのキャンバス（parent_page->getCanvas()）に対して描画を行います。
		 */
		virtual void draw() = 0;

		/**
		 * @brief 要素の描画領域（境界）を取得する純粋仮想関数
		 * @param bx 描画領域の左端X座標（戻り値）
		 * @param by 描画領域の上端Y座標（戻り値）
		 * @param bw 描画領域の幅（戻り値）
		 * @param bh 描画領域の高さ（戻り値）
		 */
		virtual void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) = 0;

		/**
		 * @brief 要素の位置を相対的に移動します
		 * @param diff 移動する差分ベクトル（座標値）
		 */
		virtual void movePosition(const point& diff){
			position += diff;
			flush_required = true;
		}
		
		/**
		 * @brief 要素の位置を絶対座標で設定します
		 * @param new_position 新しい位置座標
		 */
		virtual void setPosition(const point& new_position){
			position = new_position;
			flush_required = true;
		}

		/**
		 * @brief 現在の位置座標を取得します
		 * @return 現在の位置（point）
		 */
		virtual point getPosition() const{
			return position;
		}

		/**
		 * @brief 表示状態を設定します
		 * @param is_visible trueで表示、falseで非表示
		 */
		virtual void setVisible(bool is_visible){
			visible = is_visible;
			flush_required = true;
		}

		/**
		 * @brief 現在の表示状態を取得します
		 * @return trueなら表示、falseなら非表示
		 */
		virtual bool isVisible() const{
			return visible;
		}

		/**
		 * @brief 要素の有効・無効状態を設定します
		 * @param is_enabled trueで有効、falseで無効
		 */
		virtual void setEnabled(bool is_enabled){
			if (enabled != is_enabled) {
				enabled = is_enabled;
				flush_required = true;
			}
		}

		/**
		 * @brief 要素が有効（操作可能）かどうかを取得します
		 * @return 有効ならtrue、無効ならfalse
		 */
		virtual bool isEnabled() const{
			return enabled;
		}

		/**
		 * @brief 水平配置アライメントを設定します。変更時は再描画要求が立ちます。
		 * @param new_align 設定するアライメント
		 */
		virtual void setAlign(Align new_align) {
			if (align != new_align) {
				align = new_align;
				flush_required = true;
			}
		}

		/**
		 * @brief 現在の配置アライメントを取得します
		 * @return 配置アライメント
		 */
		virtual Align getAlign() const {
			return align;
		}

		/**
		 * @brief タッチ状態を強制キャンセルします（スクロール等のフォーカス遷移用、空実装）
		 */
		virtual void cancelPress() {}

		/**
		 * @brief 要素の状態を更新し、描画更新が必要かどうかを判定します
		 * @param force_flush trueを指定すると強制的に描画更新フラグを立てます
		 * @return 描画更新（フラッシュ）が必要な場合はtrue、不要な場合はfalse
		 */
		virtual bool update(bool force_flush = false){
			bool flush = flush_required || force_flush;
			flush_required = false;

			update_impl(force_flush);

			return flush;
		}
	};
}

#endif // UILIB_ELEMENT_H