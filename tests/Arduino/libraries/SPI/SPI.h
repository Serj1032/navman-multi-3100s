#pragma once

#include <Arduino.h>

#define SPI_HAS_TRANSACTION 1
#define SPI_HAS_NOTUSINGINTERRUPT 1
#define SPI_ATOMIC_VERSION 1

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C
#define SPI_CLOCK_MASK 0x03
#define SPI_2XCLOCK_MASK 0x01

// Mock SPISettings class
class SPISettings {
public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) 
        : clock_(clock), bitOrder_(bitOrder), dataMode_(dataMode) {}
    
    SPISettings() : clock_(4000000), bitOrder_(MSBFIRST), dataMode_(SPI_MODE0) {}
    
    uint32_t getClockFreq() const { return clock_; }
    uint8_t getBitOrder() const { return bitOrder_; }
    uint8_t getDataMode() const { return dataMode_; }
    
private:
    uint32_t clock_;
    uint8_t bitOrder_;
    uint8_t dataMode_;
    
    friend class SPIClass;
};

// Mock SPI class for host testing
class SPIClass {
public:
    SPIClass() : initialized_(false), interruptMode_(0), 
                 interruptMask_(0), interruptSave_(0) {}
    
    void begin() {
        initialized_ = true;
    }
    
    void end() {
        initialized_ = false;
    }
    
    void beginTransaction(SPISettings settings) {
        (void)settings;
        // Mock implementation
    }
    
    void endTransaction(void) {
        // Mock implementation
    }
    
    uint8_t transfer(uint8_t data) {
        // Mock implementation - echo back
        return data;
    }
    
    uint16_t transfer16(uint16_t data) {
        // Mock implementation - echo back
        return data;
    }
    
    void transfer(void *buf, size_t count) {
        // Mock implementation - data unchanged
        (void)buf;
        (void)count;
    }
    
    void setBitOrder(uint8_t bitOrder) {
        (void)bitOrder;
        // Mock implementation
    }
    
    void setDataMode(uint8_t dataMode) {
        (void)dataMode;
        // Mock implementation
    }
    
    void setClockDivider(uint8_t clockDiv) {
        (void)clockDiv;
        // Mock implementation
    }
    
    void usingInterrupt(uint8_t interruptNumber) {
        (void)interruptNumber;
        // Mock implementation
    }
    
    void notUsingInterrupt(uint8_t interruptNumber) {
        (void)interruptNumber;
        // Mock implementation
    }
    
    void attachInterrupt() {
        // Mock implementation
    }
    
    void detachInterrupt() {
        // Mock implementation
    }
    
private:
    bool initialized_;
    uint8_t interruptMode_;
    uint8_t interruptMask_;
    uint8_t interruptSave_;
};

extern SPIClass SPI;
