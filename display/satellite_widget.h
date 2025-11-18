#pragma once

#include "drawable.h"
#include "icon.h"
#include "icons/sattelite.h"
#include "color_scheme.h"
#include "utils.h"

class SatelliteWidget : public Drawable {
public:
    SatelliteWidget() : SatelliteWidget(0, 0) {};
    SatelliteWidget(uint16_t x, uint16_t y) : 
            Drawable(x, y),
            satellite_icon_(x, y, sattelite_icon, sattelite_width, sattelite_height) 
    {
        set_color(ColorScheme::get_instance().header_color());
    };
    ~SatelliteWidget() = default;

    void clear_content(Display &display) override {
        satellite_icon_.clear_content(display);
    }

    void draw_content(Display &display) override {
        if (!initialized_) {
            satellite_icon_.set_color(ColorScheme::get_instance().default_icon_color());
            satellite_icon_.draw(display);
            draw_sattelites_amount(display, ColorScheme::get_instance().text_color());

            initialized_ = true;
            return;
        }

        if (new_satellites_amount_ != satellites_amount_) {
            draw_sattelites_amount(display, ColorScheme::get_instance().background_color());
            satellites_amount_ = new_satellites_amount_;
            satellites_amount_str_ = padStart(String(satellites_amount_), 2, ' ');
            draw_sattelites_amount(display, ColorScheme::get_instance().text_color());
        }
    }

    void set_satellites_amount(uint16_t amount) {
        if (amount == new_satellites_amount_) {
            return;
        }
        new_satellites_amount_ = amount;
        mark_dirty();
    }

private:
    void draw_sattelites_amount(Display &display, uint16_t color) {
        display.draw_text(x_ - 30, y_ + 5, satellites_amount_str_.c_str(), 2, color);
    }

private:
    bool initialized_ = false;
    String satellites_amount_str_ = String("0");
    uint16_t satellites_amount_ = 0;
    uint16_t new_satellites_amount_ = 0;
    Icon satellite_icon_;
};