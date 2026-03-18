#pragma once

#include "display/widgets/widget.h"
#include "display/icons/boat_icon.h"
#include "display/icons/arrow_icon.h"

class GPS;
class Compass;

class HeadingWidget : public Widget {
public:
    HeadingWidget() : HeadingWidget(0, 0) {}
    HeadingWidget(uint16_t x, uint16_t y);
    ~HeadingWidget() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;
    void update() override;

private:
    uint16_t map_value(float x, float in_min, float in_max, float out_min, float out_max);
    uint16_t compute_arrow_length(float speed_knots);
    void draw_compass_rose(Display &display, uint16_t color);

    GPS* gps_ = nullptr;
    Compass* compass_ = nullptr;

    uint16_t radius_{100};

    BoatIcon boat_;
    ArrowIcon gps_arrow_;
};