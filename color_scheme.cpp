#include "display/color_scheme.h"

namespace
{
    static constexpr uint16_t colorRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

    static constexpr uint16_t WHITE = colorRGB(255, 255, 255);
    static constexpr uint16_t BLACK = colorRGB(0, 0, 0);
    static constexpr uint16_t ICON_DAY = colorRGB(200, 200, 200);
    static constexpr uint16_t ICON_NIGHT = colorRGB(50, 50, 50);
}

uint16_t ColorScheme::background_color()
{
    return day_mode_ ? WHITE : BLACK;
}

uint16_t ColorScheme::text_color()
{
    return day_mode_ ? BLACK : WHITE;
}

uint16_t ColorScheme::default_icon_color()
{
    return day_mode_ ? BLACK : WHITE;
}

uint16_t ColorScheme::header_color()
{
    return day_mode_ ? ICON_DAY : ICON_NIGHT;
}

uint16_t ColorScheme::button_color()
{
    return day_mode_ ? ICON_DAY : ICON_NIGHT;
}

void ColorScheme::set_day_mode(bool day_mode)
{
    day_mode_ = day_mode;
}

void ColorScheme::toggle_theme()
{
    day_mode_ = !day_mode_;
}
