# GyroScope Gun 🔫🎮

![Hardware Hacking](https://img.shields.io/badge/Hardware-Hacking-ff69b4.svg)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange.svg)
![Python](https://img.shields.io/badge/Python-3.x-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

**GyroScope Gun** is a custom hardware-hacking project that transforms a modified toy gun casing into a fully functional, low-latency PC arcade light gun. By combining an ESP32 microcontroller, an MPU-6050 gyroscope, and a Python-based bridge script, this project translates physical hand movements into precise 2D mouse coordinates. 

Perfect for reliving the nostalgic arcade experience with PC games like *The House of the Dead 2*.

## ✨ Features

- **Precise Motion Tracking:** Uses the MPU-6050 to map physical wrist/arm movements to on-screen cursor movements.
- **Immersive Haptic Feedback:** Salvaged PlayStation vibration motor integrated into the toy casing provides instant physical feedback upon pulling the trigger.
- **Low-Latency PC Integration:** High-speed serial communication (100Hz loop) bridged by a custom Python script using `PyAutoGUI` and `AHK` for system-level mouse control.
- **Auto-Calibration:** Firmware automatically calculates sensor offsets on startup for accurate drift-free aiming.

---

## 🛠️ Hardware Requirements

- **ESP32 Microcontroller** (Tested on NodeMCU ESP32-S / ESP32-C3 SuperMini)
- **MPU-6050** (Gyroscope + Accelerometer module)
- **Salvaged Vibration Motor** (e.g., from an old PlayStation controller)
- **Mechanical Buttons/Switches** (For Left and Right clicks)
- **Transistor/MOSFET & Diode** (Recommended for safely driving the vibration motor)
- Modified Toy Gun Casing

### 🔌 Wiring Guide

Based on the `main.cpp` configuration, connect your components as follows:

| Component       | ESP32 Pin | Note                                      |
|-----------------|-----------|-------------------------------------------|
| **MPU-6050 SDA**| `GPIO 21` | I2C Data                                  |
| **MPU-6050 SCL**| `GPIO 22` | I2C Clock                                 |
| **Trigger (L-Click)**| `GPIO 33` | Connect to Ground (Uses `INPUT_PULLUP`)   |
| **Alt Fire (R-Click)**| `GPIO 25` | Connect to Ground (Uses `INPUT_PULLUP`)   |
| **Haptic Motor**| `GPIO 26` | `OUTPUT` (Drive via Transistor)           |

---

## 💻 Software Setup & Installation

### 1. Firmware (ESP32)
The microcontroller firmware is developed using **PlatformIO** and the Arduino Framework.

1. Install [VS Code](https://code.visualstudio.com/) and the [PlatformIO Extension](https://platformio.org/).
2. Clone this repository and open the folder in VS Code.
3. The necessary library (`electroniccats/MPU6050`) will be installed automatically via `platformio.ini`.
4. Connect your ESP32 to your PC and click the **Upload** button in PlatformIO.

### 2. PC Control Script (Python)
The Python script reads the serial data and translates it into mouse movements and clicks.

1. Ensure you have **Python 3.x** installed.
2. Install the required Python dependencies:
   ```bash
   pip install pyserial pyautogui pydirectinput ahk
   ```
3. Ensure you have [AutoHotkey (AHK)](https://www.autohotkey.com/) installed on your system (used for bypass-level game clicking).

---

## 🚀 Usage Instructions

1. **Power up the Gun:** Plug the ESP32 into your PC via USB. Keep the gun steady on a flat surface for a few seconds during startup so the firmware can calibrate the MPU-6050 offsets.
2. **Check the COM Port:** Open Device Manager on Windows to see which COM port the ESP32 is connected to (e.g., `COM8`).
3. **Update the Script:** Open `Mouse Control.py` and update the serial port line if necessary:
   ```python
   ser = serial.Serial('COM8', 115200) # Change 'COM8' to your actual port
   ```
4. **Run the Bridge Script:**
   ```bash
   python "Mouse Control.py"
   ```
5. **Play!** Aim the gun to move your mouse cursor. Pull the trigger to click (and feel the vibration!).

---

## 📝 License
This project is licensed under the MIT License. See the `LICENSE` file for details.