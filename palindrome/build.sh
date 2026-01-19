# !bin/bash
# set -e
#
# BUILD_DIR=build
# BUILD_TYPE=${BUILD_TYPE:-Debug}
#
# if [ "$1" = "clean" ]; then
#   echo "Clean build directory"
#   rm -rf "$BUILD_DIR"
# fi
#
# if [ "$1" = "Release" ]; then
#   echo "Building Release"
#   BUILD_TYPE=-Release
# else
#   echo "Building Debug"
# fi
#
# cmake -S . -B "$BUILD_DIR" \
#   -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
# cmake --build "$BUILD_DIR" --parallel

#!/usr/bin/env sh
set -e

BUILD_DIR=build
BUILD_TYPE=Debug
DO_CLEAN=0

for arg in "$@"; do
    case "$arg" in
        clean)
            DO_CLEAN=1
            ;;
        release|Release)
            BUILD_TYPE=Release
            ;;
        debug|Debug)
            BUILD_TYPE=Debug
            ;;
        *)
            echo "Unknown argument: $arg"
            exit 1
            ;;
    esac
done

if [ "$DO_CLEAN" -eq 1 ]; then
    echo "Cleaning build directory"
    rm -rf "$BUILD_DIR"
fi

echo "Building ${BUILD_TYPE}"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build "$BUILD_DIR" --parallel


