#pragma once

#include "display/widgets/widget.h"
#include "display/widgets/metric_value_widget.h"

class GPS;
class Navman;

enum class SpeedUnit {
    MPS,    // meters per second
    KPH,    // kilometers per hour
    KTS,    // knots
};

class MetricsWidget : public Widget {
public:
    MetricsWidget() : MetricsWidget(0, 0) {}
    MetricsWidget(uint16_t x, uint16_t y);
    ~MetricsWidget() = default;

    void clear_content(Display &display) override;
    void draw_content(Display &display) override;
    void update() override;

private:
    float clamp_speed(float speed_mps);
    float convert_speed(float speed_mps);
    void update_ground_speed();
    void update_water_speed();
    void update_depth();

    GPS* gps_ = nullptr;
    Navman* navman_ = nullptr;

    SpeedUnit speed_unit_;

    MetricValueWidget ground_speed_;
    MetricValueWidget water_speed_;
    MetricValueWidget depth_;
};