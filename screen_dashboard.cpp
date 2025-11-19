#include "display/screen.h"
#include "display/screen_manager.h"


DashboardScreen::DashboardScreen() : Screen(),
                    title_text_("Dashboard A", 5, 4, 2),
                    header_icon_(0, 0, header_icon, header_width, header_height),
                    heading_widget_(350, 150),
                    clock_widget_(180, 5),
                    metrics_widget_(20, 40),
                    satellite_widget_(450, 5)
{
    header_icon_.set_child(&title_text_);
    buttons_.get_button(0).set_icon(Icon(0, 0, bulb_icon, bulb_width, bulb_height));
    // buttons_.get_button(1).set_icon(Icon(0, 0, settings_icon, settings_width, settings_height));
    // buttons_.get_button(2).set_icon(Icon(0, 0, left_arrow_icon, left_arrow_width, left_arrow_height));
    // buttons_.get_button(3).set_icon(Icon(0, 0, right_arrow_icon, right_arrow_width, right_arrow_height));
}

void DashboardScreen::draw_content(Display &display)
{
    header_icon_.set_color(ColorScheme::get_instance().header_color());
    title_text_.set_color(ColorScheme::get_instance().text_color());

    header_icon_.draw(display);
    title_text_.draw(display);
    buttons_.draw(display);
    heading_widget_.draw(display);
    clock_widget_.draw(display);
    metrics_widget_.draw(display);
    satellite_widget_.draw(display);
}

void DashboardScreen::clear(Display &display)
{
    title_text_.clear_content(display);
    header_icon_.clear_content(display);
    buttons_.clear_content(display);
    heading_widget_.clear_content(display);
    clock_widget_.clear_content(display);
    metrics_widget_.clear_content(display);
    satellite_widget_.clear_content(display);
}


void DashboardScreen::update()
{
    // Update widgets if needed
}

void DashboardScreen::key_press_handler(uint8_t button_index, ButtonEventType event_type)
{
    if (button_index == 0 && event_type == ButtonEventType::SHORT) // Bulb button
    {
        ColorScheme::get_instance().toggle_theme();
    }
}
