cmake_minimum_required(VERSION 3.15)

project(navman-multi-3100s)

# Include utility functions first (defines arduino_library function)
include(${CMAKE_SOURCE_DIR}/cmake/arduino_utils.cmake)
# Include Arduino core library configuration
include(${CMAKE_SOURCE_DIR}/cmake/arduino_core.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arduino_library.cmake)
include(${CMAKE_SOURCE_DIR}/third_party/cmake/adafruit_gfx.cmake)
include(${CMAKE_SOURCE_DIR}/third_party/cmake/adafruit_busio.cmake)
include(${CMAKE_SOURCE_DIR}/third_party/cmake/diyables_tft_touch_shield.cmake)
include(${CMAKE_SOURCE_DIR}/third_party/cmake/lsm303.cmake)

file(GLOB source_file_list
    ${CMAKE_SOURCE_DIR}/sketch/src/*.cpp
    ${CMAKE_SOURCE_DIR}/sketch/src/screens/*.cpp
)

set(include_dir_list
    ${CMAKE_SOURCE_DIR}/sketch/include
    ${CMAKE_SOURCE_DIR}/sketch/src
)

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
    diyables_tft_touch_shield
)

# Set linker options for AVR
target_link_options(navman-multi-3100s PRIVATE
    -mmcu=atmega2560
    -Wl,--gc-sections
    -flto
    -fuse-linker-plugin
)

# Generate hex and bin files
arduino_executable(navman-multi-3100s)