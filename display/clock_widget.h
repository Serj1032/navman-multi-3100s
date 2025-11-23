#pragma once

#include "../sensor_gps.h"
#include "../clock.h"
#include "color_scheme.h"
#include "utils.h"
#include "text.h"
#include "widget.h"

class ClockWidget : public Widget {
public:
    ClockWidget() : ClockWidget(0, 0) {}

    ClockWidget(uint16_t x, uint16_t y) : 
        Widget(x, y),
        time_(clock_.get_local_time()),
        separator_str_(":  :", x_ + 20, y_, 2),
        hours_str_("--", x_ - 4, y_, 2),
        minutes_str_("--", x_ + 32, y_, 2),
        seconds_str_("--", x_ + 67, y_, 2)
    {
        set_color(ColorScheme::get_instance().text_color());
    }
    
    ~ClockWidget() = default;

    void clear_content(Display &display) override {
        separator_str_.clear_content(display);
        hours_str_.clear_content(display);
        minutes_str_.clear_content(display);
        seconds_str_.clear_content(display);
    }

    void draw_content(Display &display) override {
        hours_str_.draw(display);
        minutes_str_.draw(display);
        seconds_str_.draw(display);
        separator_str_.draw(display);
    }

    void update() override {
        uint32_t local_time = clock_.get_local_time();
        if (local_time == time_) {
            return;
        }

        time_ = local_time;
        update_time_str();
    }

private:
    void update_time_str() {
        uint8_t hours = (time_ / 10000) % 100;
        uint8_t minutes = (time_ / 100) % 100;
        uint8_t seconds = time_ % 100;

        hours_str_.set_text(padStart(String(hours), 2, '0'));
        minutes_str_.set_text(padStart(String(minutes), 2, '0'));
        seconds_str_.set_text(padStart(String(seconds), 2, '0'));

        if (hours_str_.is_dirty() || minutes_str_.is_dirty() || seconds_str_.is_dirty()) {
            mark_dirty();
        }
    }

private:
    Clock& clock_ = Clock::get_instance();
    
    uint32_t time_;

    Text separator_str_;
    Text hours_str_;
    Text minutes_str_;
    Text seconds_str_;
};