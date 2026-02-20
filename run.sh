#!/bin/bash
# Run script for Circe Engine
# Usage: ./run.sh [debug|release]

BUILD_TYPE="release"

# Parse arguments
if [ $# -gt 0 ]; then
    case $1 in
        debug|Debug|DEBUG)
            BUILD_TYPE="debug"
            ;;
        release|Release|RELEASE)
            BUILD_TYPE="release"
            ;;
        *)
            echo "Usage: $0 [debug|release]"
            exit 1
            ;;
    esac
fi

EXECUTABLE="build/${BUILD_TYPE}/bin/Game"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable not found: $EXECUTABLE"
    echo "Run ./build.sh ${BUILD_TYPE} first"
    exit 1
fi

echo "Running $BUILD_TYPE build..."
"$EXECUTABLE"
