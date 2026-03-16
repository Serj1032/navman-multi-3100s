#include "display/text.h"
#include "display/color_scheme.h"

Text::Text() : Text("", 0, 0, 1)
{
}

Text::Text(const char *text, int x, int y, uint8_t font_size)
    : Drawable(x, y)
    , text_(text)
    , new_text_(text)
    , font_size_(font_size)
{
    set_color(ColorScheme::get_instance().text_color());
}

void Text::draw_content(Display &display)
{
    uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
    uint16_t color = is_visible_ ? color_ : bg_color;

    display.draw_text(x_, y_, text_.c_str(), font_size_, bg_color);
    text_ = new_text_;
    display.draw_text(x_, y_, text_.c_str(), font_size_, color);
}

void Text::clear_content(Display &display)
{
    uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
    display.draw_text(x_, y_, text_.c_str(), font_size_, bg_color);
}

const String &Text::get_text() const
{
    return new_text_;
}

void Text::set_text(const String &text)
{
    if (text_ == text) {
        return;
    }
    new_text_ = text;
    mark_dirty();
}

uint16_t Text::width() const
{
    return text_.length() * 6 * font_size_;
}

uint16_t Text::height() const
{
    return 8 * font_size_;
}
