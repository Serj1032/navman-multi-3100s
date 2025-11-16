#pragma once

#include "Arduino.h"
// https://arduinogetstarted.com/tutorials/arduino-tft-lcd-touch-screen-display

#include <DIYables_TFT_Touch_Shield.h>
#include "color_scheme.h"

#define WIDTH 480
#define HEIGHT 320

class Display {
public:
  ~Display() = default;

  static Display& get_instance() {
    static Display instance;
    return instance;
  }

  void init() {
    TFT_display.begin();

    TFT_display.setRotation(3);
    TFT_display.fillScreen(color_scheme_.background_color());
  }

  void draw_text(uint16_t x, uint16_t y, const char* text, uint16_t font_size, uint16_t color) {
    TFT_display.setTextColor(color);
    TFT_display.setTextSize(font_size);
    TFT_display.setCursor(x, y);
    TFT_display.print(text);
  }

  void draw_bitmap(uint16_t x, uint16_t y, const uint8_t* bitmap, uint16_t w, uint16_t h, uint16_t color, uint16_t bg_color) {
    TFT_display.drawBitmap(x, y, bitmap, w, h, color, bg_color);
  }

  void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
    TFT_display.fillRect(x, y, width, height, color);
  }

  void draw_round_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t radius, uint16_t color) {
    TFT_display.fillRoundRect(x, y, width, height, radius, color);
  }

  void clear() {
    TFT_display.fillScreen(color_scheme_.background_color());
  }

private:
  Display() : color_scheme_(ColorScheme::get_instance()) {}

private:
  DIYables_TFT_ILI9488_Shield TFT_display;
  ColorScheme& color_scheme_;
};