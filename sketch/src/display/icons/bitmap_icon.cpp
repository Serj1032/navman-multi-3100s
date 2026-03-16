#include "display/icons/bitmap_icon.h"
#include "display/color_scheme.h"

BitmapIcon::BitmapIcon(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h)
    : Drawable(x, y)
    , bitmap_(bitmap)
    , w_(w)
    , h_(h)
{
    set_color(ColorScheme::get_instance().default_icon_color());
}

BitmapIcon& BitmapIcon::operator=(const BitmapIcon &other) {
    if (this != &other) {
        x_ = other.x_;
        y_ = other.y_;
        bitmap_ = other.bitmap_;
        w_ = other.w_;
        h_ = other.h_;
        color_ = other.color_;
    }
    return *this;
}

void BitmapIcon::draw_content(Display &display) {
    if (bitmap_ == nullptr) {
        return;
    }
    uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
    uint16_t color = is_visible_ ? color_ : bg_color;
    display.draw_bitmap(x_, y_, bitmap_, w_, h_, color, bg_color);
}

void BitmapIcon::clear_content(Display &display) {
    if (bitmap_ == nullptr) {
        return;
    }
    uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
    display.draw_bitmap(x_, y_, bitmap_, w_, h_, bg_color, bg_color);
}
