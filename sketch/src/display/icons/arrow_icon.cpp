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

    // Draw arrow shaft
    int16_t sx = x_start;
    int16_t sy = y_start;
    int16_t ex = x_end;
    int16_t ey = y_end;
    rotate_by_angle(x_, y_, sx, sy, angle_);
    rotate_by_angle(x_, y_, ex, ey, angle_);
    draw_thick_line(display, sx, sy, ex, ey, color);

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

void ArrowIcon::draw_thick_line(Display &display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    for (int dx = -(width_ - 1); dx < width_; ++dx) {
        for (int dy = -(width_ - 1); dy < width_; ++dy) {
            if (abs(dx) + abs(dy) < width_) {
                display.draw_line(x0 + dx, y0 + dy, x1 + dx, y1 + dy, color);
            }
        }
    }
}
