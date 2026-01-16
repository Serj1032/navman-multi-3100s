# Navman Multi 3100s

## Build

Install arduino-cli
```bash
brew install arduino-cli
```

## Compile 
Use CMake to configure and build the project:
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arduino_toolchain.cmake
cmake --build .
```

## Flash

```bash
cmake -DAVRDUDE_PORT=/dev/cu.usbserial-XXXX ..
cmake --build . --target flash
```