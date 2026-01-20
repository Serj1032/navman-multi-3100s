# Mock LSM303 library for host testing
arduino_library(lsm303 "${CMAKE_SOURCE_DIR}/tests/third_party/lsm303-arduino")

target_link_libraries(lsm303 PUBLIC 
    core
    wire_library
)

message(STATUS "Using mock LSM303 library for host build")
