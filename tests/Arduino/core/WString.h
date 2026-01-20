#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Helper functions for number to string conversion
namespace {
    inline char* utoa(unsigned int value, char* buffer, int base) {
        if (base == 10) {
            snprintf(buffer, 17, "%u", value);
        } else if (base == 16) {
            snprintf(buffer, 17, "%x", value);
        } else if (base == 8) {
            snprintf(buffer, 17, "%o", value);
        } else if (base == 2) {
            char* ptr = buffer;
            if (value == 0) {
                *ptr++ = '0';
            } else {
                char temp[33];
                int i = 0;
                while (value > 0) {
                    temp[i++] = '0' + (value & 1);
                    value >>= 1;
                }
                while (i > 0) {
                    *ptr++ = temp[--i];
                }
            }
            *ptr = '\0';
        } else {
            snprintf(buffer, 17, "%u", value);
        }
        return buffer;
    }
    
    inline char* itoa(int value, char* buffer, int base) {
        if (base == 10) {
            snprintf(buffer, 18, "%d", value);
        } else if (base == 16) {
            snprintf(buffer, 18, "%x", value);
        } else if (base == 8) {
            snprintf(buffer, 18, "%o", value);
        } else {
            snprintf(buffer, 18, "%d", value);
        }
        return buffer;
    }
    
    inline char* ltoa(long value, char* buffer, int base) {
        if (base == 10) {
            snprintf(buffer, 34, "%ld", value);
        } else if (base == 16) {
            snprintf(buffer, 34, "%lx", value);
        } else if (base == 8) {
            snprintf(buffer, 34, "%lo", value);
        } else {
            snprintf(buffer, 34, "%ld", value);
        }
        return buffer;
    }
    
    inline char* ultoa(unsigned long value, char* buffer, int base) {
        if (base == 10) {
            snprintf(buffer, 33, "%lu", value);
        } else if (base == 16) {
            snprintf(buffer, 33, "%lx", value);
        } else if (base == 8) {
            snprintf(buffer, 33, "%lo", value);
        } else {
            snprintf(buffer, 33, "%lu", value);
        }
        return buffer;
    }
}

// Mock Arduino String class for host testing
class String {
public:
    // Constructors
    String(const char *cstr = "") {
        if (cstr) {
            len_ = strlen(cstr);
            capacity_ = len_ + 1;
            buffer_ = (char*)malloc(capacity_);
            if (buffer_) {
                strcpy(buffer_, cstr);
            }
        } else {
            init();
        }
    }
    
    String(const String &str) {
        init();
        *this = str;
    }
    
    String(String &&str) noexcept {
        buffer_ = str.buffer_;
        len_ = str.len_;
        capacity_ = str.capacity_;
        str.buffer_ = nullptr;
        str.len_ = 0;
        str.capacity_ = 0;
    }
    
    explicit String(char c) {
        init();
        char temp[2] = {c, '\0'};
        *this = temp;
    }
    
    explicit String(unsigned char value, unsigned char base = 10) {
        init();
        char buf[9];
        utoa(value, buf, base);
        *this = buf;
    }
    
    explicit String(int value, unsigned char base = 10) {
        init();
        char buf[18];
        itoa(value, buf, base);
        *this = buf;
    }
    
    explicit String(unsigned int value, unsigned char base = 10) {
        init();
        char buf[17];
        utoa(value, buf, base);
        *this = buf;
    }
    
    explicit String(long value, unsigned char base = 10) {
        init();
        char buf[34];
        ltoa(value, buf, base);
        *this = buf;
    }
    
    explicit String(unsigned long value, unsigned char base = 10) {
        init();
        char buf[33];
        ultoa(value, buf, base);
        *this = buf;
    }
    
    explicit String(float value, unsigned char decimalPlaces = 2) {
        init();
        char buf[33];
        snprintf(buf, sizeof(buf), "%.*f", decimalPlaces, value);
        *this = buf;
    }
    
    explicit String(double value, unsigned char decimalPlaces = 2) {
        init();
        char buf[33];
        snprintf(buf, sizeof(buf), "%.*f", decimalPlaces, value);
        *this = buf;
    }
    
    ~String() {
        if (buffer_) free(buffer_);
    }
    
