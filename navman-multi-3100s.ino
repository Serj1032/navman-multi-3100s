#include "display/display.h"
#include "display/screen_manager.h"
#include "navman.h"
#include "sensor_gps.h"
#include "sensor_compass.h"
#include "keyboard.h"
#include "clock.h"
#include "cli.h"

void setup() {
  GPS::get_instance().init();
  Compass::get_instance().init();
  Clock::get_instance().init(1); // UTC+1
  Navman::get_instance().init();
  Keyboard::get_instance().init();
  Display::get_instance().init();
  ScreenManager::get_instance().init(&Display::get_instance());
  Cli::get_instance().init();
}

void loop(void) {
  Keyboard::get_instance().process();
  GPS::get_instance().process();
  Compass::get_instance().process();
  Clock::get_instance().process();
  Navman::get_instance().process();
  ScreenManager::get_instance().process();
  Cli::get_instance().process();
}
