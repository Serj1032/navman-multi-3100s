# ============================================================================
# Arduino Core Library Configuration
# ============================================================================

# Define paths
if (ARDUINO_TOOLCHAIN)
    set(ARDUINO_CORE_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/cores/arduino")
    set(ARDUINO_VARIANT_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/variants/mega")
endif()

if (HOST_TOOLCHAIN)
    set(ARDUINO_CORE_PATH "${CMAKE_SOURCE_DIR}/tests/Arduino/core")
    set(ARDUINO_VARIANT_PATH "${CMAKE_SOURCE_DIR}/tests/Arduino/variants/mega")
endif()

# Create Arduino core static library
arduino_library(core "${ARDUINO_CORE_PATH}")

# Add variant path to include directories
target_include_directories(core PUBLIC
    ${ARDUINO_VARIANT_PATH}
)
