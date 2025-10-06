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
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build targets
cmake --build . --config Release -j$(nproc)

# Copy compile_commands.json to project root
if [ -f compile_commands.json ]; then
  cp compile_commands.json ..
fi

# Return to project root
cd ..
