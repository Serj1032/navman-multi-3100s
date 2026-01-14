#pragma once

#include "drawable.h"


class Widget : public Drawable {
public:
    Widget() : Widget(0, 0) {}

    Widget(uint16_t x, uint16_t y) : Drawable(x, y) {}

    ~Widget() = default;

    virtual void update() = 0;
};