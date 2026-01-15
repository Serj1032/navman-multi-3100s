# ============================================================================
# DIYables TFT Touch Shield Library Configuration
# ============================================================================

# Define library source path
set(DIYABLES_TFT_SRC_PATH "${CMAKE_SOURCE_DIR}/third_party/DIYables_TFT_Shield/src")

# Create static library using arduino_library function
arduino_library(diyables_tft_shield "${DIYABLES_TFT_SRC_PATH}")

# Link with Arduino core
target_link_libraries(diyables_tft_shield PUBLIC 
    core
    adafruit_gfx
)

