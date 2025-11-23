#include "display/screen.h"
#include "display/screen_manager.h"


WelcomeScreen::WelcomeScreen() : 
    Screen(), 
    line1_("NAVA", 140, 80, 8),
    line2_("Sailing", 110, 180, 6),
    author_("Author: Sergei V. Kravchenko", 10, 300, 2)
{
    ts_ = millis();
}
                    
void WelcomeScreen::draw_screen(Display &display)
{
    line1_.draw(display);
    line2_.draw(display);
    author_.draw(display);
}

void WelcomeScreen::clear_screen(Display &display)
{
    line1_.clear_content(display);
    line2_.clear_content(display);
    author_.clear_content(display);
}

void WelcomeScreen::update()
{
    uint32_t now = millis();
    if (now - ts_ > 4000) {
        ScreenManager::get_instance().set_next_screen(ScreenType::DASHBOARD);
    }
}

void WelcomeScreen::key_press_handler(uint8_t button_index, ButtonEventType event_type)
{
    // No key handling for welcome screen
}
