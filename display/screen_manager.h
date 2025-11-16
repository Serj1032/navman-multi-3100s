#pragma once

#include "screen.h"
#include "display.h"

class ScreenManager
{
public:
    ~ScreenManager() = default;

    static ScreenManager &get_instance()
    {
        static ScreenManager instance;
        return instance;
    }

    void init(Display *display)
    {
        display_ = display;
        current_screen_ = new DashboardScreen();
    }

    void reinit()
    {
        if (current_screen_ != nullptr)
        {
            delete current_screen_;
        }
        display_->clear();
        current_screen_ = new DashboardScreen();
    }

    void process()
    {
        if (current_screen_ && display_)
        {
            current_screen_->draw(*display_);
        }
    }

private:
    ScreenManager() = default;

private:
    Display *display_ = nullptr;
    Screen *current_screen_ = nullptr;
};