#pragma once 

#include <Arduino.h>

#ifdef __DEBUG__

#ifndef LOG_TAG
#define LOG_TAG "-"
#endif

// Log level integer constants — safe to use in preprocessor #if comparisons
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(msg) Logger::get_instance().log_debug("(" LOG_TAG ") " msg)
#else 
#define LOG_DEBUG(msg)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(msg) Logger::get_instance().log_info("(" LOG_TAG ") " msg)
#else
#define LOG_INFO(msg)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(msg) Logger::get_instance().log_warn("(" LOG_TAG ") " msg)
#else
#define LOG_WARN(msg)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(msg) Logger::get_instance().log_error("(" LOG_TAG ") " msg)
#else
#define LOG_ERROR(msg)
#endif

using LogListenerCallback = void(*)(const char* message, void* context);

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

    void set_listener(LogListenerCallback callback, void* context = nullptr) {
        listener_ = callback;
        listener_context_ = context;
    }

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

    LogListenerCallback listener_ = nullptr;
    void* listener_context_ = nullptr;
};
#else
#define LOG_DEBUG(msg)
#define LOG_INFO(msg)
#define LOG_WARN(msg)
#define LOG_ERROR(msg)
#endif
