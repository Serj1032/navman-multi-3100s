#include "compass.h"

#include <Wire.h>

#include "logger.h"

int Compass::probe() {
    LOG_DEBUG("Initializing Compass sensor");

    // TODO: compass may block booting process
    Wire.begin();
    if (compass.init() == false) {
        LOG_ERROR("Could not find a valid LSM303 compass sensor, check wiring!");
        return -1;
    } else {
        LOG_INFO("LSM303 compass detected");
    }
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