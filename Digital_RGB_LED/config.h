#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>

// Pins
#define LED_PIN 31  // arduino digital pin
#define NUM_LEDS 300    // total num of leds blocks on the strip
#define BRIGHTNESS 150  // max: 250
#define COLOR_ORDER GRB

// Animation parameters
#define BPM 7.5
#define WAVE_LENGTH_SCALE 0.7
#define RESOLUTION 1
#define REVERSED true

extern CRGB leds[NUM_LEDS];
extern CRGBPalette256 currentPalette;
extern TBlendType currentBlending;

#endif // CONFIG_H
