#!/bin/bash

ROOT_DIR="$(dirname "$(dirname "$(realpath "$0")")")"
CMAKE_DIR="$ROOT_DIR/cmake"

TOOLCHAIN_FILE="$CMAKE_DIR/arduino_toolchain.cmake"
BUILD_DIR="$ROOT_DIR/build-arduino"


ARDUINO_DIR="$HOME/Library/Arduino15"
AVRDUDE_PATH="$ARDUINO_DIR/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude"
AVRDUDE_CONFIG="$ARDUINO_DIR/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf"


function log_header() {
    echo "=================================================================="
    echo "          $1"
    echo "=================================================================="
}

function build() {
    log_header "Building navman..."
    
    build_variant=Release

    while [[ "$1" == -* ]]; do
        case "$1" in
            --debug)
                build_variant=Debug
                ;;
            --release)
                build_variant=Release
                ;;
            *)
                echo "Unknown option: $1"
                exit 1
                ;;
        esac
        shift
    done

    mkdir -p "$BUILD_DIR"

    cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -DCMAKE_BUILD_TYPE="$build_variant"
    cmake --build "$BUILD_DIR" 
}

function flash() {
    log_header "Flashing navman to Arduino..."

    local port="/dev/cu.usbserial-1140"
    local baud="115200"
    local programmer="wiring"
    local hex_file="$BUILD_DIR/navman-multi-3100s.hex"

    while [[ "$1" == -* ]]; do
        case "$1" in
            --port|-p)
                shift
                port="$1"
                ;;
            --baud|-b)
                shift
                baud="$1"
                ;;
            *)
                echo "Unknown option: $1"
                exit 1
                ;;
        esac
        shift
    done

    if [ ! -f "$hex_file" ]; then
        echo "Error: $hex_file not found. Build first with: $0 build-arduino"
        exit 1
    fi

    echo "Port:       $port"
    echo "Baud:       $baud"
    echo "Programmer: $programmer"
    echo "Hex file:   $hex_file"
    echo ""

    "$AVRDUDE_PATH" \
        -C"$AVRDUDE_CONFIG" \
        -v \
        -patmega2560 \
        -c"$programmer" \
        -P"$port" \
        -b"$baud" \
        -D \
        -Uflash:w:"$hex_file":i
}

function clean() {
    log_header "Cleaning build directories..."
    
    if [ -d "$ROOT_DIR/build-host" ]; then
        echo "Removing build-host..."
        rm -rf "$ROOT_DIR/build-host"
    fi
    
    if [ -d "$ROOT_DIR/build-arduino" ]; then
        echo "Removing build-arduino..."
        rm -rf "$ROOT_DIR/build-arduino"
    fi
    
    echo "Clean complete!"
}

if [ "$0" == "$BASH_SOURCE" ]; then
    cmd="$1"
    shift

    build=0

    case "$cmd" in
        "build-host")
            build=1
            TOOLCHAIN_FILE="$CMAKE_DIR/host_toolchain.cmake"
            BUILD_DIR="${ROOT_DIR}/build-host"
            ;;
        "build-arduino")
            build=1
            TOOLCHAIN_FILE="$CMAKE_DIR/arduino_toolchain.cmake"
            BUILD_DIR="${ROOT_DIR}/build-arduino"
            ;;
        "flash")
            flash "$@"
            ;;
        "clean")
            clean
            ;;
        *)
            echo "Unknown command: $cmd"
            echo "Usage: $0 <command> [args]"
            echo "Commands:"
            echo "  build-host       Build navman for host"
            echo "  build-arduino    Build navman for Arduino"
            echo "  flash            Flash navman to Arduino"
            echo "  clean            Clean all build directories"
            ;;
    esac

    if [ $build -eq 1 ]; then
        build "$@"
    fi
fi