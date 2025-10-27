#!/bin/bash

# Stop if any command fails
set -e

# Define build directory
BUILD_DIR="build"

# Remove old build folder if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "🧹 Removing existing build folder..."
    rm -rf "$BUILD_DIR"
fi

# Recreate and move into it
echo "📁 Creating build folder..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake configuration
echo "⚙️  Running CMake..."
cmake .. -G "Visual Studio 17 2022" -A x64

echo "✅ CMake generation complete!"