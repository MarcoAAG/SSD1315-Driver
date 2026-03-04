#!/bin/bash
set -e

BUILD_DIR="build"
LOGFILE="$BUILD_DIR/build.log"
TARGET=${1:-all}

# --- Target clean especial ---
if [ "$TARGET" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    echo "Clean complete."
    exit 0
fi

mkdir -p "$BUILD_DIR"
exec > >(tee "$LOGFILE") 2>&1

echo "Compiling target: $TARGET ..."

pushd "$BUILD_DIR" > /dev/null

if [ ! -f "CMakeCache.txt" ]; then
    echo "Configuring cmake..."
    cmake .. -G "Unix Makefiles"
else
    echo "CMake already configured, skipping..."
fi

make "$TARGET" -j$(nproc)

popd > /dev/null

echo "Build of $TARGET complete."