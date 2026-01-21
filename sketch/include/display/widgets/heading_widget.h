#pragma once

#include "sensor_manager.h"
#include "display/widgets/widget.h"
#include "display/icons/boat_icon.h"
#include "display/icons/arrow_icon.h"
#include "display/color_scheme.h"
#include "logger.h"

namespace {
    static const float ANGLE_RESOLUTION = 3.0f; // degrees
}

class HeadingWidget : public Widget {
public:
    HeadingWidget() : HeadingWidget(0, 0) {};

    HeadingWidget(uint16_t x, uint16_t y) : 
        Widget(x, y),
        boat_(x_, y_),
        gps_arrow_(x_, y_, radius_ - 20)
    {
        set_color(ColorScheme::get_instance().header_color());
        gps_arrow_.set_visible(false);

        gps_ = SensorManager::get_instance().get_sensor<GPS>();
        compass_ = SensorManager::get_instance().get_sensor<Compass>();
    };
    
    ~HeadingWidget() = default;

    void clear_content(Display &display) override {
        draw_compass_map(display, ColorScheme::get_instance().background_color());
        boat_.clear_content(display);
        gps_arrow_.clear_content(display);
    }

    void draw_content(Display &display) override {
        draw_compass_map(display, ColorScheme::get_instance().default_icon_color());
        
        gps_arrow_.draw_content(display);
        boat_.draw_content(display);
    }

    void update() override {
        if (gps_ != nullptr) {
            const GpsSolution* sol = gps_->get_solution();

            if (sol->is_valid_ && sol->course_is_valid_) {
                gps_arrow_.set_angle(sol->course_);
    
                uint16_t arrow_length = get_arrow_length();
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
            static float step = 1.0f;
            if (compass_->get_heading(&heading) == 0) {
                LOG_DEBUG("Compass heading: " + String(heading));
                boat_.set_angle(heading + step);
                step += ANGLE_RESOLUTION;
            }
        }

        if (gps_arrow_.is_dirty() || boat_.is_dirty()) {
            mark_dirty();
        }
    }

private:

    uint16_t map_value(float x, float in_min, float in_max, float out_min, float out_max) {
        return static_cast<uint16_t>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
    }

    uint16_t get_arrow_length() {
        // float speed_mps = gps_.get_speed_knots() / 1.94384f; // Convert knots to m/s
        float speed_mps = 0.0f;
        uint16_t length = map_value(speed_mps, 0.2f, 5.0f, 20.0f, static_cast<float>(radius_ - 20));
        if (length < 20) {
            length = 0;
        } else if (length > radius_ - 20) {
            length = radius_ - 20;
        }
        return length;
    }

    void draw_compass_map(Display &display, uint16_t color) {
        display.draw_circle(x(), y(), radius_, color);
        display.draw_circle(x(), y(), radius_ - 1, color);

        display.draw_text(x() - 5, y() - radius_ - 15, "N", 2, color);
        display.draw_text(x() - 5, y() + radius_ + 5, "S", 2, color);
        display.draw_text(x() - radius_ - 15, y() - 5, "W", 2, color);
        display.draw_text(x() + radius_ + 5, y() - 5, "E", 2, color);
    }

private:
    GPS* gps_ = nullptr;
    Compass* compass_ = nullptr;

    uint16_t radius_{100};

    BoatIcon boat_;
    ArrowIcon gps_arrow_;
};