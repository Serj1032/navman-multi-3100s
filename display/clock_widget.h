#pragma once

#include "drawable.h"
#include "color_scheme.h"

class ClockWidget : public Drawable {
public:
    ClockWidget() : ClockWidget(0, 0) {}
    ClockWidget(uint16_t x, uint16_t y) : Drawable(x, y) {
        set_color(ColorScheme::get_instance().text_color());
        current_time_ = "--:--:--";
        new_time_ = "--:--:--";
    }
    ~ClockWidget() = default;

    void clear_content(Display &display) override {
        display.draw_text(x_, y_, current_time_.c_str(), 2, ColorScheme::get_instance().background_color());
    }

    void draw_content(Display &display) override {
        display.draw_text(x_, y_, current_time_.c_str(), 2, ColorScheme::get_instance().background_color());
        current_time_ = new_time_;
        display.draw_text(x_, y_, current_time_.c_str(), 2, ColorScheme::get_instance().text_color());
    }

    void set_time(const String &time) {
        if (current_time_ == time) {
            return;
        }
        new_time_ = time;
        mark_dirty();
    }

private:
    String current_time_;
    String new_time_;
};