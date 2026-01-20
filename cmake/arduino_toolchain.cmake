set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr) 

# Mark this as Arduino toolchain
set(ARDUINO_TOOLCHAIN TRUE)

# Arduino SDK base path
set(ARDUINO_SDK_PATH "/Users/kravserg/Library/Arduino15")

set(ARDUINO_TOOLCHAIN_DIR "${ARDUINO_SDK_PATH}/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-")

set(CMAKE_CXX_COMPILER ${ARDUINO_TOOLCHAIN_DIR}g++ CACHE FILEPATH "Arduino C++ compiler")
set(CMAKE_C_COMPILER ${ARDUINO_TOOLCHAIN_DIR}gcc CACHE FILEPATH "Arduino C compiler")
set(CMAKE_LINKER ${ARDUINO_TOOLCHAIN_DIR}ld CACHE FILEPATH "Arduino linker")
set(CMAKE_AR ${ARDUINO_TOOLCHAIN_DIR}gcc-ar CACHE FILEPATH "Arduino archiver")
set(CMAKE_RANLIB ${ARDUINO_TOOLCHAIN_DIR}gcc-ranlib CACHE FILEPATH "Arduino ranlib")

#SET(CMAKE_C_FLAGS   "-U__GNUC__ -U__GNUC_MINOR__" CACHE STRING "" FORCE)
#SET(CMAKE_CXX_FLAGS "-U__GNUC__ -U__GNUC_MINOR__" CACHE STRING "" FORCE)
SET(CMAKE_C_FLAGS   "-U__GNUC_MINOR__" CACHE STRING "" FORCE)
SET(CMAKE_CXX_FLAGS "-U__GNUC_MINOR__" CACHE STRING "" FORCE)

set(CMAKE_C_STANDARD_COMPUTED_DEFAULT 11)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT 17)

set(CMAKE_CXX17_COMPILE_FEATURES "")
set(CMAKE_CXX20_COMPILE_FEATURES "")

set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++17")
set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++17")

# Arduino board configuration
set(MCU atmega2560)
set(F_CPU 16000000L)

# ============================================================================
# Arduino Toolchain-Specific Functions
# ============================================================================

# Function to configure library for Arduino target
function(toolchain_configure_library TARGET_NAME)
    target_compile_definitions(${TARGET_NAME} PUBLIC
        F_CPU=${F_CPU}
        ARDUINO=10607
        ARDUINO_AVR_MEGA2560
        ARDUINO_ARCH_AVR
    )
    
    target_compile_options(${TARGET_NAME} PUBLIC
        -mmcu=${MCU}
        -DF_CPU=${F_CPU}
        $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++11>
        $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
        $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
        -Os
        -Wall
        -Wextra
        -ffunction-sections
        -fdata-sections
        -flto
    )
endfunction()

# Function to create hex and bin files from Arduino executable
function(toolchain_post_build TARGET_NAME)
    # Get the objcopy tool
    set(AVR_OBJCOPY "${ARDUINO_TOOLCHAIN_DIR}objcopy")
    set(AVR_SIZE "${ARDUINO_TOOLCHAIN_DIR}size")
    
    # Add custom command to generate .hex file
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
        COMMENT "Generating HEX file: ${TARGET_NAME}.hex"
        BYPRODUCTS ${TARGET_NAME}.hex
    )
    
    # Add custom command to generate .bin file
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O binary -R .eeprom $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.bin
        COMMENT "Generating BIN file: ${TARGET_NAME}.bin"
        BYPRODUCTS ${TARGET_NAME}.bin
    )
    
    # Add custom command to generate .eep file (EEPROM)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.eep
        COMMENT "Generating EEPROM file: ${TARGET_NAME}.eep"
        BYPRODUCTS ${TARGET_NAME}.eep
    )
    
    # Add custom command to show size information
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${AVR_SIZE} --format=avr --mcu=${MCU} $<TARGET_FILE:${TARGET_NAME}>
        COMMENT "Size information:"
    )
endfunction()

