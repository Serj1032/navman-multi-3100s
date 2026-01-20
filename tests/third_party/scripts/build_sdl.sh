#!/bin/bash
# Script to build SDL3 as a static library for host testing

set -e

# Get absolute paths
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SDL_SOURCE_DIR="$(cd "$SCRIPT_DIR/../sdl" && pwd)"
BUILD_DIR="$SDL_SOURCE_DIR/build"
INSTALL_DIR="$(cd "$SCRIPT_DIR/.." && pwd)/libs"

echo "Building SDL3 static library..."
echo "Source: $SDL_SOURCE_DIR"
echo "Build: $BUILD_DIR"
echo "Install: $INSTALL_DIR"

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure SDL with minimal options for static library
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL_SHARED=OFF \
    -DSDL_STATIC=ON \
    -DSDL_TEST_LIBRARY=OFF \
    -DSDL_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON

# Build
cmake --build . -j$(sysctl -n hw.ncpu)

# Install to libs directory
cmake --install .

echo "SDL3 static library built and installed to: $INSTALL_DIR"
echo "Library: $INSTALL_DIR/lib/libSDL3.a"
echo "Headers: $INSTALL_DIR/include/SDL3/"
