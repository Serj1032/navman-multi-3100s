
set(ARDUINO_LIBRARY_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/libraries")

arduino_library(wire_library "${ARDUINO_LIBRARY_PATH}/Wire/src")
target_link_libraries(wire_library PRIVATE core)


arduino_library(spi_library "${ARDUINO_LIBRARY_PATH}/SPI/src")
target_link_libraries(spi_library PRIVATE core)