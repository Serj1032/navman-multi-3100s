#pragma once

#include "display/drawable.h"

class ArrowIcon : public Drawable {
public:
    ArrowIcon() : ArrowIcon(0, 0) {}
    ArrowIcon(uint16_t x, uint16_t y, uint16_t length = 80, float resolution = 1.0f);
    ~ArrowIcon() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;

    void set_angle(float angle);
    void set_length(uint16_t length);

private:
    void draw_arrow(Display &display, uint16_t color);

    float resolution_;
    uint16_t length_;
    uint16_t new_length_;
    float angle_;
    float new_angle_;
};
