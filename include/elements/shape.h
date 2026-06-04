#ifndef UILIB_SHAPE_H
#define UILIB_SHAPE_H

#include <functional>
#include <M5Unified.h>
#include "element.h"

/**
 * @file shape.h
 * @brief アフィン変換およびグラデーション描画に対応した図形要素クラスを定義するヘッダーファイル
 */

namespace uilib {
	/**
	 * @enum ShapeType
	 * @brief 描画する基本図形の種類
	 */
	enum class ShapeType {
		Rectangle,  ///< 矩形
		Circle,     ///< 円
		Triangle,   ///< 三角形
		Custom      ///< カスタム描画コールバック
	};

	/**
	 * @enum GradientType
	 * @brief グラデーションのタイプ
	 */
	enum class GradientType {
		None,              ///< 単色（グラデーションなし）
		LinearVertical,    ///< 垂直方向の線形グラデーション
		LinearHorizontal   ///< 水平方向の線形グラデーション
	};

	/**
	 * @class Shape
	 * @brief 任意のピボット位置を基準とした回転・拡大縮小・平行移動・グラデーション描画をサポートする図形クラス
	 */
	class Shape : public Element {
	private:
		ShapeType shape_type;                              ///< 図形の種類
		int32_t width;                                     ///< ローカルスプライトの幅
		int32_t height;                                    ///< ローカルスプライトの高さ
		
		vector2<float> translation = {0.0f, 0.0f};         ///< 微調整用の追加平行移動
		float rotation = 0.0f;                             ///< 回転角度（度数法: 0 - 360）
		vector2<float> scale = {1.0f, 1.0f};               ///< 拡大縮小倍率 (x, y)
		vector2<float> origin = {0.0f, 0.0f};              ///< 変換基準点（ローカル座標系における位置）

		Color color = Color(255, 255, 255);                ///< メイン色（枠線や単色塗りつぶし用）
		Color bg_color = Color(0, 0, 0);                  ///< 背景色
		
		GradientType gradient_type = GradientType::None;   ///< グラデーション設定
		Color grad_start_color = Color(255, 255, 255);     ///< グラデーション開始色
		Color grad_end_color = Color(0, 0, 0);             ///< グラデーション終了色

		int32_t stroke_width = 0;                         ///< 枠線の太さ（0で塗りつぶし）
		Color transparent_color = Color(255, 0, 255);      ///< スプライト透過用キーカラー（デフォルトはマゼンタ）

		/** @brief カスタム描画コールバック関数 */
		std::function<void(M5Canvas*, int32_t w, int32_t h)> custom_draw_callback = nullptr;

		/**
		 * @brief 2つの色の線形補間を計算します
		 */
		Color lerpColor(Color c1, Color c2, float t) const {
			uint8_t r = c1.r() + static_cast<uint8_t>((c2.r() - c1.r()) * t);
			uint8_t g = c1.g() + static_cast<uint8_t>((c2.g() - c1.g()) * t);
			uint8_t b = c1.b() + static_cast<uint8_t>((c2.b() - c1.b()) * t);
			return Color(r, g, b);
		}

	protected:
		void update_impl(bool force_flush = false) override {}

	public:
		/**
		 * @brief デフォルトコンストラクタ
		 */
		Shape() : Element(), shape_type(ShapeType::Rectangle), width(0), height(0) {}

		/**
		 * @brief 図形要素を構築します
		 * @param type 図形の種類
		 * @param pos グローバル座標系での配置座標（親ページ上の描画位置）
		 * @param w 描画バッファの幅
		 * @param h 描画バッファの高さ
		 * @param parent_page 親ページへのポインタ
		 */
		Shape(ShapeType type, const point& pos, int32_t w, int32_t h, Page* parent_page = nullptr);

		/**
		 * @brief 親ページのキャンバスへアフィン変換を適用した図形を描画します
		 */
		void draw() override;

		/**
		 * @brief 図形の変形前の描画領域（境界）を取得します
		 */
		void getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) override;

		// --- 各種パラメータのゲッター/セッター ---

		void setShapeType(ShapeType type) { shape_type = type; flush_required = true; }
		ShapeType getShapeType() const { return shape_type; }

		void setTranslation(const vector2<float>& trans) { translation = trans; flush_required = true; }
		void setTranslation(float tx, float ty) { translation = {tx, ty}; flush_required = true; }
		vector2<float> getTranslation() const { return translation; }

		void setRotation(float angle) { rotation = angle; flush_required = true; }
		float getRotation() const { return rotation; }

		void setScale(const vector2<float>& s) { scale = s; flush_required = true; }
		void setScale(float sx, float sy) { scale = {sx, sy}; flush_required = true; }
		vector2<float> getScale() const { return scale; }

		void setOrigin(const vector2<float>& o) { origin = o; flush_required = true; }
		void setOrigin(float ox, float oy) { origin = {ox, oy}; flush_required = true; }
		vector2<float> getOrigin() const { return origin; }

		void setColor(Color col) { color = col; flush_required = true; }
		Color getColor() const { return color; }

		void setBgColor(Color col) { bg_color = col; flush_required = true; }
		Color getBgColor() const { return bg_color; }

		void setGradient(GradientType type, Color start_col, Color end_col) {
			gradient_type = type;
			grad_start_color = start_col;
			grad_end_color = end_col;
			flush_required = true;
		}
		GradientType getGradientType() const { return gradient_type; }

		void setStrokeWidth(int32_t width) { stroke_width = width; flush_required = true; }
		int32_t getStrokeWidth() const { return stroke_width; }

		void setTransparentColor(Color col) { transparent_color = col; flush_required = true; }
		Color getTransparentColor() const { return transparent_color; }

		void setCustomDrawCallback(std::function<void(M5Canvas*, int32_t w, int32_t h)> cb) {
			custom_draw_callback = cb;
			flush_required = true;
		}
	};
}

#endif // UILIB_SHAPE_H
