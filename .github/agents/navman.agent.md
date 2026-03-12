---
name: navman
description: Arduino-based navigation dashboard system with GPS, compass, and TFT touchscreen display
argument-hint: tasks related to Arduino embedded development, sensor integration, display UI, or CMake build system
# tools: ['vscode', 'execute', 'read', 'agent', 'edit', 'search', 'web', 'todo']
---

# Navman Multi 3100s Project

Arduino-based navigation dashboard for reading GPS/NMEA data, compass heading, and displaying on TFT touchscreen.

sketch/ - main Arduino application code
sketch/navman-multi-3100s.cpp - entry point with setup() and loop()
sketch/src/sensors/ - GPS and compass (LSM303) sensor implementations
sketch/src/protocols/ - NMEA protocol parser for GPS data
sketch/src/screens/ - UI screens (welcome, dashboard)
sketch/include/display/ - display components, widgets, buttons, icons
sketch/src/sensor_manager.cpp - manages sensor initialization and polling
sketch/src/keyboard.h - button input handling (4 physical buttons)
sketch/src/clock.h - time management with timezone support
sketch/src/config.h - pin assignments, serial ports, and configuration constants

cmake/ - CMake build system for Arduino and host toolchains
cmake/arduino_toolchain.cmake - Arduino AVR cross-compilation setup
cmake/host_toolchain.cmake - native host builds for testing

third_party/ - external Arduino libraries
third_party/DIYables_TFT_Touch_Shield/ - TFT LCD touchscreen driver
third_party/lsm303-arduino/ - LSM303 compass/accelerometer library
third_party/Adafruit-GFX-Library/ - graphics primitives

tests/ - mock Arduino environment for host-based testing
tests/Arduino/ - stub implementations of Arduino core functions