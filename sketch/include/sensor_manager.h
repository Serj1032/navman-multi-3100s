#pragma once

#include "sensors/compass.h"
#include "sensors/gps.h"

class SensorManager {
public:
    static SensorManager& get_instance() {
        static SensorManager instance;
        return instance;
    }

    void initialize_sensors();

    void process_sensors();

    template <typename T>
    T* get_sensor();

private: 
    SensorManager() = default;
    ~SensorManager() = default;
};

template <>
Compass* SensorManager::get_sensor<Compass>();

template <>
GPS* SensorManager::get_sensor<GPS>();