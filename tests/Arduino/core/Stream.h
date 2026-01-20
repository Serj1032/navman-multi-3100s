#pragma once

#include "Arduino.h"

// Mock Stream base class
class Stream {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() { }
    
    // Print methods (inherited by Stream classes)
    size_t print(const char *str) {
        size_t n = 0;
        while (*str) {
            n += write(*str++);
        }
        return n;
    }
    
    size_t print(char c) {
        return write(c);
    }
    
    size_t print(int n, int base = 10) {
        char buf[32];
        if (base == 10) {
            snprintf(buf, sizeof(buf), "%d", n);
        } else if (base == 16) {
            snprintf(buf, sizeof(buf), "%x", n);
        } else {
            snprintf(buf, sizeof(buf), "%d", n);
        }
        return print(buf);
    }
    
    size_t println(const char *str) {
        size_t n = print(str);
        n += print("\n");
        return n;
    }
    
    size_t println() {
        return print("\n");
    }
    
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) {
        size_t n = 0;
        while (size--) {
            n += write(*buffer++);
        }
        return n;
    }
};
