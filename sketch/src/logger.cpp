#include "logger.h"

Logger::Logger() {
    Serial.begin(9600);
}

void Logger::log(const String& message, LogLevel level) {
    const char* level_str = "INFO";
    switch (level) {
        case LogLevel::DEBUG: level_str = "DEBUG"; break;
        case LogLevel::INFO:  level_str = "INFO";  break;
        case LogLevel::WARN:  level_str = "WARN";  break;
        case LogLevel::ERROR: level_str = "ERROR"; break;
    }

    Serial.print("[");
    Serial.print(level_str);
    Serial.print("] ");
    Serial.println(message);

    if (listener_) {
        String full = String("[") + level_str + "] " + message;
        listener_(full.c_str(), listener_context_);
    }
}
