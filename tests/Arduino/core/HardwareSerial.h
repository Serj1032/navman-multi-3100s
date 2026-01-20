#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration
class String;

// Mock Serial class for host testing
class HardwareSerial {
public:
    void begin(unsigned long baud) {
        (void)baud;
        // Mock - does nothing
    }
    
    void end() {
        // Mock - does nothing
    }
    
    int available() {
        return 0; // Mock - no data available
    }
    
    int read() {
        return -1; // Mock - no data
    }
    
    int peek() {
        return -1; // Mock - no data
    }
    
    void flush() {
        fflush(stdout);
    }
    
    size_t write(uint8_t c) {
        return putchar(c) != EOF ? 1 : 0;
    }
    
    size_t write(const uint8_t *buffer, size_t size) {
        return fwrite(buffer, 1, size, stdout);
    }
    
    size_t print(const char *str) {
        fputs(str, stdout);
        return strlen(str);
    }
    
    size_t print(const String &str);  // Defined after String is complete
    
    size_t print(char c) {
        putchar(c);
        return 1;
    }
    
    size_t print(int n, int base = 10) {
        char buf[32];
        if (base == 10) {
            snprintf(buf, sizeof(buf), "%d", n);
        } else if (base == 16) {
            snprintf(buf, sizeof(buf), "%x", n);
        } else if (base == 8) {
            snprintf(buf, sizeof(buf), "%o", n);
        } else {
            snprintf(buf, sizeof(buf), "%d", n);
        }
        return print(buf);
    }
    
    size_t print(unsigned int n, int base = 10) {
        char buf[32];
        if (base == 10) {
            snprintf(buf, sizeof(buf), "%u", n);
        } else if (base == 16) {
            snprintf(buf, sizeof(buf), "%x", n);
        } else if (base == 8) {
            snprintf(buf, sizeof(buf), "%o", n);
        } else {
            snprintf(buf, sizeof(buf), "%u", n);
        }
        return print(buf);
    }
    
    size_t print(long n, int base = 10) {
        char buf[32];
        if (base == 10) {
            snprintf(buf, sizeof(buf), "%ld", n);
        } else if (base == 16) {
            snprintf(buf, sizeof(buf), "%lx", n);
        } else if (base == 8) {
            snprintf(buf, sizeof(buf), "%lo", n);
        } else {
            snprintf(buf, sizeof(buf), "%ld", n);
        }
        return print(buf);
    }
    
    size_t print(unsigned long n, int base = 10) {
        char buf[32];
        if (base == 10) {
            snprintf(buf, sizeof(buf), "%lu", n);
        } else if (base == 16) {
            snprintf(buf, sizeof(buf), "%lx", n);
        } else if (base == 8) {
            snprintf(buf, sizeof(buf), "%lo", n);
        } else {
            snprintf(buf, sizeof(buf), "%lu", n);
        }
        return print(buf);
    }
    
    size_t print(double n, int digits = 2) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%.*f", digits, n);
        return print(buf);
    }
    
    size_t println(const char *str) {
        size_t n = print(str);
        n += print("\n");
        return n;
    }
    
    size_t println(const String &str);  // Defined after String is complete
    
    size_t println() {
        return print("\n");
    }
    
    template<typename T>
    size_t println(T val, int base = 10) {
        size_t n = print(val, base);
        n += println();
        return n;
    }
};

extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;
