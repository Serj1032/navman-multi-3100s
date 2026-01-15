# ============================================================================
# Arduino Utility Functions
# ============================================================================

# Function to create Arduino static library
function(arduino_library TARGET_NAME SOURCE_PATH)
    # Collect source files from the provided path (including subdirectories)
    file(GLOB_RECURSE LIB_SRCS 
        "${SOURCE_PATH}/*.c"
        "${SOURCE_PATH}/*.cpp"
    )
    
    # Create static library
    add_library(${TARGET_NAME} STATIC ${LIB_SRCS})
    
    # Set target properties
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
    
    target_include_directories(${TARGET_NAME} PUBLIC
        ${SOURCE_PATH}
    )
endfunction()

# Function to create hex and bin files from Arduino executable
function(arduino_executable TARGET_NAME)
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
