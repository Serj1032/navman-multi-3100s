#pragma once

#include "text.h"
#include "color_scheme.h"
#include "icon.h"
#include "button.h"
#include "buttons.h"
#include "heading_widget.h"
#include "clock_widget.h"
#include "icons/bulb.h"
#include "icons/header.h"
#include "icons/left_arrow.h"
#include "icons/right_arrow.h"
#include "icons/settings.h"
#include "../keyboard.h"

class Screen
{
public:
    Screen() : Screen(2)
    {
    }

    Screen(uint8_t fps) : fps_(fps)
    {
        Keyboard::get_instance().subscribe(on_key_pressed, this);
    }

    virtual ~Screen()
    {
        Keyboard::get_instance().unsubscribe(on_key_pressed, this);
    }

    void draw(Display &display)
    {
        uint32_t now = millis();
        if (now - last_draw_time_ < (1000 / fps_))
        {
            return;
        }

        draw_content(display);
        last_draw_time_ = millis();
    }

    virtual void clear(Display &display) = 0;

protected:
    virtual void draw_content(Display &display) = 0;
    virtual void key_press_handler(uint8_t button_index, ButtonEventType event_type) = 0;

private:
    static void on_key_pressed(uint8_t button_index, ButtonEventType event_type, void *context)
    {
        Screen *screen = static_cast<Screen *>(context);
        screen->key_press_handler(button_index, event_type);
    }

private:
    uint8_t fps_;
    uint32_t last_draw_time_{0};
};

class DashboardScreen : public Screen
{
public:
    DashboardScreen() : Screen(),
                        title_text_("Dashboard A", 5, 4, 2),
                        header_icon_(0, 0, header_icon, header_width, header_height),
                        heading_widget_(350, 150),
                        clock_widget_(180, 10)
    {
        header_icon_.set_child(&title_text_);
        buttons_.get_button(0).set_icon(Icon(0, 0, bulb_icon, bulb_width, bulb_height));
        buttons_.get_button(1).set_icon(Icon(0, 0, settings_icon, settings_width, settings_height));
        buttons_.get_button(2).set_icon(Icon(0, 0, left_arrow_icon, left_arrow_width, left_arrow_height));
        buttons_.get_button(3).set_icon(Icon(0, 0, right_arrow_icon, right_arrow_width, right_arrow_height));
    }

    ~DashboardScreen() override = default;

    void draw_content(Display &display) override
    {
        header_icon_.set_color(ColorScheme::get_instance().header_color());
        title_text_.set_color(ColorScheme::get_instance().text_color());

        header_icon_.draw(display);
        title_text_.draw(display);
        buttons_.draw(display);
        heading_widget_.draw(display);
        clock_widget_.draw(display);
    }

    void clear(Display &display) override
    {
        title_text_.clear_content(display);
        header_icon_.clear_content(display);
        buttons_.clear_content(display);
        heading_widget_.clear_content(display);
        clock_widget_.clear_content(display);
    }

protected:
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override
    {
        if (button_index == 0 && event_type == ButtonEventType::SHORT) // Bulb button
        {
            ColorScheme::get_instance().toggle_theme();
        }
    }

private:
    Buttons buttons_;
    Text title_text_;
    Icon header_icon_;
    HeadingWidget heading_widget_;
    ClockWidget clock_widget_;
};

class LogScreen : public Screen
{
public:
    LogScreen() : Screen(),
                  title_text_("NMEA Log", 10, 4, 2),
                  header_icon_(0, 0, header_icon, header_width, header_height)
    {
        header_icon_.set_child(&title_text_);
    }
    ~LogScreen() override = default;

    void draw_content(Display &display) override
    {
        header_icon_.set_color(ColorScheme::get_instance().header_color());
        title_text_.set_color(ColorScheme::get_instance().text_color());

        header_icon_.draw(display);
        title_text_.draw(display);
    }

    void clear(Display &display) override
    {
        title_text_.clear_content(display);
        header_icon_.clear_content(display);
    }

protected:
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override
    {
        // Handle key press events specific to LogScreen
    }

private:
    Text title_text_;
    Icon header_icon_;
};