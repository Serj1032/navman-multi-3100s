#pragma once

#include "display/drawable.h"
#include "display/button.h"
#include "display/icons/bitmap_icon.h"

namespace {
    static const uint16_t amout = 4;
    static const uint16_t spacing = 20;
    static const uint16_t height = 30;
    static const uint16_t total_width = 480;
    static const uint16_t total_height = 320;
    static constexpr uint16_t width = (total_width - amout * spacing) / amout;
}

class Buttons : public Drawable
{
public:
    Buttons() : Drawable()
    {
        uint16_t x = spacing / 2;
        for (int i = 0; i < amout; ++i) {
            buttons[i] = Button(x, total_height - height, width, height);
            buttons[i].set_parent(this);
            buttons[i].set_color(ColorScheme::get_instance().button_color());
            x += width + spacing;
        }
    }
    ~Buttons() override = default;

    // Make buttons container invisible, so it uses parent's color
    uint16_t color() const override{
        if (parent_ != nullptr) {
            return parent_->color();
        }
        return ColorScheme::get_instance().background_color();
    }

    Button& get_button(uint8_t index) {
        return buttons[index];
    }

    void draw_content(Display& display) override
    {
        for (int i = 0; i < amout; ++i) {
            buttons[i].draw(display);
        }
    }

    void clear_content(Display& display) override
    {
        for (int i = 0; i < amout; ++i) {
            buttons[i].clear_content(display);
        }
    }

private:
    Button buttons[amout];
};
