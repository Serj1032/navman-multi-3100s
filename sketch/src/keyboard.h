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

    void init();
    void process();
    void subscribe(ButtonEventCallback callback, void* context);
    void unsubscribe(ButtonEventCallback callback, void* context);

private:
    Keyboard();
    
    void init_pin(uint8_t button_index, int pin, int pull);
    
    void button_update(uint8_t button_index);
    void push_event(uint8_t button_index, ButtonEventType event_type);
    void pull_event(ButtonEvent& event);

private:
    uint16_t button_events_head;
    uint16_t button_events_tail;
    ButtonEvent button_events[BUTTONS_EVENTS_QUEUE];
    ButtonState button_states[BUTTONS_AMOUNT];
    ButtonEventHandler event_handlers[BUTTONS_EVENT_MAX_HANDLERS];
};

#endif // KEYBOARD_H