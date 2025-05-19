#!/bin/bash
set -e

APP_NAME="PANIC"
BUILD_DIR="build"
INSTALL_PREFIX="/usr/local"

echo "🔧 Building $APP_NAME..."

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE=Release
make -j"$(nproc)"

echo "📦 Installing to $INSTALL_PREFIX..."

sudo cmake --install . --prefix="$INSTALL_PREFIX"

# Install .desktop file if it exists
if [[ -f "../packaging/$APP_NAME.desktop" ]]; then
    echo "📁 Installing desktop entry..."
    sudo install -Dm644 "../packaging/$APP_NAME.desktop" "/usr/share/applications/$APP_NAME.desktop"
fi

# Install 32x32 icon if it exists
if [[ -f "../packaging/icons/32x32/apps/$APP_NAME.png" ]]; then
    echo "🖼️ Installing 32x32 icon..."
    sudo install -Dm644 "../packaging/icons/32x32/apps/$APP_NAME.png" "/usr/share/icons/hicolor/32x32/apps/$APP_NAME.png"
fi

echo "✅ $APP_NAME installed successfully!"
