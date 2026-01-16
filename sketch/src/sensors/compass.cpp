#include "compass.h"

#include <Wire.h>

#include "logger.h"

void Compass::initialize() {
    LOG_DEBUG("Initializing Compass sensor");

    // TODO: compass may block booting process
    Wire.begin();
    compass.init();
    compass.enableDefault();

    /*
    Calibration values; the default values of +/-32767 for each axis
    lead to an assumed magnetometer bias of 0. Use the Calibrate example
    program to determine appropriate values for your particular unit.
    */
    compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
    compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};

    ts_ = millis();
}

void Compass::process() {
    uint32_t now = millis();
    if (now - ts_ >= 500) { // Update heading every 500 ms
        compass.read();
        heading_ = compass.heading();
        ts_ = now;
    }
}