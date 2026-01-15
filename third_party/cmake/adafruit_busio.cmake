# ============================================================================
# Adafruit BusIO Library Configuration
# ============================================================================

# Define library source path (source files are in the root directory)
set(ADAFRUIT_BUSIO_PATH "${CMAKE_SOURCE_DIR}/third_party/Adafruit_BusIO")

# Create static library using arduino_library function
arduino_library(adafruit_busio "${ADAFRUIT_BUSIO_PATH}")

# Link with Arduino core
target_link_libraries(adafruit_busio PUBLIC 
    core
    wire_library
    spi_library
)
