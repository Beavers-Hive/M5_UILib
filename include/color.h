#ifndef UILIB_COLOR_H
#define UILIB_COLOR_H

#include <stdint.h>

/**
 * @file color.h
 * @brief 色表現（RGB/HSV/RGB565 raw値）を管理するColorクラスを定義するヘッダーファイル
 */

namespace uilib {
    /**
     * @class Color
     * @brief RGB、HSV、およびRGB565の整数値を相互に変換して保持するカラー管理クラス
     */
    class Color {
    private:
        uint16_t raw_value = 0;             ///< 内部保持するRGB565 raw値 (16bit)

    public:
        /**
         * @brief デフォルトコンストラクタ (黒で初期化)
         */
        Color() = default;

        /**
         * @brief 従来の整数（RGB565 raw値）を指定するコンストラクタ (明示的)
         * @param raw RGB565形式の16bitカラー値
         */
        explicit Color(int raw) : raw_value(static_cast<uint16_t>(raw)) {}

        /**
         * @brief RGBそれぞれを指定するコンストラクタ
         * @param r 赤 (0 - 255)
         * @param g 緑 (0 - 255)
         * @param b 青 (0 - 255)
         */
        Color(uint8_t r, uint8_t g, uint8_t b) {
            raw_value = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        /**
         * @brief 従来のint (color_t) への暗黙の変換演算子
         */
        operator int() const {
            return raw_value;
        }

        /**
         * @brief 赤 (Red) の値を 0-255 の範囲で取得します
         */
        uint8_t r() const {
            return ((raw_value >> 8) & 0xF8) | ((raw_value >> 13) & 0x07);
        }

        /**
         * @brief 緑 (Green) の値を 0-255 の範囲で取得します
         */
        uint8_t g() const {
            return ((raw_value >> 3) & 0xFC) | ((raw_value >> 9) & 0x03);
        }

        /**
         * @brief 青 (Blue) の値を 0-255 の範囲で取得します
         */
        uint8_t b() const {
            return ((raw_value << 3) & 0xF8) | ((raw_value >> 2) & 0x07);
        }

        /**
         * @brief RGBからColorを生成する静的メソッド
         */
        static Color fromRGB(uint8_t r, uint8_t g, uint8_t b) {
            return Color(r, g, b);
        }

        /**
         * @brief HSVからColorを生成する静的メソッド (float指定)
         * @param h 色相 (Hue: 0.0f - 360.0f)
         * @param s 彩度 (Saturation: 0.0f - 1.0f)
         * @param v 明度 (Value: 0.0f - 1.0f)
         */
        static Color fromHSV(float h, float s, float v) {
            float r = 0, g = 0, b = 0;
            if (s <= 0.0f) {
                r = g = b = v;
            } else {
                float hh = h;
                if (hh >= 360.0f) hh = 0.0f;
                hh /= 60.0f;
                int i = (int)hh;
                float ff = hh - i;
                float p = v * (1.0f - s);
                float q = v * (1.0f - (s * ff));
                float t = v * (1.0f - (s * (1.0f - ff)));
                switch (i) {
                    case 0:  r = v; g = t; b = p; break;
                    case 1:  r = q; g = v; b = p; break;
                    case 2:  r = p; g = v; b = t; break;
                    case 3:  r = p; g = q; b = v; break;
                    case 4:  r = t; g = p; b = v; break;
                    case 5:
                    default: r = v; g = p; b = q; break;
                }
            }
            return Color(static_cast<uint8_t>(r * 255.0f),
                         static_cast<uint8_t>(g * 255.0f),
                         static_cast<uint8_t>(b * 255.0f));
        }

        /**
         * @brief HSVからColorを生成する静的メソッド (uint8_t 0-255指定)
         * @param h 色相 (0 - 255)
         * @param s 彩度 (0 - 255)
         * @param v 明度 (0 - 255)
         */
        static Color fromHSV(uint8_t h, uint8_t s, uint8_t v) {
            return fromHSV(h * 360.0f / 255.0f, s / 255.0f, v / 255.0f);
        }
    };
}

#endif /* UILIB_COLOR_H */
