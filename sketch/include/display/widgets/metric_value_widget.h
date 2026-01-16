#pragma once

#include "display/widgets/widget.h"
#include "display/text.h"
#include "utils.h"

class MetricValueWidget : public Widget {
public:
    MetricValueWidget() : MetricValueWidget(0, 0, "", "", "") {};

    MetricValueWidget(uint16_t x, uint16_t y, const char* label, const char* value,const char* unit) : 
        Widget(x, y),
        label_text_(label, x_, y_, 2),
        value_text_(value, x_, y_ + 30, 4),
        unit_text_(unit, x_ + 140, y_ + 40, 2)
    {
        set_color(ColorScheme::get_instance().text_color());
    };
    
    ~MetricValueWidget() = default;

    void clear_content(Display &display) override {
        label_text_.clear_content(display);
        value_text_.clear_content(display);
        unit_text_.clear_content(display);
    }

    void draw_content(Display &display) override {
        label_text_.draw(display);
        value_text_.draw(display);
        unit_text_.draw(display);
    }

    void update() override {
        // No dynamic content to update
    }

    void set_value(float value) {        
        value_text_.set_text(padStart(String(value, 1), 5, ' '));
        if (value_text_.is_dirty()) {
            mark_dirty();
        }
    }

    void set_value(const char* value) {
        value_text_.set_text(value);
        if (value_text_.is_dirty()) {
            mark_dirty();
        }
    }


    void set_units(const char* unit) {
        unit_text_.set_text(unit);
        if (unit_text_.is_dirty()) {
            mark_dirty();
        }
    }

private:
    Text label_text_;
    Text value_text_;
    Text unit_text_;
};