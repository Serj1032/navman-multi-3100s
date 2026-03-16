#pragma once

#include <Arduino.h>

#include "display/drawable.h"

class Text : public Drawable {
public:
    Text();
    Text(const char *text, int x, int y, uint8_t font_size);
    ~Text() = default;

    void draw_content(Display &display) override;
    void clear_content(Display &display) override;

    const String &get_text() const;
    void set_text(const String &text);

    uint16_t width() const;
    uint16_t height() const;

private:
    String text_;
    String new_text_;
    uint8_t font_size_;
};