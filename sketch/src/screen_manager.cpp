#include "display/screen_manager.h"

ScreenManager::ScreenManager() :
    display_(nullptr),
    current_screen_(nullptr),
    current_screen_type_(ScreenType::NONE),
    next_screen_type_(ScreenType::WELCOME),
    is_day_mode_(ColorScheme::get_instance().is_day_mode())
{
}


void ScreenManager::init(Display *display)
{
    display_ = display;
}

void ScreenManager::process()
{
    if (current_screen_type_ != next_screen_type_) {
        change_screen();
    }

    if (current_screen_type_ != ScreenType::WELCOME) {    
        bool current_day_mode = ColorScheme::get_instance().is_day_mode();
        if (is_day_mode_ != current_day_mode)
        {
            is_day_mode_ = current_day_mode;
            reinit();
        }
    }

    if (current_screen_ && display_)
    {
        current_screen_->update();
        current_screen_->draw(*display_);
    }
}

void ScreenManager::reinit()
{
    if (current_screen_ != nullptr)
    {
        delete current_screen_;
        current_screen_ = nullptr;
    }
    display_->clear();
    next_screen_type_ = current_screen_type_;
    current_screen_type_ = ScreenType::NONE;
}

void ScreenManager::change_screen() {
    if (current_screen_ != nullptr) {
        current_screen_->clear(*display_);
        delete current_screen_;
        current_screen_ = nullptr;
    }

    // Serial.print("Switching to screen: ");
    switch (next_screen_type_) {
    case ScreenType::WELCOME:
        current_screen_ = new WelcomeScreen();
        // Serial.println("WELCOME");
        break;
    case ScreenType::DASHBOARD:
        current_screen_ = new DashboardScreen();
        // Serial.println("DASHBOARD");
        break;
    case ScreenType::LOG:
        current_screen_ = new LogScreen();
        // Serial.println("LOG");
        break;
    default:
        break;
    }

    current_screen_type_ = next_screen_type_;
}