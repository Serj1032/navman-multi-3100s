#include "compass.h"

#include <Wire.h>

#define LOG_TAG "CMPS"
#include "logger.h"

int Compass::probe() {
    LOG_DEBUG("Initializing Compass sensor");

    Wire.begin();
    Wire.setWireTimeout(3000, true); // Set 3ms timeout and reset on timeout
    
    // Set LSM303 timeout to prevent blocking
    compass.setTimeout(200); // 200ms timeout
    
    if (compass.init() == false) {
        LOG_ERROR("Could not find a valid LSM303 compass sensor, check wiring!");
        return -1;
    }
    
    // Check if timeout occurred during init
    if (compass.timeoutOccurred()) {
        LOG_ERROR("LSM303 compass initialization timeout - sensor not responding");
        return -1;
    }
    
    LOG_INFO("LSM303 compass detected");
    compass.enableDefault();

    /*
    Calibration values; the default values of +/-32767 for each axis
    lead to an assumed magnetometer bias of 0. Use the Calibrate example
    program to determine appropriate values for your particular unit.
    */
    compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
    compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};

    ts_ = millis();
    return 0;
}

void Compass::process() {
    if (!is_available()) {
        return;
    }
    
    uint32_t now = millis();
    if (now - ts_ >= 500) { // Update heading every 500 ms
        compass.read();
        heading_ = compass.heading();
        ts_ = now;
    }
}