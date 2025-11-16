#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"

class Cli;

enum class ButtonEventType : uint8_t {
    NONE = 0,
    SHORT,
    LONG
};

typedef void (*ButtonEventCallback)(uint8_t button_index, ButtonEventType event_type, void* context);

struct ButtonState {
    int pin;
    int pull;
    bool pressed;
    unsigned long last_change_time;
};

struct ButtonEvent {
    uint8_t button_index;
    ButtonEventType event_type;
};

struct ButtonEventHandler {
    ButtonEventCallback callback;
    void* context;
};

class Keyboard {
    friend class Cli;
public:
    ~Keyboard() = default;

    static Keyboard& get_instance() {
        static Keyboard instance;
        return instance;
    }

    void init() {
        // init_pin(0, BUTTON_1_PIN, BUTTON_1_PULL, button_1_irq_handler);
        // init_pin(1, BUTTON_2_PIN, BUTTON_2_PULL, button_2_irq_handler);
        // init_pin(2, BUTTON_3_PIN, BUTTON_3_PULL, button_3_irq_handler);
        // init_pin(3, BUTTON_4_PIN, BUTTON_4_PULL, button_4_irq_handler);
    }

    void process() {
        ButtonEvent event;
        noInterrupts();
        pull_event(event);
        interrupts();

        if (event.event_type != ButtonEventType::NONE) {
            for (uint8_t i = 0; i < BUTTONS_EVENT_MAX_HANDLERS; i++) {
                if (event_handlers[i].callback != nullptr) {
                    event_handlers[i].callback(event.button_index, event.event_type, event_handlers[i].context);
                }
            }
        }
    }

    void subscribe(ButtonEventCallback callback, void* context) {
        // Serial.println(F("Keyboard: subscribe"));
        for (uint8_t i = 0; i < BUTTONS_EVENT_MAX_HANDLERS; i++) {
            if (event_handlers[i].callback == nullptr) {
                event_handlers[i].callback = callback;
                event_handlers[i].context = context;
                break;
            }
        }
    }

    void unsubscribe(ButtonEventCallback callback, void* context) {
        // Serial.println(F("Keyboard: unsubscribe"));
        for (uint8_t i = 0; i < BUTTONS_EVENT_MAX_HANDLERS; i++) {
            if (event_handlers[i].callback == callback && event_handlers[i].context == context) {
                event_handlers[i].callback = nullptr;
                event_handlers[i].context = nullptr;
                break;
            }
        }
    }

private:
    Keyboard() {
        memset(button_states, 0, sizeof(button_states));
        memset(button_events, 0, sizeof(button_events));
        memset(event_handlers, 0, sizeof(event_handlers));
    }

    void init_pin(uint8_t button_index, int pin, int pull, void (*irq_handler)()) {
        button_states[button_index].pin = pin;
        button_states[button_index].pull = pull;
        button_states[button_index].pressed = false;
        button_states[button_index].last_change_time = 0;

        pinMode(pin, pull);
        attachInterrupt(digitalPinToInterrupt(pin), irq_handler, CHANGE);
    }

    static void button_1_irq_handler() {
        get_instance().button_irq_handler(0);
    }

    static void button_2_irq_handler() {
        get_instance().button_irq_handler(1);
    }

    static void button_3_irq_handler() {
        get_instance().button_irq_handler(2);
    }

    static void button_4_irq_handler() {
        get_instance().button_irq_handler(3);
    }

    void button_irq_handler(uint8_t button_index) {
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

    void push_event(uint8_t button_index, ButtonEventType event_type) {
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

    void pull_event(ButtonEvent& event) {
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

private:
    uint16_t button_events_head = 0;
    uint16_t button_events_tail = 0;
    ButtonEvent button_events[BUTTONS_EVENTS_QUEUE];
    ButtonState button_states[BUTTONS_AMOUNT];
    ButtonEventHandler event_handlers[BUTTONS_EVENT_MAX_HANDLERS];
};

#endif // KEYBOARD_H