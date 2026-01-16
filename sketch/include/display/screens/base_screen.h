#pragma once

#include "display/display.h"
#include "keyboard.h"

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
    
    virtual void update() = 0;

    void draw(Display &display)
    {
        uint32_t now = millis();
        if (now - last_draw_time_ < (1000 / fps_))
        {
            return;
        }
        
        draw_screen(display);
        last_draw_time_ = millis();
    }
    
    void clear(Display &display) {
        clear_screen(display);
    }
    
protected:
    virtual void draw_screen(Display &display) = 0;
    virtual void clear_screen(Display &display) = 0;
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
