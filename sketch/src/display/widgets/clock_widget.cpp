#include "display/widgets/clock_widget.h"
#include "display/color_scheme.h"
#include "clock.h"
#include "utils.h"

ClockWidget::ClockWidget(uint16_t x, uint16_t y)
    : Widget(x, y)
    , time_(clock_.get_local_time())
    , separator_str_(":  :", x_ + 20, y_, 2)
    , hours_str_("--", x_ - 4, y_, 2)
    , minutes_str_("--", x_ + 32, y_, 2)
    , seconds_str_("--", x_ + 67, y_, 2)
{
    set_color(ColorScheme::get_instance().text_color());
}

void ClockWidget::clear_content(Display &display) {
    separator_str_.clear_content(display);
    hours_str_.clear_content(display);
    minutes_str_.clear_content(display);
    seconds_str_.clear_content(display);
}

void ClockWidget::draw_content(Display &display) {
    hours_str_.draw(display);
    minutes_str_.draw(display);
    seconds_str_.draw(display);
    separator_str_.draw(display);
}

void ClockWidget::update() {
    uint32_t local_time = clock_.get_local_time();
    if (local_time == time_) {
        return;
    }

    time_ = local_time;
    update_time_str();
}

void ClockWidget::update_time_str() {
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
