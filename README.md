# Addressable LED Strip Animations

[![Build (master)](https://github.com/sedlakmate/digital-rgb-led-strip/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/szedlakmate/digital-rgb-led-strip/actions/workflows/build.yml)

## About This Project

This is the personal love-project of **Máté Sedlák** ([@sedlakmate](https://github.com/sedlakmate)).

The goal is to create a highly customizable, interactive LED strip controller for addressable LEDs, with a focus on modularity, real-time control, and fun experimentation. In the future, I plan to add support for controlling the LEDs with phones and other smart devices.

## Overview

This project controls a digital (addressable) LED strip, such as WS2812B (NeoPixel), to display a variety of animated patterns. It is designed for Arduino Mega and supports real-time parameter control via potentiometer knobs and optional interaction with an ultrasonic distance sensor.

## Features

- Multiple color palettes and smooth animation effects
- Real-time adjustment of brightness, animation speed (BPM), and pattern length via analog knobs
- Optional ultrasound sensor for interactive effects (e.g., proximity-based brightness)
- Modular configuration via `config.h` and local overrides in `config_override.h`
- Debug logging (enable/disable in config)

## Hardware Requirements

- Arduino Mega 2560 (or compatible)
- WS2812B (NeoPixel) or compatible addressable LED strip
- Potentiometers (for brightness, BPM, and pattern length control; optional)
- HC-SR04 or compatible ultrasonic distance sensor (optional)

## Setup

1. **Install Dependencies**
   - [FastLED](https://github.com/FastLED/FastLED) (avoid version 3.9.18, which is incompatible)
   - [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)

2. **Configure Hardware**
   - Edit `config.h` to set pin assignments and animation defaults.
   - For local or per-device changes, copy `config_override_template.h` to `config_override.h` and override only the needed settings.
   - See comments in `config.h` and module headers for wiring details.

3. **Build and Upload**
   - Open the project in the Arduino IDE or use Arduino CLI.
   - Select the correct board (Arduino Mega 2560).
   - Compile and upload the sketch.

## Usage

- Adjust the connected knobs to control brightness, animation speed (BPM), and pattern length in real time.
- If an ultrasound sensor is connected, approach or move away to interactively change effects (e.g., brightness).
- Enable debug output by setting `#define DEBUG 1` in your `config_override.h`.

## File Structure

- `Digital_RGB_LED.ino` — Main entry point
- `animation.*` — Animation logic
- `palette.*` — Color palette definitions
- `knob.h` — Analog knob mapping utilities
- `ultrasound.*` — Ultrasound sensor driver
- `debug.h` — Debug logging utilities
- `config.h` — Main configuration
- `config_override_template.h` — Template for local overrides

## Notes

- Only define the pins/features you use in your configuration; unused features are automatically stubbed out.
- Do not commit `config_override.h` to version control (see `.gitignore`).

## License

See [LICENSE](LICENSE) for details.
