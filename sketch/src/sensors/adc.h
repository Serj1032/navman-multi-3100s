#pragma once

#include "sensor.h"
#include "config.h"

#define ADC_AVG_SAMPLES 10

class BatterySensor : public Sensor {
public:
    BatterySensor() = default;
    ~BatterySensor() = default;

    float voltage() const { return voltage_; }

    void process() override;

private:
    BatterySensor(const BatterySensor&) = delete;
    BatterySensor& operator=(const BatterySensor&) = delete;

    int probe() override;

    float samples_[ADC_AVG_SAMPLES]{};
    uint8_t sample_idx_{0};
    bool initialized_{false};
    float sum_{0.0f};
    float voltage_{0.0f};
};
