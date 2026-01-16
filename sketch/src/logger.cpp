#include "logger.h"

Logger::Logger() {
    Serial.begin(9600);
}

void Logger::log(const String& message, LogLevel level) {
    Serial.print("[");
    switch (level) {
        case LogLevel::DEBUG:
            Serial.print("DEBUG");
            break;
        case LogLevel::INFO:
            Serial.print("INFO");
            break;
        case LogLevel::WARN:
            Serial.print("WARN");
            break;
        case LogLevel::ERROR:
            Serial.print("ERROR");
            break;  
    }
    Serial.print("] ");
    Serial.println(message);
}
