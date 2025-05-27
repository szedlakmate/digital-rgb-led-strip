// Main configuration file for the project.
// To override anything locally, create a `config_override.h` in the same folder
// and define only the parameters you want to change. Do NOT track that file
// in version control.

#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>
#include "palette.h"  // brings in PREDEFINED_PALETTES + typedefs

/* ─────────── Hardware pins ─────────── */
#define LED_PIN 31      // Arduino digital pin
#define NUM_LEDS 300    // total LEDs on the strip
#define BRIGHTNESS 220  // 0-255
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

/* ─────────── Animation defaults ─────── */
#define BPM 5.0
#define WAVE_LENGTH_SCALE 1.0
#define RESOLUTION 1
#define REVERSED false
#define PALETTE_INDEX 6

/* ─────────── Debug flag ─────────────── */
#define DEBUG 0  // set to 1 in config_override.h for verbose logs

/* ─────────── Globals (defined in .ino) ─ */
extern CRGB leds[NUM_LEDS];
extern CRGBPalette256 currentPalette;
extern TBlendType currentBlending;

/* palette registry (table lives in palette.cpp, pointer lives in .ino) */
extern const TProgmemRGBPalette16* const* gPalettes;  // pointer to table
extern const uint8_t gPaletteCount;
extern uint8_t gPaletteIndex;

/* ─────────── Local overrides ─────────── */
#ifdef __has_include
#if __has_include("config_override.h")
#include "config_override.h"
#endif
#endif

#endif  // CONFIG_H
