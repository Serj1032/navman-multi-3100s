#include "display/screen_manager.h"

ScreenManager::ScreenManager() :
    display_(nullptr),
    current_screen_type_(ScreenType::NONE),
    next_screen_type_(ScreenType::WELCOME),
    current_screen_(nullptr),
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

void ScreenManager::next_screen() {
    uint8_t next = static_cast<uint8_t>(current_screen_type_) + 1;
    // Skip WELCOME (index 1) when cycling — it's only shown at startup
    if (next >= static_cast<uint8_t>(ScreenType::SCREEN_COUNT)) {
        next = static_cast<uint8_t>(ScreenType::DASHBOARD);
    }
    set_next_screen(static_cast<ScreenType>(next));
}

void ScreenManager::previous_screen() {
    uint8_t prev = static_cast<uint8_t>(current_screen_type_) - 1;
    // Skip WELCOME and NONE — wrap to last usable screen
    if (prev <= static_cast<uint8_t>(ScreenType::WELCOME)) {
        prev = static_cast<uint8_t>(ScreenType::SCREEN_COUNT) - 1;
    }
    set_next_screen(static_cast<ScreenType>(prev));
}

void ScreenManager::change_screen() {
    if (current_screen_ != nullptr) {
        current_screen_->clear(*display_);
        delete current_screen_;
        current_screen_ = nullptr;
    }

    switch (next_screen_type_) {
    case ScreenType::WELCOME:
        current_screen_ = new WelcomeScreen();
        break;
    case ScreenType::DASHBOARD:
        current_screen_ = new DashboardScreen();
        break;
    case ScreenType::LOGS:
        current_screen_ = new LogsScreen();
        break;
    case ScreenType::SYSTEM_INFO:
        current_screen_ = new SystemInfoScreen();
        break;
    default:
        break;
    }

    current_screen_type_ = next_screen_type_;
}