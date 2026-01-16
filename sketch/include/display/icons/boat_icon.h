#pragma once

#include "display/drawable.h"
#include "display/color_scheme.h"
#include "utils.h"


namespace {
    static const int16_t px[] = {-20, -25, -25, -15,  -10,   0};
    static const int16_t py[] = { 80,   5, -10, -50, -60, -80};
}


class BoatIcon : public Drawable {
public:
    BoatIcon() : BoatIcon(0, 0) {};

    BoatIcon(uint16_t x, uint16_t y, float resolution = 1.0f) : 
        Drawable(x, y),
        resolution_(resolution),
        angle_(0.0f),
        new_angle_(0.0f)
    {
        set_color(ColorScheme::get_instance().default_icon_color());
    }

    ~BoatIcon() = default;

    void clear_content(Display &display) override {
        draw_boat(display, ColorScheme::get_instance().background_color());
    }

    void draw_content(Display &display) override {
        if (is_dirty()) {
            clear_content(display);
            angle_ = new_angle_;
        }
        if (is_visible()) {
            draw_boat(display, ColorScheme::get_instance().default_icon_color());
        }
        is_dirty_ = false;
    }

    void set_angle(float angle) {
        if (abs(angle - new_angle_) < resolution_) {
            return;
        }
        if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }

        new_angle_ = angle;
        mark_dirty();
    }

private:
    void draw_boat(Display &display, uint16_t color) {
        int16_t xl = x_ + px[0];
        int16_t yl = y_ + py[0];
        int16_t xr = x_ - px[0];
        int16_t yr = y_ + py[0];
        rotate_by_angle(x_, y_, xl, yl, angle_);
        rotate_by_angle(x_, y_, xr, yr, angle_);
        display.draw_line(xl, yl, xr, yr, color);

        for (int i = 0; i < sizeof(px)/sizeof(px[0]) - 1; ++i) {
            int16_t _xl = x_ + px[i + 1];
            int16_t _yl = y_ + py[i + 1];
            int16_t _xr = x_ - px[i + 1];
            int16_t _yr = y_ + py[i + 1];

            rotate_by_angle(x_, y_, _xl, _yl, angle_);
            rotate_by_angle(x_, y_, _xr, _yr, angle_);

            display.draw_line(xl, yl, _xl, _yl, color);
            display.draw_line(xr, yr, _xr, _yr, color);

            xl = _xl;
            yl = _yl;
            xr = _xr;
            yr = _yr;
        }
    }

private:
    float resolution_;
    float angle_;
    float new_angle_;
};