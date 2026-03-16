#include "display/icons/boat_icon.h"
#include "display/color_scheme.h"
#include "utils.h"

namespace {
    const int16_t kBoatX[] = {-20, -25, -25, -15, -10,   0};
    const int16_t kBoatY[] = { 80,   5, -10, -50, -60, -80};
    const int kBoatPointCount = sizeof(kBoatX) / sizeof(kBoatX[0]);
}

BoatIcon::BoatIcon(uint16_t x, uint16_t y, float resolution)
    : Drawable(x, y)
    , resolution_(resolution)
    , angle_(0.0f)
    , new_angle_(0.0f)
{
    set_color(ColorScheme::get_instance().default_icon_color());
}

void BoatIcon::clear_content(Display &display) {
    draw_boat(display, ColorScheme::get_instance().background_color());
}

void BoatIcon::draw_content(Display &display) {
    if (is_dirty()) {
        clear_content(display);
        angle_ = new_angle_;
    }
    if (is_visible()) {
        draw_boat(display, ColorScheme::get_instance().default_icon_color());
    }
    is_dirty_ = false;
}

void BoatIcon::set_angle(float angle) {
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

void BoatIcon::draw_boat(Display &display, uint16_t color) {
    int16_t xl = x_ + kBoatX[0];
    int16_t yl = y_ + kBoatY[0];
    int16_t xr = x_ - kBoatX[0];
    int16_t yr = y_ + kBoatY[0];
    rotate_by_angle(x_, y_, xl, yl, angle_);
    rotate_by_angle(x_, y_, xr, yr, angle_);
    display.draw_line(xl, yl, xr, yr, color);

    for (int i = 0; i < kBoatPointCount - 1; ++i) {
        int16_t _xl = x_ + kBoatX[i + 1];
        int16_t _yl = y_ + kBoatY[i + 1];
        int16_t _xr = x_ - kBoatX[i + 1];
        int16_t _yr = y_ + kBoatY[i + 1];

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
