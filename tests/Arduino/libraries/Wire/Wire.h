#pragma once

#include <inttypes.h>
#include "Stream.h"

#define BUFFER_LENGTH 32
#define WIRE_HAS_END 1

// Mock TwoWire class for host testing
class TwoWire : public Stream {
private:
    uint8_t rxBuffer[BUFFER_LENGTH];
    uint8_t rxBufferIndex;
    uint8_t rxBufferLength;
    
    uint8_t txAddress;
    uint8_t txBuffer[BUFFER_LENGTH];
    uint8_t txBufferIndex;
    uint8_t txBufferLength;
    
    uint8_t transmitting;
    
public:
    TwoWire() : rxBufferIndex(0), rxBufferLength(0), 
                txAddress(0), txBufferIndex(0), 
                txBufferLength(0), transmitting(0) {}
    
    void begin() {
        // Mock implementation
    }
    
    void begin(uint8_t address) {
        (void)address;
        // Mock implementation
    }
    
    void begin(int address) {
        begin((uint8_t)address);
    }
    
    void end() {
        // Mock implementation
    }
    
    void setClock(uint32_t clock) {
        (void)clock;
        // Mock implementation
    }
    
    void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false) {
        (void)timeout;
        (void)reset_with_timeout;
        // Mock implementation
    }
    
    bool getWireTimeoutFlag(void) {
        return false;
    }
    
    void clearWireTimeoutFlag(void) {
        // Mock implementation
    }
    
    void beginTransmission(uint8_t address) {
        transmitting = 1;
        txAddress = address;
        txBufferIndex = 0;
        txBufferLength = 0;
    }
    
    void beginTransmission(int address) {
        beginTransmission((uint8_t)address);
    }
    
    uint8_t endTransmission(void) {
        return endTransmission(true);
    }
    
    uint8_t endTransmission(uint8_t sendStop) {
        (void)sendStop;
        // Mock implementation - always success
        transmitting = 0;
        txBufferIndex = 0;
        txBufferLength = 0;
        return 0;
    }
    
    uint8_t requestFrom(uint8_t address, uint8_t quantity) {
        return requestFrom(address, quantity, (uint8_t)true);
    }
    
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
        (void)address;
        (void)sendStop;
        // Mock implementation - no data received
        rxBufferIndex = 0;
        rxBufferLength = 0;
        return 0;
    }
    
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, 
                        uint8_t isize, uint8_t sendStop) {
        (void)iaddress;
        (void)isize;
        return requestFrom(address, quantity, sendStop);
    }
    
    uint8_t requestFrom(int address, int quantity) {
        return requestFrom((uint8_t)address, (uint8_t)quantity);
    }
    
    uint8_t requestFrom(int address, int quantity, int sendStop) {
        return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
    }
    
    virtual size_t write(uint8_t data) {
        if (transmitting) {
            if (txBufferLength >= BUFFER_LENGTH) {
                return 0;
            }
            txBuffer[txBufferIndex] = data;
            ++txBufferIndex;
            txBufferLength = txBufferIndex;
        }
        return 1;
    }
    
    virtual size_t write(const uint8_t *data, size_t quantity) {
        size_t written = 0;
        for (size_t i = 0; i < quantity; ++i) {
            written += write(data[i]);
        }
        return written;
    }
    
    virtual int available(void) {
        return rxBufferLength - rxBufferIndex;
    }
    
    virtual int read(void) {
        if (rxBufferIndex < rxBufferLength) {
            return rxBuffer[rxBufferIndex++];
        }
        return -1;
    }
    
    virtual int peek(void) {
        if (rxBufferIndex < rxBufferLength) {
            return rxBuffer[rxBufferIndex];
        }
        return -1;
    }
    
    virtual void flush(void) {
        // Mock implementation
    }
    
    void onReceive(void (*callback)(int)) {
        (void)callback;
        // Mock implementation
    }
    
    void onRequest(void (*callback)(void)) {
        (void)callback;
        // Mock implementation
    }
    
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
};

extern TwoWire Wire;
