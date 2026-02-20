#!/bin/bash
# Build script for Circe Engine
# Usage: ./build.sh [debug|release] [clean]

BUILD_TYPE="Release"
CLEAN=0

# Parse arguments
for arg in "$@"; do
    case $arg in
        debug|Debug|DEBUG)
            BUILD_TYPE="Debug"
            ;;
        release|Release|RELEASE)
            BUILD_TYPE="Release"
            ;;
        clean)
            CLEAN=1
            ;;
        *)
            echo "Unknown argument: $arg"
            echo "Usage: $0 [debug|release] [clean]"
            exit 1
            ;;
    esac
done

BUILD_DIR="build/${BUILD_TYPE,,}"

echo "==============================================="
echo "Building Circe Engine"
echo "Build Type: $BUILD_TYPE"
echo "Build Directory: $BUILD_DIR"
echo "==============================================="

# Clean if requested
if [ $CLEAN -eq 1 ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Configure
echo "Configuring..."
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Build
echo "Building..."
cmake --build "$BUILD_DIR" -j$(nproc)

# Check result
if [ $? -eq 0 ]; then
    echo ""
    echo "==============================================="
    echo "Build successful!"
    echo "Executable: $BUILD_DIR/bin/Game"
    echo "Library: $BUILD_DIR/lib/libCirce.so"
    echo "==============================================="
else
    echo ""
    echo "Build failed!"
    exit 1
fi
