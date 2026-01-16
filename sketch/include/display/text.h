#ifndef TEXT_H
#define TEXT_H

#include <Arduino.h>

#include "drawable.h"
#include "display/color_scheme.h"

class Text : public Drawable
{
public:
    Text() : Text("", 0, 0, 1)
    {
    }

    Text(const char *text, int x, int y, uint8_t font_size) : 
        Drawable(x, y), 
        text_(text), 
        new_text_(text), 
        font_size_(font_size)
    {
        set_color(ColorScheme::get_instance().text_color());
    }
    
    ~Text() = default;

    void draw_content(Display &display) override
    {
        uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
        uint16_t color = is_visible_ ? color_ : bg_color;

        display.draw_text(x_, y_, text_.c_str(), font_size_, bg_color);
        text_ = new_text_;
        display.draw_text(x_, y_, text_.c_str(), font_size_, color);
    }

    void clear_content(Display &display) override
    {
        uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
        display.draw_text(x_, y_, text_.c_str(), font_size_, bg_color);
    }

    const String &get_text() const
    {
        return text_;
    }

    void set_text(const String &text)
    {
        if (text_ == text)
        {
            return;
        }
        new_text_ = text;
        mark_dirty();
    }

    uint16_t width() const
    {
        return text_.length() * 6 * font_size_;
    }

    uint16_t height() const
    {
        return 8 * font_size_;
    }

private:
    String text_;
    String new_text_;
    uint8_t font_size_;
};

#endif // TEXT_H