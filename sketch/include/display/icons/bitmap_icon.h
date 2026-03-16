#pragma once

#include "display/drawable.h"

class BitmapIcon : public Drawable {
public:
    BitmapIcon() : BitmapIcon(0, 0, nullptr, 0, 0) {}
    BitmapIcon(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h);
    ~BitmapIcon() override = default;

    BitmapIcon(const BitmapIcon &other)
        : Drawable(other.x_, other.y_)
        , bitmap_(other.bitmap_)
        , w_(other.w_)
        , h_(other.h_)
    {
        set_color(other.color_);
    }

    BitmapIcon& operator=(const BitmapIcon &other);

    uint16_t width() const { return w_; }
    uint16_t height() const { return h_; }

    void draw_content(Display &display) override;
    void clear_content(Display &display) override;

private:
    const uint8_t *bitmap_;
    uint16_t w_;
    uint16_t h_;
};
