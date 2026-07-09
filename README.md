![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Bluetooth](https://img.shields.io/badge/Bluetooth-BLE%20HID-informational)

# ESP32 BLE Dual Joystick Game Controller

A DIY Bluetooth game controller built using an ESP32 development board. This project uses the ESP32 BLE Gamepad library to emulate a standard Bluetooth HID gamepad with dual analog joysticks and multiple push buttons.

## Features

- Bluetooth Low Energy (BLE) HID Gamepad
- Dual Analog Joysticks
- 9 Digital Push Buttons
- L3 and R3 Joystick Click Buttons
- Debounced Button Inputs
- Analog Deadzone Filtering
- Compatible with Windows, Android, Linux and other BLE-supported devices
- Wireless Low-Latency Gameplay

## Hardware

- ESP32 DevKit V1
- 2 × Analog Joystick Modules
- 9 × Push Buttons
- Breadboard
- Jumper Wires
- USB Cable

## Wiring Diagram

![Wiring Diagram](wiring_diagram.svg)

## Pin Configuration

### Left Joystick

| Function | GPIO |
|----------|------|
| X Axis | 34 |
| Y Axis | 35 |
| L3 Button | 32 |

### Right Joystick

| Function | GPIO |
|----------|------|
| X Axis | 33 |
| Y Axis | 36 |
| R3 Button | 25 |

### Push Buttons

| Button | GPIO |
|--------|------|
| Button 1 | 23 |
| Button 2 | 22 |
| Button 3 | 21 |
| Button 4 | 19 |
| Button 5 | 18 |
| Button 6 | 27 |
| Button 7 | 14 |
| Button 8 | 12 |
| Button 9 | 13 |

All digital pins use the ESP32's internal pull-up resistors (`INPUT_PULLUP`), so no external resistors are required. Each button only needs one leg wired to its GPIO and the diagonally opposite leg wired to any GND pin.

## Libraries

Install both through Arduino IDE → Sketch → Include Library → Manage Libraries:

- [ESP32 BLE Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad) by lemmingDev
- NimBLE-Arduino (dependency, installs automatically if prompted)

## Installation & Setup

1. Install the **Arduino IDE** (1.8.x or 2.x).
2. Add ESP32 board support: File → Preferences → Additional Board Manager URLs → add
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   Then Tools → Board → Boards Manager → search "esp32" → Install.
3. Install the libraries listed above via Library Manager.
4. Clone this repository or download the `.ino` file.
5. Open the sketch in Arduino IDE.
6. Select **Tools → Board → ESP32 Dev Module**.
7. Select the correct **COM port** for your board.
8. Wire the hardware according to the Pin Configuration table above.
9. Click **Upload**.

## Usage

1. Power the ESP32 (via USB or battery).
2. On your PC/phone/tablet, open Bluetooth settings and pair with **"ESP32 Gamepad"**.
3. Once connected, verify all inputs at [gamepad-tester.com](https://gamepad-tester.com) — every button and joystick axis should respond live on screen.
4. Open any game or emulator that supports generic HID gamepads and start playing.

> **Note:** Some games (especially older titles built only for Xbox controllers) require XInput specifically. For those, a tool like [x360ce](https://www.x360ce.com/) can translate this controller's signal into an Xbox-compatible one.

## How It Works

The ESP32 reads the analog joystick values and digital button states, then sends them as Bluetooth HID gamepad reports. Once paired, the device behaves like a standard wireless game controller. Button presses are software-debounced, and joystick axes have a small deadzone filter to eliminate idle jitter — both configurable in the code (`debounceDelay` and `DEADZONE` constants).

## Future Improvements

- Battery-powered operation
- USB-C charging
- OLED status display
- Vibration (Haptic Feedback)
- Trigger buttons (L2/R2)
- RGB LED indicators
- Custom 3D Printed Enclosure

## License

MIT License
