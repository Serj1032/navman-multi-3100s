#pragma once

#include <Arduino.h>

class Sensor {
public:
    Sensor() = default;
    virtual ~Sensor() = default;

    int initialize() {
        int ret = probe();
        if(ret == 0) {
            is_available_ = true;
        }
        return ret;
    }

    virtual void process() = 0;

    bool is_available() const {
        return is_available_;
    }

protected:
    virtual int probe() = 0;

private:
    bool is_available_{false};
};
