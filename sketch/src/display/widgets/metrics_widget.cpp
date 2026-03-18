#include "display/widgets/metrics_widget.h"
#include "display/color_scheme.h"
#include "sensor_manager.h"

namespace {
    const char* SPEED_UNIT_STRINGS[] = {
        "m/s",
        "km/h",
        "kts"
    };

    const char GROUND_SPEED[] = "Ground Speed";
    const char WATER_SPEED[] = "Water Speed";
    const char DEPTH[] = "Depth";

    const float MAX_SPEED_MPS = 60.0f;   // 60 m/s ~ 116 kts ~ 216 km/h
    const float MAX_DEPTH_M   = 999.0f;  // 999 meters
}

MetricsWidget::MetricsWidget(uint16_t x, uint16_t y)
    : Widget(x, y)
    , speed_unit_(SpeedUnit::MPS)
    , ground_speed_(x_, y_, GROUND_SPEED, " --.-", SPEED_UNIT_STRINGS[static_cast<int>(speed_unit_)])
    , water_speed_(x_, y_ + 80, WATER_SPEED, " --.-", SPEED_UNIT_STRINGS[static_cast<int>(speed_unit_)])
    , depth_(x_, y_ + 160, DEPTH, "---.-", "m")
{
    set_color(ColorScheme::get_instance().header_color());
    gps_ = SensorManager::get_instance().get_sensor<GPS>();
    navman_ = SensorManager::get_instance().get_sensor<Navman>();
}

void MetricsWidget::clear_content(Display &display) {
    ground_speed_.clear_content(display);
    water_speed_.clear_content(display);
    depth_.clear_content(display);
}

void MetricsWidget::draw_content(Display &display) {
    ground_speed_.draw(display);
    water_speed_.draw(display);
    depth_.draw(display);
}

void MetricsWidget::update() {
    update_ground_speed();
    update_water_speed();
    update_depth();

    if (ground_speed_.is_dirty() || water_speed_.is_dirty() || depth_.is_dirty()) {
        mark_dirty();
    }
}

float MetricsWidget::clamp_speed(float speed_mps) {
    if (speed_mps < -MAX_SPEED_MPS) {
        return -MAX_SPEED_MPS;
    }
    if (speed_mps > MAX_SPEED_MPS) {
        return MAX_SPEED_MPS;
    }
    return speed_mps;
}

float MetricsWidget::convert_speed(float speed_mps) {
    switch (speed_unit_) {
        case SpeedUnit::MPS: return speed_mps;
        case SpeedUnit::KPH: return speed_mps * 3.6f;
        case SpeedUnit::KTS: return speed_mps * 1.94384f;
        default:             return speed_mps;
    }
}

void MetricsWidget::update_ground_speed() {
    if (!gps_) {
        return;
    }
    const GpsSolution* sol = gps_->get_solution();
    if (!sol->is_valid_) {
        ground_speed_.set_value(" --.-");
        return;
    }
    float speed_mps = clamp_speed(sol->speed_knots_ / 1.94384f);
    ground_speed_.set_value(convert_speed(speed_mps));
}

void MetricsWidget::update_water_speed() {
    if (!navman_) {
        return;
    }
    const NavmanSolution* nsol = navman_->get_solution();
    if (nsol->data_available_ && nsol->water_speed_valid_) {
        float speed_mps = clamp_speed(nsol->water_speed_knots_ / 1.94384f);
        water_speed_.set_value(convert_speed(speed_mps));
    } else {
        water_speed_.set_value(" --.-");
    }
}

void MetricsWidget::update_depth() {
    if (!navman_) {
        return;
    }
    const NavmanSolution* nsol = navman_->get_solution();
    if (nsol->data_available_ && nsol->depth_valid_) {
        float depth = nsol->depth_m_;
        if (depth > MAX_DEPTH_M) {
            depth = MAX_DEPTH_M;
        }
        depth_.set_value(depth);
    } else {
        depth_.set_value("---.-");
    }
}
