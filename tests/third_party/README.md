# Building and Using SDL3 Static Library

## Build SDL3

To build SDL3 as a static library for host testing, run:

```bash
./tests/third_party/scripts/build_sdl.sh
```

This script will:
1. Configure SDL3 with minimal options (static library only, no tests)
2. Build SDL3 in Release mode
3. Install the library and headers to `tests/third_party/libs/`

## Output Location

After building, you'll find:
- **Library**: `tests/third_party/libs/lib/libSDL3.a`
- **Headers**: `tests/third_party/libs/include/SDL3/`

## Using SDL3 in the Project

SDL3 is automatically linked when building with the host toolchain. The CMake configuration checks for the library and links it if available.

To use SDL3 in your code:

```cpp
#ifdef HOST_BUILD
#include <SDL3/SDL.h>
#endif
```

## Build Requirements

### macOS
- CMake 3.16 or higher
- Xcode Command Line Tools
- System frameworks (automatically linked)

### Ubuntu/Linux
- CMake 3.16 or higher
- GCC or Clang
- Development packages:
  ```bash
  sudo apt-get install libx11-dev libxext-dev libxrandr-dev \
    libxrender-dev libxinerama-dev libxi-dev libxcursor-dev \
    libasound2-dev libpulse-dev
  ```

## CMake Integration

The SDL3 library is integrated via `tests/third_party/cmake/sdl3.cmake`:
- Only included when `HOST_TOOLCHAIN` is set
- Creates an imported target `SDL3::SDL3`
- Automatically links required system frameworks/libraries
- Warns if the library hasn't been built yet

## Rebuilding SDL3

To rebuild SDL3 (e.g., after updating the SDL source):

```bash
rm -rf tests/third_party/sdl/build
rm -rf tests/third_party/libs
./tests/third_party/scripts/build_sdl.sh
```

## Build Options

The build script uses these SDL3 options:
- `SDL_SHARED=OFF` - Don't build shared library
- `SDL_STATIC=ON` - Build static library
- `SDL_TEST_LIBRARY=OFF` - Don't build test library
- `SDL_TESTS=OFF` - Don't build tests
- `CMAKE_POSITION_INDEPENDENT_CODE=ON` - Enable PIC for static library

To customize, edit `tests/third_party/scripts/build_sdl.sh`.
