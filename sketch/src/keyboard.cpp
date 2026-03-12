#include "keyboard.h"

#include <Arduino.h>

#define LOG_TAG "KBRD"
#include "logger.h"

Keyboard::Keyboard() {
    memset(button_states, 0, sizeof(button_states));
    memset(button_events, 0, sizeof(button_events));
    memset(event_handlers, 0, sizeof(event_handlers));
    button_events_head = 0;
    button_events_tail = 0;
}

void Keyboard::init() {
    init_pin(0, BUTTON_1_PIN, BUTTON_1_PULL);
    init_pin(1, BUTTON_2_PIN, BUTTON_2_PULL);
    init_pin(2, BUTTON_3_PIN, BUTTON_3_PULL);
    init_pin(3, BUTTON_4_PIN, BUTTON_4_PULL);
}

void Keyboard::process() {
    ButtonEvent event;

    for (uint8_t i = 0; i < BUTTONS_AMOUNT; i++) {
        button_update(i);
    }

    pull_event(event);

    if (event.event_type != ButtonEventType::NONE) {
        LOG_INFO("Button " + String(event.button_index + 1) + ", Event: " + String(static_cast<int>(event.event_type)));
        for (uint8_t i = 0; i < BUTTONS_EVENT_MAX_HANDLERS; i++) {
            if (event_handlers[i].callback != nullptr) {
                event_handlers[i].callback(event.button_index, event.event_type, event_handlers[i].context);
            }
        }
    }
}

void Keyboard::subscribe(ButtonEventCallback callback, void* context) {
    LOG_DEBUG("Keyboard: subscribe");
    for (uint8_t i = 0; i < BUTTONS_EVENT_MAX_HANDLERS; i++) {
        if (event_handlers[i].callback == nullptr) {
            event_handlers[i].callback = callback;
            event_handlers[i].context = context;
            break;
        }
    }
}

void Keyboard::unsubscribe(ButtonEventCallback callback, void* context) {
    LOG_DEBUG("Keyboard: unsubscribe");
    for (uint8_t i = 0; i < BUTTONS_EVENT_MAX_HANDLERS; i++) {
        if (event_handlers[i].callback == callback && event_handlers[i].context == context) {
            event_handlers[i].callback = nullptr;
            event_handlers[i].context = nullptr;
            break;
        }
    }
}

void Keyboard::init_pin(uint8_t button_index, int pin, int pull) {
    button_states[button_index].pin = pin;
    button_states[button_index].pull = pull;
    button_states[button_index].pressed = false;
    button_states[button_index].last_change_time = 0;

    pinMode(pin, pull);
}

void Keyboard::button_update(uint8_t button_index) {
    bool current_state = digitalRead(button_states[button_index].pin) == LOW;
    unsigned long current_time = millis();

    if (current_state != button_states[button_index].pressed) {
        unsigned long last_time = button_states[button_index].last_change_time;
        bool last_state = button_states[button_index].pressed;

        if (current_state == false && last_state == true) {
            unsigned long duration = current_time - last_time;
            ButtonEventType event_type = ButtonEventType::NONE;

            if (duration >= BUTTON_LONG_PRESS_DURATION_MS) {
                event_type = ButtonEventType::LONG;
            } else if (duration >= BUTTON_SHORT_PRESS_DURATION_MS) {
                event_type = ButtonEventType::SHORT;
            }

            if (event_type != ButtonEventType::NONE) {
                push_event(button_index, event_type);
            }
        }

        button_states[button_index].pressed = current_state;
        button_states[button_index].last_change_time = current_time;
    }
}

void Keyboard::push_event(uint8_t button_index, ButtonEventType event_type) {
    button_events[button_events_head].button_index = button_index;
    button_events[button_events_head].event_type = event_type;
    button_events_head++;
    if (button_events_head >= BUTTONS_EVENTS_QUEUE) {
        button_events_head = 0;
    }
    if (button_events_head == button_events_tail) {
        button_events_tail++;
        if (button_events_tail >= BUTTONS_EVENTS_QUEUE) {
            button_events_tail = 0;
        }
    }
}

void Keyboard::pull_event(ButtonEvent& event) {
    if (button_events_head != button_events_tail) {
        event = button_events[button_events_tail];
        button_events_tail++;
        if (button_events_tail >= BUTTONS_EVENTS_QUEUE) {
            button_events_tail = 0;
        }
    } else {
        event.event_type = ButtonEventType::NONE;
    }
}
