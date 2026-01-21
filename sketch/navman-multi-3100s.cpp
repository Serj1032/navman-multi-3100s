#include <Arduino.h>

#include "logger.h"
#include "sensor_manager.h"

#include "display/display.h"
#include "display/screen_manager.h"
// #include "navman.h"
#include "keyboard.h"
#include "clock.h"
// #include "cli.h"

void setup() {
  LOG_INFO("Starting Navman Multi 3100s");
  SensorManager::get_instance().initialize_sensors();
  // Cli::get_instance().init();
  Clock::get_instance().init(1); // UTC+1
  // Navman::get_instance().init();
  Keyboard::get_instance().init();
  Display::get_instance().init();
  ScreenManager::get_instance().init(&Display::get_instance());
}

void loop(void) {
  SensorManager::get_instance().process_sensors();
  
  Keyboard::get_instance().process();
  Clock::get_instance().process();
  // Navman::get_instance().process();
  Display::get_instance().process();
  ScreenManager::get_instance().process();
  // Cli::get_instance().process();
}
