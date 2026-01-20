# ============================================================================
# Arduino Standard Libraries
# ============================================================================

if(ARDUINO_TOOLCHAIN)
    set(ARDUINO_LIBRARY_PATH "${ARDUINO_SDK_PATH}/packages/arduino/hardware/avr/1.8.6/libraries")

    arduino_library(wire_library "${ARDUINO_LIBRARY_PATH}/Wire/src")
    target_link_libraries(wire_library PRIVATE core)

    arduino_library(spi_library "${ARDUINO_LIBRARY_PATH}/SPI/src")
    target_link_libraries(spi_library PRIVATE core)
    
elseif(HOST_TOOLCHAIN)
    # Create mock Wire library for host builds
    set(WIRE_MOCK_PATH "${CMAKE_SOURCE_DIR}/tests/Arduino/libraries/Wire")
    arduino_library(wire_library "${WIRE_MOCK_PATH}")
    target_link_libraries(wire_library PUBLIC core)
    
    # Create mock SPI library for host builds
    set(SPI_MOCK_PATH "${CMAKE_SOURCE_DIR}/tests/Arduino/libraries/SPI")
    arduino_library(spi_library "${SPI_MOCK_PATH}")
    target_link_libraries(spi_library PUBLIC core)
    
    message(STATUS "Using mock Wire and SPI libraries for host build")
endif()