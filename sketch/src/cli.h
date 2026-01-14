#pragma once

#include <Arduino.h>

#include "keyboard.h"

class Cli
{
public:
    ~Cli() = default;

    static Cli& get_instance() {
        static Cli instance;
        return instance;
    }

    void init()
    {
        Serial.begin(115200);
    }

    void process()
    {
        if (Serial.available() > 0) {
            String command = Serial.readStringUntil('\n');
            if (command[0] == 'b'){
                parse_button_command(command);
            }
        }
    }

private:
    Cli() {}

    void parse_button_command(const String& command)
    {
        // Example command: "b 1 s" for button 1 short press, "b 2 l" for button 2 long press
        int button_index = command.substring(2, 3).toInt();
        char event_type_char = command.charAt(4);
        ButtonEventType event_type = ButtonEventType::NONE;
        if (event_type_char == 's') {
            event_type = ButtonEventType::SHORT;
        } else if (event_type_char == 'l') {
            event_type = ButtonEventType::LONG;
        }

        if (event_type != ButtonEventType::NONE) {
            Keyboard::get_instance().push_event(button_index, event_type);
        }
    }
};