#pragma once

#include "display/widgets/widget.h"
#include "display/text.h"

class MetricValueWidget : public Widget {
public:
    MetricValueWidget() : MetricValueWidget(0, 0, "", "", "") {}
    MetricValueWidget(uint16_t x, uint16_t y, const char* label, const char* value, const char* unit);
    ~MetricValueWidget() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;
    void update() override {}

    void set_value(float value);
    void set_value(const char* value);
    void set_units(const char* unit);

private:
    Text label_text_;
    Text value_text_;
    Text unit_text_;
};