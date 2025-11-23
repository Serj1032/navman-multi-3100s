#pragma once

#include "display.h"

class Drawable
{
public:
    Drawable() : x_(0), y_(0) {}

    Drawable(uint16_t x, uint16_t y) : x_(x), y_(y) {}

    virtual ~Drawable() = default;

    void set_position(uint16_t x, uint16_t y)
    {
        if (x_ == x && y_ == y)
        {
            return;
        }
        x_ = x;
        y_ = y;
        mark_dirty();
    }

    uint16_t x() const
    {
        return x_;
    }

    uint16_t y() const
    {
        return y_;
    }

    void set_visible(bool visible)
    {
        if (is_visible_ == visible)
        {
            return;
        }
        is_visible_ = visible;
        mark_dirty();
    }

    bool is_visible() const
    {
        return is_visible_;
    }

    void set_color(uint16_t color)
    {
        if (color_ == color)
        {
            return;
        }
        color_ = color;
        mark_dirty();
    }

    virtual uint16_t color() const
    {
        return color_;
    }

    bool is_dirty() const
    {
        return is_dirty_;
    }

    void mark_dirty()
    {
        Drawable *drawable = this;
        while (drawable->parent_ != nullptr)
        {
            drawable = drawable->parent_;
        }

        while (drawable != nullptr)
        {
            drawable->is_dirty_ = true;
            drawable = drawable->child_;
        }
    }

    void draw(Display &display)
    {
        if (is_dirty_)
        {
            draw_content(display);
            is_dirty_ = false;
        }
    }

    void set_parent(Drawable *parent)
    {
        parent_ = parent;
        parent_->child_ = this;
    }

    void set_child(Drawable *child)
    {
        child_ = child;
        child_->parent_ = this;
    }

    virtual void clear_content(Display &display) = 0;

protected:
    virtual void draw_content(Display &display) = 0;

protected:
    uint16_t x_;
    uint16_t y_;
    Drawable *parent_ = nullptr;
    Drawable *child_ = nullptr;
    bool is_dirty_ = true;
    bool is_visible_ = true;
    uint16_t color_;
};
