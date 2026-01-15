#pragma once

#include "sensors/sensor_gps.h"
#include "screen.h"
#include "display.h"
#include "color_scheme.h"

enum class ScreenType : uint8_t
{
    NONE = 0,
    WELCOME,
    DASHBOARD,
    LOG,
};

class ScreenManager
{
public:
    ~ScreenManager() = default;

    static ScreenManager &get_instance()
    {
        static ScreenManager instance;
        return instance;
    }

    void init(Display *display);

    void process();

    void set_next_screen(ScreenType screen_type)
    {
        next_screen_type_ = screen_type;
    }

private:
    ScreenManager();

    void reinit();

    void change_screen();

private:
    GPS& gps_ = GPS::get_instance();
    Display *display_ = nullptr;
    ScreenType current_screen_type_;
    ScreenType next_screen_type_;
    Screen *current_screen_ = nullptr;
    bool is_day_mode_;
};