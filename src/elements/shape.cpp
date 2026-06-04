#include <M5Unified.h>
#include <M5GFX.h>
#include "elements/shape.h"
#include "page.h"

namespace uilib {
	Shape::Shape(ShapeType type, const point& pos, int32_t w, int32_t h, Page* parent_page)
		: Element(), shape_type(type), width(w), height(h) {
		this->position = pos;
		this->parent_page = parent_page;
		// デフォルトの基準点（ピボット）を図形の中心に設定
		this->origin = { w / 2.0f, h / 2.0f };
	}

	void Shape::getBounds(int32_t& bx, int32_t& by, int32_t& bw, int32_t& bh) {
		bw = width;
		bh = height;
		bx = position.x + getAlignXOffset(bw);
		by = position.y;
	}

	void Shape::draw() {
		if (!parent_page || !visible) return;
		auto canvas = parent_page->getCanvas();
		if (!canvas) return;

		// 一時スプライトの生成
		M5Canvas sprite(canvas);
		if (!sprite.createSprite(width, height)) {
			UILIB_DEBUG_PRINTLN("Error: Failed to create sprite for Shape element.");
			return;
		}

		// 透過色でクリア
		sprite.fillScreen(transparent_color);

		// 1. グラデーション背景の描画
		if (gradient_type != GradientType::None) {
			if (gradient_type == GradientType::LinearVertical) {
				for (int32_t y = 0; y < height; ++y) {
					float t = static_cast<float>(y) / (height > 1 ? (height - 1) : 1);
					Color col = lerpColor(grad_start_color, grad_end_color, t);
					sprite.drawFastHLine(0, y, width, col);
				}
			} else if (gradient_type == GradientType::LinearHorizontal) {
				for (int32_t x = 0; x < width; ++x) {
					float t = static_cast<float>(x) / (width > 1 ? (width - 1) : 1);
					Color col = lerpColor(grad_start_color, grad_end_color, t);
					sprite.drawFastVLine(x, 0, height, col);
				}
			}
		} else {
			// 単色塗りつぶし (Rectangle のみ全体塗りつぶし)
			if (shape_type == ShapeType::Rectangle) {
				if (stroke_width == 0) {
					sprite.fillScreen(color);
				} else {
					sprite.fillScreen(bg_color);
				}
			}
		}

		// 2. 図形に応じたクリッピング・透過処理および枠線描画
		if (shape_type == ShapeType::Rectangle) {
			// 矩形の場合、グラデーションありの時は全体がすでにグラデーションで満たされている。
			// グラデーションなしで塗りつぶし無しの場合は、背景色で塗りつぶされている。
			if (stroke_width > 0) {
				// 枠線の描画
				for (int32_t i = 0; i < stroke_width; ++i) {
					sprite.drawRect(i, i, width - 2 * i, height - 2 * i, color);
				}
			}
		} else if (shape_type == ShapeType::Circle) {
			float cx = width / 2.0f;
			float cy = height / 2.0f;
			float r = (width < height ? width : height) / 2.0f;

			// 円の外側を透過色で塗りつぶす（グラデーションや塗りつぶし色のクリッピング）
			for (int32_t y = 0; y < height; ++y) {
				for (int32_t x = 0; x < width; ++x) {
					float dx = x - cx;
					float dy = y - cy;
					if (dx * dx + dy * dy > r * r) {
						sprite.writePixel(x, y, transparent_color);
					}
				}
			}

			// 単色で塗りつぶす場合（グラデーションなし）
			if (gradient_type == GradientType::None) {
				if (stroke_width == 0) {
					sprite.fillCircle(cx, cy, r, color);
				} else {
					sprite.fillCircle(cx, cy, r, bg_color);
				}
			}

			// 枠線の描画
			if (stroke_width > 0) {
				for (int32_t i = 0; i < stroke_width; ++i) {
					sprite.drawCircle(cx, cy, r - i, color);
				}
			}
		} else if (shape_type == ShapeType::Triangle) {
			// デフォルト頂点: 上(w/2, 0), 左下(0, h), 右下(w, h)
			float x0 = width / 2.0f;
			float y0 = 0.0f;
			float x1 = 0.0f;
			float y1 = static_cast<float>(height);
			float x2 = static_cast<float>(width);
			float y2 = static_cast<float>(height);

			// 三角形の内外判定ラムダ
			auto is_inside_triangle = [](float px, float py, float ax, float ay, float bx, float by, float cx, float cy) {
				auto sign = [](float x1, float y1, float x2, float y2, float x3, float y3) {
					return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
				};
				float d1 = sign(px, py, ax, ay, bx, by);
				float d2 = sign(px, py, bx, by, cx, cy);
				float d3 = sign(px, py, cx, cy, ax, ay);
				bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
				bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
				return !(has_neg && has_pos);
			};

			// 三角形の外側を透過色で塗りつぶす
			for (int32_t y = 0; y < height; ++y) {
				for (int32_t x = 0; x < width; ++x) {
					if (!is_inside_triangle(x, y, x0, y0, x1, y1, x2, y2)) {
						sprite.writePixel(x, y, transparent_color);
					}
				}
			}

			// 単色で塗りつぶす場合（グラデーションなし）
			if (gradient_type == GradientType::None) {
				if (stroke_width == 0) {
					sprite.fillTriangle(x0, y0, x1, y1, x2, y2, color);
				} else {
					sprite.fillTriangle(x0, y0, x1, y1, x2, y2, bg_color);
				}
			}

			// 枠線の描画
			if (stroke_width > 0) {
				// 簡易的に外枠を描画
				sprite.drawTriangle(x0, y0, x1, y1, x2, y2, color);
				if (stroke_width > 1) {
					// 内側に少し縮小した枠線を重ねて描画する（簡易的な太線対応）
					for (int32_t i = 1; i < stroke_width; ++i) {
						float offset = i * 1.5f; // 三角形の縮小オフセット調整
						sprite.drawTriangle(
							x0, y0 + offset, 
							x1 + offset, y1 - offset / 2.0f, 
							x2 - offset, y2 - offset / 2.0f, 
							color
						);
					}
				}
			}
		} else if (shape_type == ShapeType::Custom) {
			if (custom_draw_callback) {
				custom_draw_callback(&sprite, width, height);
			}
		}

		// 3. ピボット設定とアフィン変換描画
		sprite.setPivot(origin.x, origin.y);
		sprite.pushRotateZoom(
			canvas, 
			position.x + translation.x, 
			position.y + translation.y, 
			rotation, 
			scale.x, 
			scale.y, 
			transparent_color
		);

		// スプライトのリソース解放
		sprite.deleteSprite();
	}
}
