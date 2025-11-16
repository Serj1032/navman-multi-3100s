#include "display/display.h"
#include "display/screen_manager.h"
#include "nmea.h"
#include "navman.h"
#include "gps.h"
#include "keyboard.h"
#include "cli.h"

void setup() {
  GPS::get_instance().init();
  Navman::get_instance().init();
  Keyboard::get_instance().init();
  Display::get_instance().init();
  ScreenManager::get_instance().init(&Display::get_instance());
  Cli::get_instance().init();
}

void loop(void) {
  Keyboard::get_instance().process();
  GPS::get_instance().process();
  Navman::get_instance().process();
  ScreenManager::get_instance().process();
  Cli::get_instance().process();
}
