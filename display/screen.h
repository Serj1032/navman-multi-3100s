#pragma once

#include "text.h"
#include "color_scheme.h"
#include "icon.h"
#include "button.h"
#include "buttons.h"
#include "heading_widget.h"
#include "clock_widget.h"
#include "metrics_widget.h"
#include "satellite_widget.h"
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
    
    virtual void update() = 0;

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
    DashboardScreen();
    ~DashboardScreen() override = default;

    void update() override;
    void draw_content(Display &display) override;
    void clear(Display &display) override;

protected:
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    Buttons buttons_;
    Text title_text_;
    Icon header_icon_;
    HeadingWidget heading_widget_;
    ClockWidget clock_widget_;
    MetricsWidget metrics_widget_;
    SatelliteWidget satellite_widget_;
};


class WelcomeScreen : public Screen
{
public:
    WelcomeScreen();
    ~WelcomeScreen() override = default;
    
    void update() override;
    void draw_content(Display &display) override;
    void clear(Display &display) override;

private:
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    uint32_t ts_;

    Text line1_;
    Text line2_;
    Text author_;
};


class LogScreen : public Screen
{
public:
    LogScreen();
    ~LogScreen() override = default;

    void update() override;
    void draw_content(Display &display) override;
    void clear(Display &display) override;

protected:
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    Text title_text_;
    Icon header_icon_;
};