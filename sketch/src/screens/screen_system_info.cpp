#include "display/screen.h"
#include "display/screen_manager.h"
#include "display/color_scheme.h"

#define LOG_TAG "S-SYS"
#include "logger.h"

SystemInfoScreen::SystemInfoScreen()
    : Screen()
    , title_text_("System Info", 10, 4, 2)
    , header_icon_(0, 0, header_icon, header_width, header_height)
{
    header_icon_.set_child(&title_text_);

    buttons_.get_button(2).set_icon(BitmapIcon(0, 0, left_arrow_icon, left_arrow_width, left_arrow_height));
    buttons_.get_button(3).set_icon(BitmapIcon(0, 0, right_arrow_icon, right_arrow_width, right_arrow_height));
}

void SystemInfoScreen::draw_screen(Display &display)
{
    header_icon_.set_color(ColorScheme::get_instance().header_color());
    title_text_.set_color(ColorScheme::get_instance().text_color());
    buttons_.draw(display);

    header_icon_.draw(display);
    title_text_.draw(display);
}

void SystemInfoScreen::clear_screen(Display &display)
{
    title_text_.clear_content(display);
    header_icon_.clear_content(display);
    buttons_.clear_content(display);
}

void SystemInfoScreen::update()
{
    // TODO: implement
}

void SystemInfoScreen::key_press_handler(uint8_t button_index, ButtonEventType event_type)
{
    if (button_index == 2 && event_type == ButtonEventType::SHORT) 
    {
        ScreenManager::get_instance().next_screen();
    } 
    else if (button_index == 3 && event_type == ButtonEventType::SHORT) 
    {
        ScreenManager::get_instance().previous_screen();
    } 
}
