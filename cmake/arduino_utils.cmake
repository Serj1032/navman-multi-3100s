# ============================================================================
# Toolchain-Agnostic Utility Functions
# ============================================================================
# These functions work with both Arduino and Host toolchains by delegating
# to toolchain-specific functions defined in the respective toolchain files.

# Function to create a static library
function(arduino_library TARGET_NAME SOURCE_PATH)
    # Collect source files from the provided path (including subdirectories)
    file(GLOB_RECURSE LIB_SRCS 
        "${SOURCE_PATH}/*.c"
        "${SOURCE_PATH}/*.cpp"
    )
    
    # Create static library
    add_library(${TARGET_NAME} STATIC ${LIB_SRCS})
    
    # Include the source path
    target_include_directories(${TARGET_NAME} PUBLIC
        ${SOURCE_PATH}
    )
    
    # Apply toolchain-specific configuration
    if(COMMAND toolchain_configure_library)
        toolchain_configure_library(${TARGET_NAME})
    else()
        message(WARNING "toolchain_configure_library not defined in toolchain file")
    endif()
endfunction()

# Function to apply post-build steps for executables
function(arduino_executable TARGET_NAME)
    # Apply toolchain-specific post-build actions
    if(COMMAND toolchain_post_build)
        toolchain_post_build(${TARGET_NAME})
    else()
        message(WARNING "toolchain_post_build not defined in toolchain file")
    endif()
endfunction()
