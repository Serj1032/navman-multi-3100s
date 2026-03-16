#include "MockedGPIO.h"
#include "Arduino.h"

namespace MockedGPIO {

// Pin state table — defaults to HIGH (INPUT_PULLUP not pressed)
static uint8_t pin_values[MAX_PINS];
static bool initialized = false;

static void ensure_initialized() {
    if (!initialized) {
        for (uint8_t i = 0; i < MAX_PINS; i++) {
            pin_values[i] = HIGH;
        }
        initialized = true;
    }
}

void set_pin_value(uint8_t pin, uint8_t val) {
    ensure_initialized();
    if (pin < MAX_PINS) {
        pin_values[pin] = val;
    }
}

uint8_t get_pin_value(uint8_t pin) {
    ensure_initialized();
    if (pin < MAX_PINS) {
        return pin_values[pin];
    }
    return HIGH;
}

uint8_t sdl_key_to_pin(uint32_t sdl_keycode) {
    // SDL keycodes for number keys '1' through '4'
    // SDLK_1 = '1' = 0x31, SDLK_2 = '2' = 0x32, etc.
    switch (sdl_keycode) {
        case '1': return 22;  // BUTTON_1_PIN
        case '2': return 24;  // BUTTON_2_PIN
        case '3': return 26;  // BUTTON_3_PIN
        case '4': return 28;  // BUTTON_4_PIN
        default:  return 0xFF;
    }
}

} // namespace MockedGPIO
