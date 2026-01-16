#pragma once 

#include <Arduino.h>

#ifdef __DEBUG__

#define LOG_DEBUG(msg) Logger::get_instance().log_debug(msg)
#define LOG_INFO(msg) Logger::get_instance().log_info(msg)
#define LOG_WARN(msg) Logger::get_instance().log_warn(msg)
#define LOG_ERROR(msg) Logger::get_instance().log_error(msg)

class Logger {
public:
    enum class LogLevel : uint8_t {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR
    };

    static Logger& get_instance() {
        static Logger instance;
        return instance;
    }

    void log(const String& message, LogLevel level = LogLevel::INFO);

    void log_debug(const String& message) {
        log(message, LogLevel::DEBUG);
    }

    void log_info(const String& message) {
        log(message, LogLevel::INFO);
    }

    void log_warn(const String& message) {
        log(message, LogLevel::WARN);
    }

    void log_error(const String& message) {
        log(message, LogLevel::ERROR);
    }

private:
    Logger();
    ~Logger() = default;

private:
    LogLevel level_ = LogLevel::INFO;
};
#else
#define LOG_DEBUG(msg)
#define LOG_INFO(msg)
#define LOG_WARN(msg)
#define LOG_ERROR(msg)
#endif
