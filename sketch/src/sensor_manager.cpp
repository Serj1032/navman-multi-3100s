#include "sensor_manager.h"

static Compass compass;
static GPS gps;

void SensorManager::initialize_sensors() {
    compass.initialize();
    gps.initialize();
}

void SensorManager::process_sensors() {
    compass.process();
    gps.process();
}

template <>
Compass* SensorManager::get_sensor<Compass>() {
    if (!compass.is_available()) {
        return nullptr;
    }
    return &compass;
}

template <>
GPS* SensorManager::get_sensor<GPS>() {
    if (!gps.is_available()) {
        return nullptr;
    }
    return &gps;
}