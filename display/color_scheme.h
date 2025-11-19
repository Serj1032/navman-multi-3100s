#pragma once

#include "Arduino.h"

class ColorScheme
{
public:
    virtual ~ColorScheme() = default;

    static ColorScheme &get_instance()
    {
        static ColorScheme instance;
        return instance;
    }

    uint16_t background_color();

    uint16_t text_color();

    uint16_t default_icon_color();

    uint16_t header_color();

    uint16_t button_color();

    void set_day_mode(bool day_mode);

    void toggle_theme();

    bool is_day_mode() const { return day_mode_; }

private:
    ColorScheme() = default;

private:
    bool day_mode_ = false;
};