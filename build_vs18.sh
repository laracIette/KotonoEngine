#!/bin/bash

main() {
    # Stop if any command fails inside main
    set -e

    echo "🛠️ Build with editor? (y/n)"
    read -n 1 ans
    echo

    if [[ "$ans" == "y" ]]; then
        EDITOR="-DWITH_EDITOR=ON"
    else
        EDITOR="-DWITH_EDITOR=OFF"
    fi

    BUILD_DIR="build"

    echo "📁 Creating build folder at: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
    mkdir "$BUILD_DIR"
    cd "$BUILD_DIR"

    echo "⚙️ Running CMake from project root..."
    cmake .. -G "Visual Studio 18 2026" -A x64 $EDITOR
    echo "✅ CMake generation complete!"
}

# Run the script and catch errors
if ! main; then
    echo
    echo "❌ The script failed. Press Enter to exit."
    read
fi