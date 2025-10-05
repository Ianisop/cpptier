#!/usr/bin/env bash
set -e

# Build directory
BUILD_DIR="build"

# Create or clean build directory
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build targets
cmake --build . --config Release -j$(nproc)

# Return to project root
cd ..
