cmake_minimum_required(VERSION 3.15)

project(navman-multi-3100s)

include(${CMAKE_SOURCE_DIR}/cmake/arduino_utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arduino_core.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arduino_library.cmake)

if (ARDUINO_TOOLCHAIN)
    include(${CMAKE_SOURCE_DIR}/cmake/arduino_toolchain.cmake)
    include(${CMAKE_SOURCE_DIR}/third_party/cmake/adafruit_gfx.cmake)
    include(${CMAKE_SOURCE_DIR}/third_party/cmake/adafruit_busio.cmake)
    include(${CMAKE_SOURCE_DIR}/third_party/cmake/diyables_tft_touch_shield.cmake)
    include(${CMAKE_SOURCE_DIR}/third_party/cmake/lsm303.cmake)
elseif(HOST_TOOLCHAIN)
    # Use mock libraries for host builds
    include(${CMAKE_SOURCE_DIR}/tests/third_party/cmake/lsm303.cmake)
    include(${CMAKE_SOURCE_DIR}/tests/third_party/cmake/sdl3.cmake)
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests/Arduino/libraries/DIYables_TFT_Touch_Shield)

    # Mocked data sources for host testing
    set(mocked_data_sources
        ${CMAKE_SOURCE_DIR}/tests/mocked_data/gps_nmea_data.cpp
        ${CMAKE_SOURCE_DIR}/tests/mocked_data/navman_nmea_data.cpp
        ${CMAKE_SOURCE_DIR}/tests/mocked_data/mocked_data_setup.cpp
    )
    set(mocked_data_includes
        ${CMAKE_SOURCE_DIR}/tests/mocked_data
        ${CMAKE_SOURCE_DIR}/tests/Arduino/core
    )
endif()

set(source_file_list
    ${CMAKE_SOURCE_DIR}/sketch/src/sensor_manager.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/protocols/nmea_protocol.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/sensors/compass.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/sensors/gps.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/sensors/navman.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/color_scheme.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/screens/screen_manager.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/screens/screen_welcome.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/screens/screen_dashboard.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/screens/screen_logs.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/screens/screen_system_info.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/keyboard.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/text.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/icons/arrow_icon.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/icons/boat_icon.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/icons/bitmap_icon.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/widgets/heading_widget.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/widgets/clock_widget.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/widgets/satellite_widget.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/widgets/metric_value_widget.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/display/widgets/metrics_widget.cpp
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
    ${mocked_data_sources}
)

target_include_directories(navman-multi-3100s PRIVATE
    ${include_dir_list}
    ${mocked_data_includes}
)

# Link Arduino core library
target_link_libraries(navman-multi-3100s PRIVATE 
    core
    lsm303
    diyables_tft_touch_shield
)

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