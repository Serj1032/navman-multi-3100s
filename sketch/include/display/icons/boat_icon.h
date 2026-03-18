#pragma once

#include "display/drawable.h"

class BoatIcon : public Drawable {
public:
    BoatIcon() : BoatIcon(0, 0) {}
    BoatIcon(uint16_t x, uint16_t y, float resolution = 1.0f);
    ~BoatIcon() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;

    void set_angle(float angle);

private:
    void draw_boat(Display &display, uint16_t color);
    void draw_thick_line(Display &display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

    float resolution_;
    float angle_;
    float new_angle_;
    int width_ = 3;
};