#pragma once

#include "drawable.h"

class Button : public Drawable
{
public:
    Button() : Drawable(), width_(0), height_(0) {}

    Button(int16_t x, int16_t y, int16_t width, int16_t height)
        : Drawable(x, y), width_(width), height_(height)
    {
        set_color(ColorScheme::get_instance().button_color());
        set_visible(false);
    }

    ~Button() override = default;

    void set_icon(const BitmapIcon &icon) {
        set_visible(true);
        icon_ = icon;
        icon_.set_parent(this);
        icon_.set_color(ColorScheme::get_instance().default_icon_color());
        icon_.set_position(x_ + (width_ - icon_.width()) / 2, y_ + (height_ - icon_.height()) / 2);
        mark_dirty();

        content_ = &icon_;
    }

    void set_label(const Text &label) {
        set_visible(true);
        label_ = label;
        label_.set_parent(this);
        label_.set_color(ColorScheme::get_instance().text_color());
        label_.set_position(x_ + (width_ - label_.width()) / 2, y_ + (height_ - label_.height()) / 2);
        mark_dirty();

        content_ = &label_;
    }

    void draw_content(Display &display) override
    {
        if (!initialized_ && !is_visible_) {
            initialized_ = true;
            return;
        }

        uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
        uint16_t color = is_visible_ ? color_ : bg_color;
        display.draw_round_rect(x_, y_, width_, height_, radius_, color);

        if (content_ != nullptr) {
            content_->draw(display);
        }
    }

    void clear_content(Display &display) override
    {
        if (content_ != nullptr) {
            content_->clear_content(display);
        }

        uint16_t bg_color = (parent_ != nullptr) ? parent_->color() : ColorScheme::get_instance().background_color();
        display.draw_round_rect(x_, y_, width_, height_, radius_, bg_color);
    }

private:
    bool initialized_ = false;

    int16_t width_;
    int16_t height_;
    uint16_t radius_ = 8;
    Drawable* content_ = nullptr;
    BitmapIcon icon_;
    Text label_;
};