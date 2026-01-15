arduino_library(lsm303 "${CMAKE_SOURCE_DIR}/third_party/lsm303-arduino")

target_link_libraries(lsm303 PUBLIC 
    core
    wire_library
)
