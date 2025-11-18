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