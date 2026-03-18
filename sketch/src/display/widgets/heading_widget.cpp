#include "display/widgets/heading_widget.h"
#include "display/color_scheme.h"
#include "sensor_manager.h"

HeadingWidget::HeadingWidget(uint16_t x, uint16_t y)
    : Widget(x, y)
    , boat_(x_, y_)
    , gps_arrow_(x_, y_, radius_ - 20)
{
    set_color(ColorScheme::get_instance().header_color());
    gps_arrow_.set_visible(false);

    gps_ = SensorManager::get_instance().get_sensor<GPS>();
    compass_ = SensorManager::get_instance().get_sensor<Compass>();
}

void HeadingWidget::clear_content(Display &display) {
    draw_compass_rose(display, ColorScheme::get_instance().background_color());
    boat_.clear_content(display);
    gps_arrow_.clear_content(display);
}

void HeadingWidget::draw_content(Display &display) {
    draw_compass_rose(display, ColorScheme::get_instance().default_icon_color());
    gps_arrow_.draw_content(display);
    boat_.draw_content(display);
}

void HeadingWidget::update() {
    if (gps_ != nullptr) {
        const GpsSolution* sol = gps_->get_solution();

        if (sol->is_valid_ && sol->course_is_valid_) {
            gps_arrow_.set_angle(sol->course_);

            uint16_t arrow_length = compute_arrow_length(sol->speed_knots_);
            if (arrow_length == 0) {
                gps_arrow_.set_visible(false);
            } else {
                gps_arrow_.set_visible(true);
                gps_arrow_.set_length(arrow_length);
            }
        } else {
            gps_arrow_.set_visible(false);
        }
    }

    if (compass_ != nullptr) {
        float heading = 0.0f;
        if (compass_->get_heading(&heading) == 0) {
            boat_.set_angle(heading);
        }
    }

    if (gps_arrow_.is_dirty() || boat_.is_dirty()) {
        mark_dirty();
    }
}

uint16_t HeadingWidget::map_value(float x, float in_min, float in_max, float out_min, float out_max) {
    return static_cast<uint16_t>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

uint16_t HeadingWidget::compute_arrow_length(float speed_knots) {
    float speed_mps = speed_knots / 1.94384f; // Convert knots to m/s
    uint16_t length = map_value(speed_mps, 0.2f, 5.0f, 20.0f, static_cast<float>(radius_ - 20));
    if (length < 20) {
        length = 0;
    } else if (length > radius_ - 20) {
        length = radius_ - 20;
    }
    return length;
}

void HeadingWidget::draw_compass_rose(Display &display, uint16_t color) {
    display.draw_circle(x(), y(), radius_, color);
    display.draw_circle(x(), y(), radius_ - 1, color);
    display.draw_circle(x(), y(), radius_ - 2, color);

    display.draw_text(x() - 5, y() - radius_ - 15, "N", 2, color);
    display.draw_text(x() - 5, y() + radius_ + 5, "S", 2, color);
    display.draw_text(x() - radius_ - 15, y() - 5, "W", 2, color);
    display.draw_text(x() + radius_ + 5, y() - 5, "E", 2, color);
}
