#pragma once

#include "drawable.h"
#include "color_scheme.h"

namespace {
    static const float ANGLE_RESOLUTION = 3.0f; // degrees
    static const int16_t px[] = {-20, -25, -25, -15,  -10,   0};
    static const int16_t py[] = { 80,   5, -10, -50, -60, -80};
}

class HeadingWidget : public Drawable {
public:
    HeadingWidget() : Drawable() {};
    HeadingWidget(uint16_t x, uint16_t y) : Drawable(x, y) {
        set_color(ColorScheme::get_instance().header_color());
    };
    ~HeadingWidget() = default;

    void clear_content(Display &display) override {
        draw_compass_map(display, ColorScheme::get_instance().background_color());
        draw_boat(display, ColorScheme::get_instance().background_color());
    }

    void draw_content(Display &display) override {
        draw_compass_map(display, ColorScheme::get_instance().default_icon_color());

        draw_boat(display, ColorScheme::get_instance().background_color());
        draw_gps_arrow(display, ColorScheme::get_instance().background_color());

        compass_heading_ = new_compass_heading_;
        gps_heading_ = new_gps_heading_;

        draw_boat(display, ColorScheme::get_instance().default_icon_color());
        draw_gps_arrow(display, ColorScheme::get_instance().default_icon_color());
    }

    void set_compass_heading(float heading) {
        if (abs(heading - new_compass_heading_) < ANGLE_RESOLUTION) {
            return;
        }
        if (heading < 0.0f) {
            heading += 360.0f;
        } else if (heading >= 360.0f) {
            heading -= 360.0f;
        }
        new_compass_heading_ = heading;
        mark_dirty();
    }

    void set_gps_heading(float heading) {
        if (abs(heading - new_gps_heading_) < ANGLE_RESOLUTION) {
            return;
        }
        if (heading < 0.0f) {
            heading += 360.0f;
        } else if (heading >= 360.0f) {
            heading -= 360.0f;
        }
        new_gps_heading_ = heading;
        mark_dirty();
    }

private:
    void draw_compass_map(Display &display, uint16_t color) {
        display.draw_circle(x(), y(), radius_, color);
        display.draw_circle(x(), y(), radius_ - 1, color);

        display.draw_text(x() - 5, y() - radius_ - 15, "N", 2, color);
        display.draw_text(x() - 5, y() + radius_ + 5, "S", 2, color);
        display.draw_text(x() - radius_ - 15, y() - 5, "W", 2, color);
        display.draw_text(x() + radius_ + 5, y() - 5, "E", 2, color);
    }

    void rotate_by_angle(int16_t& x, int16_t& y, float angle) {
    int16_t x0 = x_;
    int16_t y0 = y_;
    int16_t orig_x = x;
    int16_t orig_y = y;

    float rad = angle * PI / 180.0f;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    x = static_cast<int16_t>(x0 + (orig_x - x0) * cos_a - (orig_y - y0) * sin_a);
    y = static_cast<int16_t>(y0 + (orig_x - x0) * sin_a + (orig_y - y0) * cos_a);
    }

    void draw_boat(Display &display, uint16_t color) {
        int16_t xl = x_ + px[0];
        int16_t yl = y_ + py[0];
        int16_t xr = x_ - px[0];
        int16_t yr = y_ + py[0];
        rotate_by_angle(xl, yl, compass_heading_);
        rotate_by_angle(xr, yr, compass_heading_);
        display.draw_line(xl, yl, xr, yr, color);

        for (int i = 0; i < sizeof(px)/sizeof(px[0]) - 1; ++i) {
            int16_t _xl = x_ + px[i + 1];
            int16_t _yl = y_ + py[i + 1];
            int16_t _xr = x_ - px[i + 1];
            int16_t _yr = y_ + py[i + 1];

            rotate_by_angle(_xl, _yl, compass_heading_);
            rotate_by_angle(_xr, _yr, compass_heading_);

            display.draw_line(xl, yl, _xl, _yl, color);
            display.draw_line(xr, yr, _xr, _yr, color);

            xl = _xl;
            yl = _yl;
            xr = _xr;
            yr = _yr;
        }
    }

    void draw_gps_arrow(Display &display, uint16_t color) {
        int16_t x_start = x_;
        int16_t y_start = y_;
        int16_t x_end = x_;
        int16_t y_end = y_ - radius_ + 20;

        for (int i = -1; i <= 1; i++) {
            int16_t x0 = x_start + i;
            int16_t y0 = y_start;
            int16_t x1 = x_end + i;
            int16_t y1 = y_end;
            rotate_by_angle(x0, y0, gps_heading_);
            rotate_by_angle(x1, y1, gps_heading_);
            display.draw_line(x0, y0, x1, y1, color);
        }

        // Draw arrowhead
        int16_t x0 = x_end;
        int16_t y0 = y_end - 10;
        int16_t x1 = x_end - 5;
        int16_t y1 = y_end + 5;
        int16_t x2 = x_end + 5;
        int16_t y2 = y_end + 5;
        rotate_by_angle(x0, y0, gps_heading_);
        rotate_by_angle(x1, y1, gps_heading_);
        rotate_by_angle(x2, y2, gps_heading_);
        display.draw_fill_triangle(x0, y0, x1, y1, x2, y2, color);
    }

private:
    uint16_t radius_{100};
    float gps_heading_{0.0f}; // angle in degrees
    float new_gps_heading_{0.0f}; // angle in degrees
    float compass_heading_{0.0f}; // angle in degrees
    float new_compass_heading_{0.0f}; // angle in degrees

};