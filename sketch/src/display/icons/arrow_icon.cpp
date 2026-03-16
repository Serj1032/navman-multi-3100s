#include "display/icons/arrow_icon.h"
#include "display/color_scheme.h"
#include "utils.h"

ArrowIcon::ArrowIcon(uint16_t x, uint16_t y, uint16_t length, float resolution)
    : Drawable(x, y)
    , resolution_(resolution)
    , length_(length)
    , new_length_(length)
    , angle_(0.0f)
    , new_angle_(0.0f)
{
    set_color(ColorScheme::get_instance().default_icon_color());
}

void ArrowIcon::clear_content(Display &display) {
    draw_arrow(display, ColorScheme::get_instance().background_color());
}

void ArrowIcon::draw_content(Display &display) {
    if (is_dirty()) {
        clear_content(display);
        angle_ = new_angle_;
        length_ = new_length_;
    }
    if (is_visible()) {
        draw_arrow(display, ColorScheme::get_instance().default_icon_color());
    }
    is_dirty_ = false;
}

void ArrowIcon::set_angle(float angle) {
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

void ArrowIcon::set_length(uint16_t length) {
    if (length == new_length_) {
        return;
    }
    new_length_ = length;
    mark_dirty();
}

void ArrowIcon::draw_arrow(Display &display, uint16_t color) {
    int16_t x_start = x_;
    int16_t y_start = y_;
    int16_t x_end = x_;
    int16_t y_end = y_ - length_;

    // Draw arrow shaft (3 lines for thickness)
    for (int i = -1; i <= 1; i++) {
        int16_t x0 = x_start + i;
        int16_t y0 = y_start;
        int16_t x1 = x_end + i;
        int16_t y1 = y_end;
        rotate_by_angle(x_, y_, x0, y0, angle_);
        rotate_by_angle(x_, y_, x1, y1, angle_);
        display.draw_line(x0, y0, x1, y1, color);
    }

    // Draw arrowhead
    int16_t x0 = x_end;
    int16_t y0 = y_end - 10;
    int16_t x1 = x_end - 5;
    int16_t y1 = y_end + 5;
    int16_t x2 = x_end + 5;
    int16_t y2 = y_end + 5;
    rotate_by_angle(x_, y_, x0, y0, angle_);
    rotate_by_angle(x_, y_, x1, y1, angle_);
    rotate_by_angle(x_, y_, x2, y2, angle_);
    display.draw_fill_triangle(x0, y0, x1, y1, x2, y2, color);
}
