#pragma once

#include <Arduino.h>

static String padStart(const String &str, size_t total_length, char pad_char) {
    if (str.length() >= total_length) {
        return str;
    }
    String padded_str = "";
    for (size_t i = 0; i < total_length - str.length(); ++i) {
        padded_str += pad_char;
    }
    padded_str += str;
    return padded_str;
}

static void rotate_by_angle(int16_t x0, int16_t y0, int16_t& x, int16_t& y, float angle) {
    int16_t orig_x = x;
    int16_t orig_y = y;

    float rad = angle * PI / 180.0f;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    x = static_cast<int16_t>(x0 + (orig_x - x0) * cos_a - (orig_y - y0) * sin_a);
    y = static_cast<int16_t>(y0 + (orig_x - x0) * sin_a + (orig_y - y0) * cos_a);
}
