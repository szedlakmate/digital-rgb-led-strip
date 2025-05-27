// Main configuration file for the project.
// To override any settings locally, create a `config_override.h` file in the same directory.
// Only include the parameters you want to change in `config_override.h`.
// This allows you to keep local changes out of version control and maintain a clean git history.

#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>

// Pins
#define LED_PIN 31  // arduino digital pin
#define NUM_LEDS 300    // total num of leds blocks on the strip
#define BRIGHTNESS 220  // max: 250
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Animation parameters
#define BPM 5.0
#define WAVE_LENGTH_SCALE 1.0
#define RESOLUTION 1
#define REVERSED false

// Debug flag
#define DEBUG 0

extern CRGB leds[NUM_LEDS];
extern CRGBPalette256 currentPalette;
extern TBlendType currentBlending;

// --- Add this block at the end ---
#ifdef __has_include
#  if __has_include("config_override.h")
#    include "config_override.h"
#  endif
#endif

#endif // CONFIG_H
