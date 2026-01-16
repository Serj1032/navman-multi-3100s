#pragma once

#include <Arduino.h>

class Sensor {
public:
    Sensor() = default;
    virtual ~Sensor() = default;

    virtual void process() = 0;
};
