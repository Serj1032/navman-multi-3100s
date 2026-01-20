# Host toolchain for compiling and testing code on the development machine
# This allows building unit tests and development tools without Arduino dependencies
# Compatible with macOS and Ubuntu/Linux

# Mark this as Host toolchain
set(HOST_TOOLCHAIN TRUE)

# Detect host system
if(APPLE)
    set(CMAKE_SYSTEM_NAME Darwin)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
elseif(UNIX)
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
endif()

# Use native compilers (prefer clang on macOS, gcc on Linux)
if(APPLE)
    set(CMAKE_C_COMPILER /usr/bin/clang CACHE FILEPATH "Host C compiler")
    set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE FILEPATH "Host C++ compiler")
else()
    # Linux/Ubuntu - try to find gcc/g++ or fallback to clang
    find_program(GCC_COMPILER gcc)
    find_program(GXX_COMPILER g++)
    if(GCC_COMPILER AND GXX_COMPILER)
        set(CMAKE_C_COMPILER ${GCC_COMPILER} CACHE FILEPATH "Host C compiler")
        set(CMAKE_CXX_COMPILER ${GXX_COMPILER} CACHE FILEPATH "Host C++ compiler")
    else()
        set(CMAKE_C_COMPILER /usr/bin/clang CACHE FILEPATH "Host C compiler")
        set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE FILEPATH "Host C++ compiler")
    endif()
endif()

set(CMAKE_LINKER /usr/bin/ld CACHE FILEPATH "Host linker")
set(CMAKE_AR /usr/bin/ar CACHE FILEPATH "Host archiver")
set(CMAKE_RANLIB /usr/bin/ranlib CACHE FILEPATH "Host ranlib")

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags for host builds
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra" CACHE STRING "Host C flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" CACHE STRING "Host C++ flags")

# Debug/Release configurations
set(CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE STRING "Host C debug flags")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "Host C++ debug flags")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "Host C release flags")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "Host C++ release flags")

# Define that this is a host build (useful for conditional compilation)
add_definitions(-DHOST_BUILD)

# Skip Arduino-specific checks
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

message(STATUS "Configured for host build on ${CMAKE_SYSTEM_NAME}")

# ============================================================================
# Host Toolchain-Specific Functions
# ============================================================================

# Function to configure library for host target
function(toolchain_configure_library TARGET_NAME)
    target_compile_options(${TARGET_NAME} PUBLIC
        -Wall
        -Wextra
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
    )
endfunction()

# Function for post-build actions on host (no-op for now)
function(toolchain_post_build TARGET_NAME)
    # Host builds don't need special post-build steps
    # Could add code coverage, sanitizer reports, etc. here
endfunction()
