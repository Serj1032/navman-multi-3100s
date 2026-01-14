# ============================================================================
# Arduino Utility Functions
# ============================================================================

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
