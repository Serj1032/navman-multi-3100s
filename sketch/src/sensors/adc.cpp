#include "sensors/adc.h"

#define LOG_TAG "ADC"
#include "logger.h"

int BatterySensor::probe() {
    LOG_INFO("ADC battery sensor initialized on pin " + String(ADC_BATTERY_PIN));
    return 0;
}

void BatterySensor::process() {
    int raw = analogRead(ADC_BATTERY_PIN);
    float sample = (static_cast<float>(raw) / ADC_RESOLUTION) * ADC_VREF * ADC_BATTERY_SCALE_FACTOR;

    if (!initialized_) {
        for (uint8_t i = 0; i < ADC_AVG_SAMPLES; i++) {
            samples_[i] = sample;
        }
        sum_ = sample * ADC_AVG_SAMPLES;
        initialized_ = true;
    } else {
        sum_ -= samples_[sample_idx_];
        samples_[sample_idx_] = sample;
        sum_ += sample;
    }

    sample_idx_ = (sample_idx_ + 1) % ADC_AVG_SAMPLES;
    voltage_ = sum_ / ADC_AVG_SAMPLES;
}
