#pragma once

#include "display/widgets/widget.h"
#include "display/text.h"
#include "clock.h"

class ClockWidget : public Widget {
public:
    ClockWidget() : ClockWidget(0, 0) {}
    ClockWidget(uint16_t x, uint16_t y);
    ~ClockWidget() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;
    void update() override;

private:
    void update_time_str();

    Clock& clock_ = Clock::get_instance();

    uint32_t time_;

    Text separator_str_;
    Text hours_str_;
    Text minutes_str_;
    Text seconds_str_;
};