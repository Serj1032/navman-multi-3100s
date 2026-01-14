# ============================================================================
# Arduino Core Library Configuration
# ============================================================================

# Define paths
set(ARDUINO_CORE_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/cores/arduino")
set(ARDUINO_VARIANT_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/variants/mega")

# MCU configuration for Arduino Mega 2560 (ATmega2560)
set(MCU "atmega2560")
set(F_CPU "16000000L")

# Collect all C and C++ source files from Arduino core
file(GLOB ARDUINO_CORE_SRCS 
    "${ARDUINO_CORE_PATH}/*.c"
    "${ARDUINO_CORE_PATH}/*.cpp"
)

# Create static library named "core"
add_library(core STATIC ${ARDUINO_CORE_SRCS})

# Set target properties for the core library
target_compile_definitions(core PUBLIC
    F_CPU=${F_CPU}
    ARDUINO=10607
    ARDUINO_AVR_MEGA2560
    ARDUINO_ARCH_AVR
)

target_compile_options(core PUBLIC
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++11>
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
    -Os  # Optimize for size
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
    -flto
)

target_include_directories(core PUBLIC
    ${ARDUINO_CORE_PATH}
    ${ARDUINO_VARIANT_PATH}
)
