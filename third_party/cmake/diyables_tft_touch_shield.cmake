# ============================================================================
# DIYables TFT Touch Shield Library Configuration
# ============================================================================

include(${CMAKE_SOURCE_DIR}/third_party/cmake/diyables_tft_shield.cmake)

# Define library source path
set(DIYABLES_TFT_TOUCH_SRC_PATH "${CMAKE_SOURCE_DIR}/third_party/DIYables_TFT_Touch_Shield/src")

# Create static library using arduino_library function
arduino_library(diyables_tft_touch_shield "${DIYABLES_TFT_TOUCH_SRC_PATH}")

# Link with Arduino core
target_link_libraries(diyables_tft_touch_shield PUBLIC 
    core
    adafruit_gfx
    diyables_tft_shield
)

