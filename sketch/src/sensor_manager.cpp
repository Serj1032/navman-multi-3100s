#include "sensor_manager.h"

#include "compass.h"

static Compass compass;

void SensorManager::initialize_sensors() {
    compass.initialize();
}

void SensorManager::process_sensors() {
    compass.process();
}