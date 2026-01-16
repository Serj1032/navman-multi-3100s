#pragma once

#include <LSM303.h>

#include "sensor.h"

class Compass : public Sensor {
public:
    Compass() = default;
    ~Compass() = default;

    void initialize();

    int get_heading(float* heading) const {
        *heading = heading_;
        return 0;
    }

    void process() override;
    
private:
    Compass(const Compass&) = delete;
    Compass& operator=(const Compass&) = delete;

private:
    uint32_t ts_{0};
    float heading_{0.0f};
    LSM303 compass;
};