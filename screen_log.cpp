#include "display/screen.h"
#include "display/screen_manager.h"


LogScreen::LogScreen() : Screen(),
                title_text_("NMEA Log", 10, 4, 2),
                header_icon_(0, 0, header_icon, header_width, header_height)
{
    header_icon_.set_child(&title_text_);
}

void LogScreen::draw_content(Display &display)
{
    header_icon_.set_color(ColorScheme::get_instance().header_color());
    title_text_.set_color(ColorScheme::get_instance().text_color());

    header_icon_.draw(display);
    title_text_.draw(display);
}

void LogScreen::clear(Display &display)
{
    title_text_.clear_content(display);
    header_icon_.clear_content(display);
}

void LogScreen::update()
{
    // Update widgets if needed
}

void LogScreen::key_press_handler(uint8_t button_index, ButtonEventType event_type)
{
    // Handle key press events specific to LogScreen
}
