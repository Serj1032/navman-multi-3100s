#pragma once

#include "drawable.h"
#include "color_scheme.h"
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


class MetricsWidget : public Drawable {
public:
    MetricsWidget() : MetricsWidget(0, 0) {};

    MetricsWidget(uint16_t x, uint16_t y) : Drawable(x, y) {
        set_color(ColorScheme::get_instance().header_color());      
        
        ground_speed_str_ = String(" --.-");
        ground_speed_ = MAX_SPEED_MPS;
        new_ground_speed_ = ground_speed_;
        water_speed_str_ = String(" --.-");
        water_speed_ = MAX_SPEED_MPS;
        new_water_speed_ = water_speed_;
        depth_str_ = String("---.-");
        depth_ = MAX_DEPTH_M;
        new_depth_ = depth_;
    };
    
    ~MetricsWidget() = default;

    void clear_content(Display &display) override {
        draw_ground_speed(display,  ColorScheme::get_instance().background_color());
        draw_water_speed(display,  ColorScheme::get_instance().background_color());
        draw_depth(display,  ColorScheme::get_instance().background_color());
    }

    void draw_content(Display &display) override {
        if (!initialized_) {
            draw_ground_speed(display, ColorScheme::get_instance().text_color());
            draw_water_speed(display, ColorScheme::get_instance().text_color());
            draw_depth(display, ColorScheme::get_instance().text_color());
            initialized_ = true;
            return;
        }

        if (abs(new_ground_speed_ - ground_speed_) >= SPEED_RESOLUTION) {
            draw_ground_speed(display, ColorScheme::get_instance().background_color(), true);
            ground_speed_ = new_ground_speed_;
            ground_speed_str_ = padStart(String(convert_speed(ground_speed_), 1), 5, ' ');
            draw_ground_speed(display, ColorScheme::get_instance().text_color(), true);
        }
        
        if (abs(new_water_speed_ - water_speed_) >= SPEED_RESOLUTION) {
            draw_water_speed(display, ColorScheme::get_instance().background_color(), true);
            water_speed_ = new_water_speed_;
            water_speed_str_ = padStart(String(convert_speed(water_speed_), 1), 5, ' ');
            draw_water_speed(display, ColorScheme::get_instance().text_color(), true);
        }

        if (abs(new_depth_ - depth_) >= DEPTH_RESOLUTION) {
            draw_depth(display, ColorScheme::get_instance().background_color(), true);
            depth_ = new_depth_;
            depth_str_ = padStart(String(depth_, 1), 5, ' ');
            draw_depth(display, ColorScheme::get_instance().text_color(), true);
        }
    }

    void set_ground_speed(float speed_mps) {
        if (abs(speed_mps - new_ground_speed_) < SPEED_RESOLUTION) {
            return;
        }
        new_ground_speed_ = speed_mps;
        if (new_ground_speed_ > MAX_SPEED_MPS) {
            new_ground_speed_ = MAX_SPEED_MPS;
        } else if (new_ground_speed_ < -MAX_SPEED_MPS) {
            new_ground_speed_ = -MAX_SPEED_MPS;
        }
        mark_dirty();
    }

    void set_water_speed(float speed_mps) {
        if (abs(speed_mps - new_water_speed_) < SPEED_RESOLUTION) {
            return;
        }
        new_water_speed_ = speed_mps;
        if (new_water_speed_ > MAX_SPEED_MPS) {
            new_water_speed_ = MAX_SPEED_MPS;
        } else if (new_water_speed_ < -MAX_SPEED_MPS) {
            new_water_speed_ = -MAX_SPEED_MPS;
        } 
        mark_dirty();
    }

    void set_depth(float depth_m) {
        if (abs(depth_m - new_depth_) < DEPTH_RESOLUTION) {
            return;
        } else if (depth_m < 0.0f) {
            depth_m = 0.0f;
        }
        new_depth_ = depth_m;
        if (new_depth_ > MAX_DEPTH_M) {
            new_depth_ = MAX_DEPTH_M;
        }
        mark_dirty();
    }

private:
    void draw_ground_speed(Display &display, uint16_t color, bool value_only = false) {
        draw_metric_unit(GROUND_SPEED, ground_speed_str_, SPEED_UNIT_STRINGS[static_cast<int>(speed_unit_)], 0, 0, display, color, value_only);
    }

    void draw_water_speed(Display &display, uint16_t color, bool value_only = false) {
        draw_metric_unit(WATER_SPEED, water_speed_str_, SPEED_UNIT_STRINGS[static_cast<int>(speed_unit_)], 0, 80, display, color, value_only);
    }

    void draw_depth(Display &display, uint16_t color, bool value_only = false) {
        draw_metric_unit(DEPTH, depth_str_, "m", 0, 160, display, color, value_only);
    }

    void draw_metric_unit(const char* label, const String &value, const char* unit, uint16_t x, uint16_t y, Display &display, uint16_t color, bool value_only) {
        if (value_only) {
            display.draw_text(x_ + x, y_ + y + 30, value.c_str(), 4, color);
            return;
        }
        display.draw_text(x_ + x, y_ + y, label, 2, color);
        display.draw_text(x_ + x + 140, y_ + y + 40, unit, 2, color);
        display.draw_text(x_ + x, y_ + y + 30, value.c_str(), 4, color);
    }

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
    bool initialized_ = false;

    String ground_speed_str_;
    float ground_speed_; // m/s
    float new_ground_speed_; // m/s

    String water_speed_str_;
    float water_speed_; // m/s
    float new_water_speed_; // m/s

    String depth_str_;
    float depth_; // m
    float new_depth_; // m

    SpeedUnit speed_unit_ = SpeedUnit::MPS;
};