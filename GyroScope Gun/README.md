# GyroScope Gun

GyroScope Gun is a PlatformIO/ESP32 project that turns an MPU-6050-based gyroscope module into a simple input device for pointer movement and button-like controls.

## What it does

- Reads motion data from an MPU-6050 sensor
- Converts gyroscope movement into cursor-style output
- Drives a digital output pin for a click/motor-style trigger
- Streams motion and button state over Serial for debugging or external use

## Hardware

- ESP32-C3 SuperMini
- MPU-6050 module
- Optional output pin for a trigger/motor signal

## Software

- PlatformIO
- Arduino framework
- Adafruit libraries for display support (if used by your build configuration)

## Getting started

1. Install PlatformIO
2. Open this folder in VS Code or your PlatformIO environment
3. Build and upload the firmware with:
   - `pio run`
   - `pio run --target upload`

## Notes

This repository is set up for a new project identity named GyroScope Gun while keeping the original remote available as `upstream`.
