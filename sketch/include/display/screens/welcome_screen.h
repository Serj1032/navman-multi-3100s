#pragma once

#include "display/screens/base_screen.h"
#include "display/text.h"

class WelcomeScreen : public Screen
{
public:
    WelcomeScreen();
    ~WelcomeScreen() override = default;
    
    void update() override;
    
private:
    void draw_screen(Display &display) override;
    void clear_screen(Display &display) override;
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    uint32_t ts_;

    Text line1_;
    Text line2_;
    Text author_;
};
