#ifndef UILIB_UTILS_H
#define UILIB_UTILS_H
#include <cmath>
#include <Arduino.h>
#include "color.h"

#ifdef UILIB_DEBUG
#define UILIB_DEBUG_PRINT(x) Serial.print(x)
#define UILIB_DEBUG_PRINTLN(x) Serial.println(x)
#define UILIB_DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define UILIB_DEBUG_PRINT(x)
#define UILIB_DEBUG_PRINTLN(x)
#define UILIB_DEBUG_PRINTF(...)
#endif

/**
 * @file utils.h
 * @brief 各種アライメントフラグ、2次元ベクトル・座標用構造体などのユーティリティを定義するヘッダーファイル
 */

namespace uilib{
	/**
	 * @namespace align
	 * @brief テキストや要素の整列（アライメント）用定数を定義する名前空間
	 */
	namespace align{
		/** @brief アライメント指定用の型 */
		using align_t = uint8_t;
		
		constexpr align_t left =	0b00000001;  ///< 左揃え
		constexpr align_t center =	0b00000010;  ///< 左右中央揃え
		constexpr align_t right =	0b00000100;  ///< 右揃え
		constexpr align_t top = 	0b00001000;  ///< 上揃え
		constexpr align_t middle =	0b00010000;  ///< 上下中央揃え
		constexpr align_t bottom =	0b00100000;  ///< 下揃え

		constexpr align_t top_left = left | top;           ///< 左上揃え
		constexpr align_t top_center = center | top;       ///< 上中央揃え
		constexpr align_t top_right = right | top;         ///< 右上揃え
		constexpr align_t middle_left = left | middle;     ///< 左中央揃え
		constexpr align_t middle_center = center | middle; ///< 中央揃え
		constexpr align_t middle_right = right | middle;   ///< 右中央揃え
		constexpr align_t bottom_left = left | bottom;     ///< 左下揃え
		constexpr align_t bottom_center = center | bottom; ///< 下中央揃え
		constexpr align_t bottom_right = right | bottom;   ///< 右下揃え
	}

	/**
	 * @brief 描画用カラー値の型定義
	 */
	using color_t = int;

	/**
	 * @brief タッチ判定のチャタリング防止（デバウンス）時間（ミリ秒）
	 */
	constexpr uint32_t DEBOUNCE_TIME_MS = 5;

	/**
	 * @struct vector2
	 * @brief 2次元ベクトルおよび座標系を表現するための汎用テンプレート構造体
	 * @tparam T 数値型 (int32_t, float など)
	 */
	template<typename T>
	struct vector2{
		T x; ///< X成分
		T y; ///< Y成分

		/**
		 * @brief ゼロ初期化するデフォルトコンストラクタ
		 */
		vector2() : x(0), y(0) {}

		/**
		 * @brief 指定した値で初期化するコンストラクタ
		 * @param x X成分の初期値
		 * @param y Y成分の初期値
		 */
		vector2(T x, T y) : x(x), y(y) {}

		// --- 算術演算子オーバーロード ---

		vector2 operator+(const vector2& other) const {
			return vector2(x + other.x, y + other.y);
		}
		vector2 operator+(const T scalar) const {
			return vector2(x + scalar, y + scalar);
		}
		vector2& operator+=(const vector2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		vector2 operator+=(T scalar) {
			x += scalar;
			y += scalar;
			return *this;
		}
		vector2 operator-(const vector2& other) const {
			return vector2(x - other.x, y - other.y);
		}
		vector2 operator-(const T scalar) const {
			return vector2(x - scalar, y - scalar);
		}
		vector2& operator-=(const vector2& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}
		vector2 operator-=(T scalar) {
			x -= scalar;
			y -= scalar;
			return *this;
		}
		vector2 operator*(const vector2& other) const {
			return vector2(x * other.x, y * other.y);
		}
		vector2 operator*(T scalar) const {
			return vector2(x * scalar, y * scalar);
		}
		vector2& operator*=(const vector2& other) {
			x *= other.x;
			y *= other.y;
			return *this;
		}
		vector2 operator*=(T scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		vector2 operator/(T scalar) const {
			return vector2(x / scalar, y / scalar);
		}
		vector2 operator/(const vector2& other) const {
			return vector2(x / other.x, y / other.y);
		}
		vector2 operator/=(T scalar) {
			x /= scalar;
			y /= scalar;
			return *this;
		}
		vector2 operator/=(const vector2& other) {
			x /= other.x;
			y /= other.y;
			return *this;
		}
		bool operator==(const vector2& other) const {
			return x == other.x && y == other.y;
		}
		bool operator!=(const vector2& other) const {
			return !(*this == other);
		}

		/**
		 * @brief ベクトルの長さ（ノルム）を計算します
		 * @return ベクトルの長さ（float）
		 */
		float length() const {
			return sqrt(x * x + y * y);
		}

		/**
		 * @brief ベクトルの正規化（長さを1.0にする）を行い、新しいベクトルを返します
		 * @return 正規化されたベクトル
		 */
		vector2 normalized() const {
			auto len = length();
			if (len == 0) return vector2(0, 0);
			return vector2(x / len, y / len);
		}
		
		/**
		 * @brief 座標の文字列表現を取得します (デバッグ用)
		 * @return "[x, y]" 形式の文字列
		 */
		String to_string() const {
			return String("[") + String(x) + String(", ") + String(y) + String("]");
		}
	};

	/**
	 * @brief 2次元の整数座標/サイズを示す型定義
	 */
	using point = vector2<int32_t>;
}
#endif // UILIB_UTILS_H