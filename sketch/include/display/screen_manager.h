#pragma once

#include "display/screen.h"
#include "display/display.h"
#include "display/color_scheme.h"

enum class ScreenType : uint8_t
{
    NONE = 0,
    WELCOME,
    DASHBOARD,
    SYSTEM_INFO,
    LOGS,
    SCREEN_COUNT  // always last — used for navigation
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

    void next_screen();
    void previous_screen();

private:
    ScreenManager();

    void reinit();

    void change_screen();

private:
    Display *display_ = nullptr;
    ScreenType current_screen_type_;
    ScreenType next_screen_type_;
    Screen *current_screen_ = nullptr;
    bool is_day_mode_;
};