cmake_minimum_required(VERSION 3.15)

project(navman-multi-3100s)

include(${CMAKE_SOURCE_DIR}/cmake/arduino_utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arduino_core.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arduino_library.cmake)

if (ARDUINO_TOOLCHAIN)
    include(${CMAKE_SOURCE_DIR}/cmake/arduino_toolchain.cmake)
    # include(${CMAKE_SOURCE_DIR}/third_party/cmake/adafruit_gfx.cmake)
    # include(${CMAKE_SOURCE_DIR}/third_party/cmake/adafruit_busio.cmake)
    # include(${CMAKE_SOURCE_DIR}/third_party/cmake/diyables_tft_touch_shield.cmake)
    include(${CMAKE_SOURCE_DIR}/third_party/cmake/lsm303.cmake)
elseif(HOST_TOOLCHAIN)
    # Use mock libraries for host builds
    include(${CMAKE_SOURCE_DIR}/tests/third_party/cmake/lsm303.cmake)
    include(${CMAKE_SOURCE_DIR}/tests/third_party/cmake/sdl3.cmake)
endif()

set(source_file_list
    ${CMAKE_SOURCE_DIR}/sketch/src/sensor_manager.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/protocols/nmea_protocol.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/sensors/compass.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/sensors/gps.cpp
)

set(include_dir_list
    ${CMAKE_SOURCE_DIR}/sketch/include
    ${CMAKE_SOURCE_DIR}/sketch/src
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")
    add_definitions(-D__DEBUG__)

    set(source_file_list
        ${source_file_list}
        ${CMAKE_SOURCE_DIR}/sketch/src/logger.cpp
    )
else()
    message(STATUS "Release build")
endif()

# Create executable from .ino and other sources
add_executable(navman-multi-3100s 
    sketch/navman-multi-3100s.cpp
    ${source_file_list}
)

target_include_directories(navman-multi-3100s PRIVATE
    ${include_dir_list} 
)

# Link Arduino core library
target_link_libraries(navman-multi-3100s PRIVATE 
    core
    lsm303
    # diyables_tft_touch_shield
)

# Link SDL3 for host builds
if(HOST_TOOLCHAIN AND TARGET SDL3::SDL3)
    target_link_libraries(navman-multi-3100s PRIVATE SDL3::SDL3)
endif()

# Set linker options (toolchain-specific)
if(ARDUINO_TOOLCHAIN)
    target_link_options(navman-multi-3100s PRIVATE
        -mmcu=atmega2560
        -Wl,--gc-sections
        -flto
        -fuse-linker-plugin
    )
endif()

# Generate hex and bin files
arduino_executable(navman-multi-3100s)

# AVR flash configuration
set(AVRDUDE_PATH "$ENV{HOME}/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude" CACHE STRING "Path to avrdude binary")
set(AVRDUDE_CONFIG "$ENV{HOME}/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf" CACHE STRING "Path to avrdude config")
set(AVRDUDE_PORT "/dev/cu.usbserial-1140" CACHE STRING "Serial port for flashing")
set(AVRDUDE_PROGRAMMER "wiring" CACHE STRING "Programmer type (wiring for ATmega2560)")
set(AVRDUDE_BAUD "115200" CACHE STRING "Baud rate for flashing")

# Add flash target
add_custom_target(flash
    COMMAND ${AVRDUDE_PATH} -C${AVRDUDE_CONFIG} -v -patmega2560 -c${AVRDUDE_PROGRAMMER} -P${AVRDUDE_PORT} -b${AVRDUDE_BAUD} -D -Uflash:w:navman-multi-3100s.hex:i
    DEPENDS navman-multi-3100s
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing ATmega2560 with ${AVRDUDE_PORT} at ${AVRDUDE_BAUD} baud"
)

# Add flash-bin target (alternative using .bin file)
add_custom_target(flash-bin
    COMMAND ${AVRDUDE_PATH} -C${AVRDUDE_CONFIG} -v -patmega2560 -c${AVRDUDE_PROGRAMMER} -P${AVRDUDE_PORT} -b${AVRDUDE_BAUD} -D -Uflash:w:navman-multi-3100s.bin:r
    DEPENDS navman-multi-3100s
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing ATmega2560 with .bin file using ${AVRDUDE_PORT} at ${AVRDUDE_BAUD} baud"
)