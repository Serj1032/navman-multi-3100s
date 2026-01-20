# SDL3 library integration for host builds
# This assumes SDL3 has been pre-built using build_sdl.sh script

if(NOT HOST_TOOLCHAIN)
    message(WARNING "SDL3 should only be used with HOST_TOOLCHAIN")
    return()
endif()

set(SDL3_LIB_DIR "${CMAKE_SOURCE_DIR}/tests/third_party/libs/lib")
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/tests/third_party/libs/include/SDL3")

# Check if SDL3 library exists
if(NOT EXISTS "${SDL3_LIB_DIR}/libSDL3.a")
    message(WARNING "SDL3 static library not found at ${SDL3_LIB_DIR}/libSDL3.a")
    message(WARNING "Please run: tests/third_party/scripts/build_sdl.sh")
    return()
endif()

# Create imported target for SDL3
add_library(SDL3::SDL3 STATIC IMPORTED)
set_target_properties(SDL3::SDL3 PROPERTIES
    IMPORTED_LOCATION "${SDL3_LIB_DIR}/libSDL3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}"
)

# SDL3 may require system frameworks on macOS
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FORCEFEEDBACK_LIBRARY ForceFeedback)
    find_library(CARBON_LIBRARY Carbon)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(METAL_LIBRARY Metal)
    find_library(COREHAPTICS_LIBRARY CoreHaptics)
    find_library(GAMECONTROLLER_LIBRARY GameController)
    
    set(SDL3_SYSTEM_LIBS
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${FORCEFEEDBACK_LIBRARY}
        ${CARBON_LIBRARY}
        ${COREAUDIO_LIBRARY}
        ${AUDIOTOOLBOX_LIBRARY}
        ${COREVIDEO_LIBRARY}
        ${COREGRAPHICS_LIBRARY}
        ${COREFOUNDATION_LIBRARY}
        ${METAL_LIBRARY}
        ${COREHAPTICS_LIBRARY}
        ${GAMECONTROLLER_LIBRARY}
    )
    
    target_link_libraries(SDL3::SDL3 INTERFACE ${SDL3_SYSTEM_LIBS})
elseif(UNIX)
    # Linux dependencies
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(SDL3::SDL3 INTERFACE ${X11_LIBRARIES})
    endif()
endif()

message(STATUS "SDL3 static library configured: ${SDL3_LIB_DIR}/libSDL3.a")
