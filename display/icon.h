// https://javl.github.io/image2cpp/

#pragma once

#include "drawable.h"

class Icon : public Drawable
{
public:

    Icon() : Icon(0, 0, nullptr, 0, 0) {
    }

    Icon(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h) : Drawable(x, y), bitmap_(bitmap), w_(w), h_(h)
    {
        set_color(ColorScheme::get_instance().default_icon_color());
    }
    virtual ~Icon() = default;

    Icon(const Icon &other) : Drawable(other.x_, other.y_), bitmap_(other.bitmap_), w_(other.w_), h_(other.h_)
    {
        set_color(other.color_);
    }

    Icon& operator=(const Icon &other)
    {
        if (this != &other)
        {
            x_ = other.x_;
            y_ = other.y_;
            bitmap_ = other.bitmap_;
            w_ = other.w_;
            h_ = other.h_;
            color_ = other.color_;
        }
        return *this;
    }

    uint16_t width() const {
        return w_;
    }

    uint16_t height() const {
        return h_;
    }

    void draw_content(Display &display) override
    {
        if (bitmap_ == nullptr) {
            return;
        }
        uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
        uint16_t color = is_visible_ ? color_ : bg_color;
        display.draw_bitmap(x_, y_, bitmap_, w_, h_, color, bg_color);
    }

    void clear_content(Display& display) override {
        if (bitmap_ == nullptr) {
            return;
        }
        uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
        display.draw_bitmap(x_, y_, bitmap_, w_, h_, bg_color, bg_color);
    }

private:
    const uint8_t *bitmap_;
    uint16_t w_;
    uint16_t h_;
};
