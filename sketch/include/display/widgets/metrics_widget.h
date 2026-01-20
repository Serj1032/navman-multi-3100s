#pragma once

#include "sensor_manager.h"
#include "display/widgets/widget.h"
#include "display/color_scheme.h"
#include "display/widgets/metric_value_widget.h"
#include "utils.h"

namespace {
    static const char* SPEED_UNIT_STRINGS[] = {
        (char*)"m/s",
        (char*)"km/h",
        (char*)"kts"
    };  

    static const char GROUND_SPEED[] = "Ground Speed";
    static const char WATER_SPEED[] = "Water Speed";
    static const char DEPTH[] = "Depth";

    static const float SPEED_RESOLUTION = 0.1f; // units depend on selected SpeedUnit
    static const float DEPTH_RESOLUTION = 0.1f; // meters

    static const float MAX_SPEED_MPS = 60.0f; // 60 m/s ~ 116 kts ~ 216 km/h
    static const float MAX_DEPTH_M = 999.0f; // 999 meters
}


enum class SpeedUnit {
    MPS,    // meters per second
    KPH,    // kilometers per hour
    KTS,    // knots
};


class MetricsWidget : public Widget {
public:
    MetricsWidget() : MetricsWidget(0, 0) {};

    MetricsWidget(uint16_t x, uint16_t y) : 
        Widget(x, y),
        speed_unit_(SpeedUnit::MPS),
        ground_speed_(x_, y_, GROUND_SPEED, " --.-", SPEED_UNIT_STRINGS[static_cast<int>(speed_unit_)]),
        water_speed_(x_, y_ + 80, WATER_SPEED, " --.-", SPEED_UNIT_STRINGS[static_cast<int>(speed_unit_)]),
        depth_(x_, y_ + 160, DEPTH, "---.-", "m")
    {
        set_color(ColorScheme::get_instance().header_color());    
        gps_ = SensorManager::get_instance().get_sensor<GPS>();
    };
    
    ~MetricsWidget() = default;

    void clear_content(Display &display) override {
        ground_speed_.clear_content(display);
        water_speed_.clear_content(display);
        depth_.clear_content(display);
    }

    void draw_content(Display &display) override {
        ground_speed_.draw(display);
        water_speed_.draw(display);
        depth_.draw(display);
    }


    void update() override {
        if (!gps_) {
            return;
        }

        const GpsSolution* sol = gps_->get_solution();

        if (!sol->is_valid_) {
            ground_speed_.set_value(" --.-");
        } else {
            float speed_mps = sol->speed_knots_ / 1.94384f;
            if (speed_mps < -MAX_SPEED_MPS) {
                speed_mps = -MAX_SPEED_MPS;
            } else if (speed_mps > MAX_SPEED_MPS) {
                speed_mps = MAX_SPEED_MPS;
            }
            ground_speed_.set_value(convert_speed(speed_mps));
        }

        if (ground_speed_.is_dirty() || water_speed_.is_dirty() || depth_.is_dirty()) {
            mark_dirty();
        }
    }


private:
    float convert_speed(float speed_mps) {
        switch (speed_unit_) {
            case SpeedUnit::MPS:
                return speed_mps;
            case SpeedUnit::KPH:
                return speed_mps * 3.6f;
            case SpeedUnit::KTS:
                return speed_mps * 1.94384f;
            default:
                return speed_mps;
        }
    }

private:
    GPS* gps_ = nullptr;

    SpeedUnit speed_unit_;
    
    MetricValueWidget ground_speed_;
    MetricValueWidget water_speed_;
    MetricValueWidget depth_;
};