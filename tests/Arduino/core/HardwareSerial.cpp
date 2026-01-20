#include "HardwareSerial.h"
#include "WString.h"

// Implement String-dependent methods
size_t HardwareSerial::print(const String &str) {
    fputs(str.c_str(), stdout);
    return str.length();
}

size_t HardwareSerial::println(const String &str) {
    size_t n = print(str);
    n += print("\n");
    return n;
}

// Global Serial instances
HardwareSerial Serial;
HardwareSerial Serial1;
HardwareSerial Serial2;
HardwareSerial Serial3;
