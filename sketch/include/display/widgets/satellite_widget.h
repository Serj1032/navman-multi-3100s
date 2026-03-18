#pragma once

#include "display/widgets/widget.h"
#include "display/icons/bitmap_icon.h"
#include "display/text.h"

class GPS;

class SatelliteWidget : public Widget {
public:
    SatelliteWidget() : SatelliteWidget(0, 0) {}
    SatelliteWidget(uint16_t x, uint16_t y);
    ~SatelliteWidget() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;
    void update() override;

private:
    GPS* gps_ = nullptr;

    Text sv_count_text_;
    BitmapIcon satellite_icon_;
    Text mode_text_;
};