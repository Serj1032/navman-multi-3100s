#pragma once

#include "sensors/sensor_gps.h"
#include "display/widgets/widget.h"
#include "display/icons/bitmap_icon.h"
#include "display/assets/sattelite.h"
#include "display/color_scheme.h"
#include "display/text.h"
#include "utils.h"

class SatelliteWidget : public Widget {
public:
    SatelliteWidget() : SatelliteWidget(0, 0) {};

    SatelliteWidget(uint16_t x, uint16_t y) : 
            Widget(x, y),
            sv_count_text_(" 0", x_, y_ + 5, 2),
            satellite_icon_(x_ + 30, y_, sattelite_icon, sattelite_width, sattelite_height),
            mode_text_("", x_ + 60, y_ + 5, 2)
    {
        set_color(ColorScheme::get_instance().header_color());
    };
    
    ~SatelliteWidget() = default;

    void clear_content(Display &display) override {
        satellite_icon_.clear_content(display);
        sv_count_text_.clear_content(display);
        mode_text_.clear_content(display);
    }

    void draw_content(Display &display) override {
        satellite_icon_.draw(display);
        sv_count_text_.draw(display);
        mode_text_.draw(display);
    }

    void update() override {
        uint16_t sv_count = gps_.get_sv_count();
        sv_count_text_.set_text(padStart(String(sv_count), 2, ' '));
    
        if (gps_.is_valid()) {
            satellite_icon_.set_color(ColorScheme::get_instance().default_icon_color());
        } else {
            satellite_icon_.set_color(ColorScheme::get_instance().warning_color());
        }

        mode_text_.set_text(String(gps_.get_mode()));

        if (sv_count_text_.is_dirty() || satellite_icon_.is_dirty() || mode_text_.is_dirty()) {
            mark_dirty();
        }
    }

private:
    GPS& gps_ = GPS::get_instance();

    Text sv_count_text_;
    BitmapIcon satellite_icon_;
    Text mode_text_;
};