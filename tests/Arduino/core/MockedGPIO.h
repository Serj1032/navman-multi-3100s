#pragma once

#include <stdint.h>

/**
 * MockedGPIO — simple pin state table for host-based testing.
 *
 * Tracks digital pin values so that digitalRead() can return
 * values set by external sources (e.g. SDL keyboard events).
 *
 * INPUT_PULLUP pins default to HIGH (not pressed).
 * Setting a pin LOW simulates a button press.
 */
namespace MockedGPIO {

// Maximum number of GPIO pins to track (ATmega2560 has pins 0-69)
static constexpr uint8_t MAX_PINS = 70;

/**
 * Set the digital value of a pin.
 * @param pin  Pin number
 * @param val  HIGH or LOW
 */
void set_pin_value(uint8_t pin, uint8_t val);

/**
 * Get the current digital value of a pin.
 * Returns HIGH by default (INPUT_PULLUP behavior).
 */
uint8_t get_pin_value(uint8_t pin);

/**
 * Map an SDL keyboard scancode to a GPIO pin number.
 * Returns the pin number, or 0xFF if no mapping exists.
 *
 * Mapping:
 *   Keyboard '1' → BUTTON_1_PIN (22)
 *   Keyboard '2' → BUTTON_2_PIN (24)
 *   Keyboard '3' → BUTTON_3_PIN (26)
 *   Keyboard '4' → BUTTON_4_PIN (28)
 */
uint8_t sdl_key_to_pin(uint32_t sdl_keycode);

} // namespace MockedGPIO
