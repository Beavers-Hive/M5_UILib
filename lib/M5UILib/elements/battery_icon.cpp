#include "battery_icon.h"
#include "page.h"
#include <Arduino.h>
#include <M5Unified.h>

namespace uilib {

BatteryIcon::BatteryIcon(const point &position, bool show_pct,
                         Page *parent_page)
    : Element(), show_percentage(show_pct) {
  this->position = position;
  this->parent_page = parent_page;
  this->align = Align::RIGHT; // デフォルトはステータスバーらしく右寄せ
}

void BatteryIcon::getBounds(int32_t &bx, int32_t &by, int32_t &bw,
                            int32_t &bh) {
  if (!parent_page) {
    bx = by = bw = bh = 0;
    return;
  }

  int32_t text_w = 0;
  if (show_percentage) {
    auto canvas = parent_page->getCanvas();
    canvas->setFont(font);
    canvas->setTextSize(1.0f);
    // "100%" 相当の幅を固定として扱い、表示のガタつきを防止
    text_w = canvas->textWidth("100%") + 4;
  }

  bw = text_w + width + 2; // テキスト幅 + バッテリー枠幅 + 端子(2px)
  bh = height;

  bx = position.x + getAlignXOffset(bw);
  by = position.y;
}

void BatteryIcon::draw() {
  if (!parent_page || !visible)
    return;

  int32_t bx, by, bw, bh;
  getBounds(bx, by, bw, bh);

  auto canvas = parent_page->getCanvas();
  if (!canvas)
    return;

  int32_t text_w = 0;

  // 1. パーセンテージ数値の描画
  if (show_percentage) {
    canvas->setFont(font);
    canvas->setTextSize(1.0f);
    text_w = canvas->textWidth("100%") + 4;

    String pct_str = String(cached_level >= 0 ? cached_level : 0) + "%";
    canvas->setTextColor(text_color);
    // 右揃えになるようにテキストを描画
    int32_t tx = bx + (text_w - 4) - canvas->textWidth(pct_str);
    // テキストの縦位置をバッテリーの中央に揃える
    int32_t ty = by + (height - canvas->fontHeight()) / 2;
    canvas->drawString(pct_str, tx, ty);
  }

  // 2. バッテリー本体と枠の描画
  int32_t bat_x = bx + text_w;
  canvas->drawRect(bat_x, by, width, height, border_color);

  // 3. バッテリー右端の凸（端子）の描画
  int32_t cap_h = height / 2;
  int32_t cap_y = by + (height - cap_h) / 2;
  canvas->fillRect(bat_x + width, cap_y, 2, cap_h, border_color);

  // 4. 残量インジケータ（中身）の描画
  int32_t level = cached_level >= 0 ? cached_level : 0;
  int32_t max_fill_w = width - 4;
  int32_t fill_w = (max_fill_w * level) / 100;
  if (fill_w < 0)
    fill_w = 0;
  if (fill_w > max_fill_w)
    fill_w = max_fill_w;

  int32_t fill_h = height - 4;
  int32_t fill_x = bat_x + 2;
  int32_t fill_y = by + 2;

  color_t fill_color = 0x07E0; // デフォルト緑

  if (cached_charging) {
    fill_color = 0x07E0; // 充電中も基本は緑
  } else {
    if (level > 50) {
      fill_color = 0x07E0; // 緑
    } else if (level >= 20) {
      fill_color = 0xFBE0; // 黄/オレンジ (RGB565)
    } else {
      fill_color = 0xF800; // 赤
    }
  }

  if (fill_w > 0) {
    canvas->fillRect(fill_x, fill_y, fill_w, fill_h, fill_color);
  }

  // 5. 充電中の雷マークの描画
  if (cached_charging) {
    int32_t cx = bat_x + width / 2;
    int32_t cy = by + height / 2;
    // 雷マークを黄色(0xFFE0)で描画
    drawLightning(canvas, cx, cy, 0xFFE0);
  }
}

void BatteryIcon::drawLightning(M5Canvas *canvas, int32_t cx, int32_t cy,
                                color_t color) {
  // バッテリー内に収まるように上下2つの鋭い三角形を組み合わせて雷マークを形成
  // 上半分三角形
  canvas->fillTriangle(cx + 1, cy - 4, cx - 2, cy, cx, cy, color);
  // 下半分三角形
  canvas->fillTriangle(cx, cy, cx + 2, cy, cx - 1, cy + 4, color);
}

void BatteryIcon::update_impl(bool force_flush) {
  // 実機PMICへの頻繁なI2Cアクセスを防ぐため、5秒おきに状態取得を更新
  if (force_flush || cached_level == -1 || millis() - last_check_time >= 5000) {
    int32_t new_level = M5.Power.getBatteryLevel();
    bool new_charging = M5.Power.isCharging();

    // 異常値対策
    if (new_level < 0)
      new_level = 0;
    if (new_level > 100)
      new_level = 100;

    if (new_level != cached_level || new_charging != cached_charging) {
      cached_level = new_level;
      cached_charging = new_charging;
      flush_required = true;
    }
    last_check_time = millis();
  }
}
} // namespace uilib
