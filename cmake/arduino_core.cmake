# ============================================================================
# Arduino Core Library Configuration
# ============================================================================

# Define paths
set(ARDUINO_CORE_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/cores/arduino")
set(ARDUINO_VARIANT_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/variants/mega")

# MCU configuration for Arduino Mega 2560 (ATmega2560)
set(MCU "atmega2560")
set(F_CPU "16000000L")

# Create Arduino core static library
arduino_library(core "${ARDUINO_CORE_PATH}")

# Add variant path to include directories
target_include_directories(core PUBLIC
    ${ARDUINO_VARIANT_PATH}
)
