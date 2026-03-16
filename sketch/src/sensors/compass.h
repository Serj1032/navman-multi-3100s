#pragma once

#include <LSM303.h>

#include "sensor.h"

// #define COMPASS_CALIBRATION

class Compass : public Sensor {
public:
    Compass() = default;
    ~Compass() = default;

    int get_heading(float* heading) const {
        *heading = heading_;
        return 0;
    }

    void process() override;
    
private:
    Compass(const Compass&) = delete;
    Compass& operator=(const Compass&) = delete;

    int probe() override;

#ifdef COMPASS_CALIBRATION
    void calibration_update();
    void calibration_report();
#endif

private:
    uint32_t ts_{0};
    float heading_{0.0f};
    LSM303 compass;

#ifdef COMPASS_CALIBRATION
    LSM303::vector<int16_t> cal_min_{32767, 32767, 32767};
    LSM303::vector<int16_t> cal_max_{-32768, -32768, -32768};
    uint32_t cal_report_ts_{0};
#endif
};