# Addressable LED strip animations

![Build](https://github.com/szedlakmate/digital-rgb-led-strip/actions/workflows/build.yml/badge.svg)


## Description

In this project I controls a digital / addressable LED strip to handle different animations

## Setup

This project works with Arduino Mega and ESP8266. Wiring is pretty simple, and the first lines of the source code explain it. Wiring may vary between different versions of the code and different types of the used devices.

## Features

### Rolling animation

This animation consists a static pattern what is sliding infinitely on the LED strip. The following parameters can be configured:

#### special versions and attempts

- [Animation doubled](https://github.com/szedlakmate/digital-rgb-led-strip/tree/double_stripes) for symmetrical fun
- [Knobs](https://github.com/szedlakmate/digital-rgb-led-strip/blob/potmeter-3) controlling animation parameters: brightness / speed / length of the pattern
- [Bluetooth](https://github.com/szedlakmate/digital-rgb-led-strip/tree/bluetooth) based control, similarly to the knob based version
- [HTTP request based control](https://github.com/szedlakmate/digital-rgb-led-strip/tree/esp-8266)
- websocket based control


<img src="https://github.com/szedlakmate/digital-rgb-led-strip/assets/18262723/e38f261d-c5ab-4a61-8aa1-b6269f57a19f" width="310" height="410">

<img src="https://github.com/szedlakmate/digital-rgb-led-strip/assets/18262723/20f23b67-a6ae-4033-ac92-812d13a92f24" width="310" height="410">


### Sunset lamp

This version is a co-production with Rossana Ribeiro. In this project we built a a wall lamp with a slow smooth animation, imitating the lights and shades of sunset.

<img src="https://github.com/szedlakmate/digital-rgb-led-strip/assets/18262723/78d88a44-8ebf-4a6c-90e3-aeca3c72e1b1" width="420" height="340">
