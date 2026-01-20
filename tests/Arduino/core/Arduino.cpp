#include "Arduino.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>


// Mock implementations for host testing

static unsigned long start_time_ms = 0;
static unsigned long start_time_us = 0;

static void init_time() {
    static bool initialized = false;
    if (!initialized) {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        start_time_ms = tv.tv_sec * 1000UL + tv.tv_usec / 1000UL;
        start_time_us = tv.tv_sec * 1000000UL + tv.tv_usec;
        initialized = true;
    }
}

unsigned long millis() {
    init_time();
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    unsigned long now_ms = tv.tv_sec * 1000UL + tv.tv_usec / 1000UL;
    return now_ms - start_time_ms;
}

unsigned long micros() {
    init_time();
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    unsigned long now_us = tv.tv_sec * 1000000UL + tv.tv_usec;
    return now_us - start_time_us;
}

void delay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        // Busy wait (not ideal but simple for testing)
    }
}

void delayMicroseconds(unsigned int us) {
    unsigned long start = micros();
    while (micros() - start < us) {
        // Busy wait
    }
}

void pinMode(uint8_t pin, uint8_t mode) {
    // Mock implementation - does nothing
    (void)pin;
    (void)mode;
}

void digitalWrite(uint8_t pin, uint8_t val) {
    // Mock implementation - does nothing
    (void)pin;
    (void)val;
}

int digitalRead(uint8_t pin) {
    // Mock implementation - always returns LOW
    (void)pin;
    return LOW;
}

int analogRead(uint8_t pin) {
    // Mock implementation - returns 0
    (void)pin;
    return 0;
}

void analogWrite(uint8_t pin, int val) {
    // Mock implementation - does nothing
    (void)pin;
    (void)val;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long random(long howbig) {
    if (howbig == 0) {
        return 0;
    }
    return rand() % howbig;
}

long random(long howsmall, long howbig) {
    if (howsmall >= howbig) {
        return howsmall;
    }
    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

int main() {
    setup();
    while (true) {
        loop();
    }
    return 0;
}
