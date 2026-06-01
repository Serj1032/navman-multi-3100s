#include <Arduino.h>

#include "logger.h"
#include "sensor_manager.h"

#include "display/display.h"
#include "display/screen_manager.h"

#include "keyboard.h"
#include "clock.h"
// #include "cli.h"

#ifdef HOST_BUILD
#include "mocked_data_setup.h"
#endif

void setup() {
#ifdef HOST_BUILD
  MockedData::setup();
#endif

  LOG_INFO("Starting Navman Multi 3100s");
  SensorManager::get_instance().initialize_sensors();
  // Cli::get_instance().init();
  Clock::get_instance().init(2); // UTC+2
  Keyboard::get_instance().init();
  Display::get_instance().init();
  ScreenManager::get_instance().init(&Display::get_instance());
}

void loop(void) {
  SensorManager::get_instance().process_sensors();
  
  Keyboard::get_instance().process();
  Clock::get_instance().process();
  Display::get_instance().process();
  ScreenManager::get_instance().process();
  // Cli::get_instance().process();
}