    // Assignment operators
    String& operator=(const String &rhs) {
        if (this == &rhs) return *this;
        if (rhs.buffer_) {
            copy(rhs.buffer_, rhs.len_);
        } else {
            invalidate();
        }
        return *this;
    }
    
    String& operator=(String &&rhs) noexcept {
        if (this != &rhs) {
            if (buffer_) free(buffer_);
            buffer_ = rhs.buffer_;
            len_ = rhs.len_;
            capacity_ = rhs.capacity_;
            rhs.buffer_ = nullptr;
            rhs.len_ = 0;
            rhs.capacity_ = 0;
        }
        return *this;
    }
    
    String& operator=(const char *cstr) {
        if (cstr) {
            copy(cstr, strlen(cstr));
        } else {
            invalidate();
        }
        return *this;
    }
    
    // Comparison operators
    bool operator==(const String &rhs) const {
        return equals(rhs);
    }
    
    bool operator!=(const String &rhs) const {
        return !equals(rhs);
    }
    
    bool operator<(const String &rhs) const {
        return compareTo(rhs) < 0;
    }
    
    bool operator>(const String &rhs) const {
        return compareTo(rhs) > 0;
    }
    
    bool operator<=(const String &rhs) const {
        return compareTo(rhs) <= 0;
    }
    
    bool operator>=(const String &rhs) const {
        return compareTo(rhs) >= 0;
    }
    
    bool equals(const String &s) const {
        if (len_ != s.len_) return false;
        if (len_ == 0) return true;
        return strcmp(buffer_, s.buffer_) == 0;
    }
    
    bool equals(const char *cstr) const {
        if (len_ == 0) return (cstr == nullptr || cstr[0] == '\0');
        if (cstr == nullptr) return false;
        return strcmp(buffer_, cstr) == 0;
    }
    
    // Concatenation
    String& operator+=(const String &rhs) {
        concat(rhs);
        return *this;
    }
    
    String& operator+=(const char *cstr) {
        concat(cstr);
        return *this;
    }
    
    String& operator+=(char c) {
        concat(c);
        return *this;
    }
    
    friend String operator+(const String &lhs, const String &rhs) {
        String result = lhs;
        result += rhs;
        return result;
    }
    
    // Access
    char charAt(unsigned int index) const {
        if (index >= len_) return 0;
        return buffer_[index];
    }
    
    char operator[](unsigned int index) const {
        return charAt(index);
    }
    
    char& operator[](unsigned int index) {
        static char dummy = 0;
        if (index >= len_) return dummy;
        return buffer_[index];
    }
    
    // Properties
    unsigned int length() const { return len_; }
    const char* c_str() const { return buffer_ ? buffer_ : ""; }
    
    // Modification
    void concat(const String &str) {
        concat(str.buffer_, str.len_);
    }
    
    void concat(const char *cstr) {
        if (!cstr) return;
        concat(cstr, strlen(cstr));
    }
    
    void concat(char c) {
        char buf[2] = {c, '\0'};
        concat(buf, 1);
    }
    
    void clear() {
        if (buffer_) {
            free(buffer_);
        }
        init();
    }
    
    int compareTo(const String &s) const {
        if (!buffer_) {
            return s.buffer_ ? -1 : 0;
        }
        if (!s.buffer_) {
            return 1;
        }
        return strcmp(buffer_, s.buffer_);
    }
    
private:
    char *buffer_;
    unsigned int len_;
    unsigned int capacity_;
    
    void init() {
        buffer_ = nullptr;
        len_ = 0;
        capacity_ = 0;
    }
    
    void invalidate() {
        if (buffer_) {
            free(buffer_);
        }
        init();
    }
    
    bool reserve(unsigned int size) {
        if (buffer_ && capacity_ >= size) return true;
        char *newbuffer = (char*)realloc(buffer_, size);
        if (newbuffer) {
            buffer_ = newbuffer;
            capacity_ = size;
            return true;
        }
        return false;
    }
    
    void copy(const char *cstr, unsigned int length) {
        if (!reserve(length + 1)) {
            invalidate();
            return;
        }
        len_ = length;
        strcpy(buffer_, cstr);
    }
    
    void concat(const char *cstr, unsigned int length) {
        if (length == 0) return;
        unsigned int newlen = len_ + length;
        if (!reserve(newlen + 1)) return;
        strcpy(buffer_ + len_, cstr);
        len_ = newlen;
    }
};
