# ============================================================================
# Adafruit GFX Library Configuration
# ============================================================================

# Define library source path (source files are in the root directory)
set(ADAFRUIT_GFX_PATH "${CMAKE_SOURCE_DIR}/third_party/Adafruit-GFX-Library")

# Create static library using arduino_library function
arduino_library(adafruit_gfx "${ADAFRUIT_GFX_PATH}")

# Link with Arduino core
target_link_libraries(adafruit_gfx PUBLIC 
    core
    adafruit_busio
)
