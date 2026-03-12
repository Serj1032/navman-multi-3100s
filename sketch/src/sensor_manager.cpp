#include "sensor_manager.h"

static Compass compass;
static GPS gps;
static Navman navman;

void SensorManager::initialize_sensors() {
    compass.initialize();
    gps.initialize();
    navman.initialize();
}

void SensorManager::process_sensors() {
    if (compass.is_available()) {
        compass.process();
    }
    if (gps.is_available()) {
        gps.process();
    }
    if (navman.is_available()) {
        navman.process();
    }
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

template <>
Navman* SensorManager::get_sensor<Navman>() {
    if (!navman.is_available()) {
        return nullptr;
    }
    return &navman;
}