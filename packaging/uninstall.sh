#!/bin/bash
set -e

APP_NAME="PANIC"

echo "🧹 Uninstalling $APP_NAME..."

sudo rm -f /usr/local/bin/$APP_NAME
sudo rm -f /usr/share/applications/$APP_NAME.desktop
sudo rm -f /usr/share/icons/hicolor/32x32/apps/$APP_NAME.png

echo "✅ $APP_NAME removed successfully."
