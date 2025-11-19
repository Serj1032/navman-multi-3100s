#include "display/screen_manager.h"


void ScreenManager::init(Display *display)
{
    display_ = display;
    is_day_mode_ = ColorScheme::get_instance().is_day_mode();
    current_screen_ = nullptr;
    current_screen_type_ = ScreenType::NONE;
    next_screen_type_ = ScreenType::WELCOME;
}

void ScreenManager::process()
{
    if (is_day_mode_ != ColorScheme::get_instance().is_day_mode())
    {
        is_day_mode_ = ColorScheme::get_instance().is_day_mode();
        reinit();
        return;
    }

    if (current_screen_type_ != next_screen_type_) {
        change_screen();
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

    switch (next_screen_type_) {
    case ScreenType::WELCOME:
        current_screen_ = new WelcomeScreen();
        break;
    case ScreenType::DASHBOARD:
        current_screen_ = new DashboardScreen();
        break;
    case ScreenType::LOG:
        current_screen_ = new LogScreen();
        break;
    default:
        break;
    }

    current_screen_type_ = next_screen_type_;
}