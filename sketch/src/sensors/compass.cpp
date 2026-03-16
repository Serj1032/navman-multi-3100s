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
    // compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
    // compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
    compass.m_min = (LSM303::vector<int16_t>){-523, -612, -301};
    compass.m_max = (LSM303::vector<int16_t>){+393, +510, +475};

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
#ifdef COMPASS_CALIBRATION
        calibration_update();
#endif
        // Compass is mounted with: X→down, Y→left, Z→forward
        // The 'from' vector is the vessel's forward direction in sensor coords = Z axis
        heading_ = compass.heading((LSM303::vector<int>){0, 0, 1});
        ts_ = now;
    }
}

#ifdef COMPASS_CALIBRATION
/*
 * Calibration mode — enable by adding -DCOMPASS_CALIBRATION to build flags.
 *
 * Rotate the compass slowly through all orientations (figure-8 / full sphere).
 * The running min/max values are printed to Serial every second.
 * Once values stabilize, copy them into probe() as m_min / m_max.
 *
 * Output format:
 *   COMPASS CAL | min: {  -523,  -612,  -498}  max: {  +467,  +510,  +475}
 */
void Compass::calibration_update() {
    // Track running min/max across all reads
    if (compass.m.x < cal_min_.x) cal_min_.x = compass.m.x;
    if (compass.m.y < cal_min_.y) cal_min_.y = compass.m.y;
    if (compass.m.z < cal_min_.z) cal_min_.z = compass.m.z;

    if (compass.m.x > cal_max_.x) cal_max_.x = compass.m.x;
    if (compass.m.y > cal_max_.y) cal_max_.y = compass.m.y;
    if (compass.m.z > cal_max_.z) cal_max_.z = compass.m.z;

    // Print report once per second
    uint32_t now = millis();
    if (now - cal_report_ts_ >= 1000) {
        cal_report_ts_ = now;
        calibration_report();
    }
}

void Compass::calibration_report() {
    char buf[120];
    snprintf(buf, sizeof(buf),
        "COMPASS CAL | min: {%+6d, %+6d, %+6d}  max: {%+6d, %+6d, %+6d}",
        cal_min_.x, cal_min_.y, cal_min_.z,
        cal_max_.x, cal_max_.y, cal_max_.z);
    Serial.println(buf);
}
#endif // COMPASS_CALIBRATION