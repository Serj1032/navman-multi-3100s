#pragma once

#include "display/screens/base_screen.h"
#include "display/text.h"
#include "display/icons/bitmap_icon.h"
#include "display/assets/header.h"
#include "display/color_scheme.h"

class LogScreen : public Screen
{
public:
    LogScreen();
    ~LogScreen() override = default;

    void update() override;
    
protected:
    void draw_screen(Display &display) override;
    void clear_screen(Display &display) override;
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    Text title_text_;
    BitmapIcon header_icon_;
};
