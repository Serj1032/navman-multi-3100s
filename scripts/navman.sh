#!/bin/bash

ROOT_DIR="$(dirname "$(dirname "$(realpath "$0")")")"
CMAKE_DIR="$ROOT_DIR/cmake"

TOOLCHAIN_FILE="$CMAKE_DIR/arduino_toolchain.cmake"
BUILD_DIR="$ROOT_DIR/build-arduino"

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

    cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE"
    cmake --build "$BUILD_DIR" --config "$build_variant"
}

function flash() {
    log_header "Flashing navman to Arduino..."

    port="/dev/cu.usbserial-140"

    while [[ "$1" == -* ]]; do
        case "$1" in
            --port|-p)
                shift
                port="$1"
                ;;
            *)
                echo "Unknown option: $1"
                exit 1
                ;;
        esac
        shift
    done

    cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DAVRDUDE_PORT="$port" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE"
    cmake --build "$BUILD_DIR" --target flash
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