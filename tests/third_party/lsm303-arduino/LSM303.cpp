#include "LSM303.h"
#include <math.h>

// Mock LSM303 implementation for host testing

LSM303::LSM303(void)
{
    _device = device_auto;
    io_timeout = 0;
    did_timeout = false;
    
    // Initialize vectors to zero
    a.x = a.y = a.z = 0;
    m.x = m.y = m.z = 0;
    m_max.x = m_max.y = m_max.z = 32767;
    m_min.x = m_min.y = m_min.z = -32768;
    
    acc_address = 0x19;
    mag_address = 0x1E;
    last_status = 0;
}

bool LSM303::init(deviceType device, sa0State sa0)
{
    (void)sa0;
    
    // Mock implementation - always succeeds
    if (device != device_auto) {
        _device = device;
    } else {
        _device = device_DLHC; // Default to DLHC
    }
    
    return true;
}

void LSM303::enableDefault(void)
{
    // Mock implementation - does nothing
}

void LSM303::writeAccReg(byte reg, byte value)
{
    (void)reg;
    (void)value;
    // Mock implementation - does nothing
}

byte LSM303::readAccReg(byte reg)
{
    (void)reg;
    // Mock implementation - returns 0
    return 0;
}

void LSM303::writeMagReg(byte reg, byte value)
{
    (void)reg;
    (void)value;
    // Mock implementation - does nothing
}

byte LSM303::readMagReg(int reg)
{
    (void)reg;
    // Mock implementation - returns 0
    return 0;
}

void LSM303::writeReg(byte reg, byte value)
{
    (void)reg;
    (void)value;
    // Mock implementation - does nothing
}

byte LSM303::readReg(int reg)
{
    (void)reg;
    // Mock implementation - returns 0
    return 0;
}

void LSM303::readAcc(void)
{
    // Mock implementation - returns constant values
    a.x = 100;
    a.y = 200;
    a.z = 300;
}

void LSM303::readMag(void)
{
    // Mock implementation - returns constant values
    m.x = 150;
    m.y = 250;
    m.z = 350;
}

void LSM303::read(void)
{
    readAcc();
    readMag();
}

void LSM303::setTimeout(unsigned int timeout)
{
    io_timeout = timeout;
}

unsigned int LSM303::getTimeout(void)
{
    return io_timeout;
}

bool LSM303::timeoutOccurred(void)
{
    return did_timeout;
}

float LSM303::heading(void)
{
    // Mock implementation - returns a fixed heading
    // In a real implementation, this would calculate heading from magnetometer data
    return 45.0f; // Return 45 degrees as mock value
}

void LSM303::vector_normalize(vector<float> *a)
{
    float mag = sqrt(vector_dot(a, a));
    if (mag > 0.0f) {
        a->x /= mag;
        a->y /= mag;
        a->z /= mag;
    }
}

int LSM303::testAddrReg(byte address, regAddr reg)
{
    (void)address;
    (void)reg;
    // Mock implementation
    return 0;
}

int16_t LSM303::testReg(byte address, regAddr reg)
{
    (void)address;
    (void)reg;
    // Mock implementation
    return 0;
}
