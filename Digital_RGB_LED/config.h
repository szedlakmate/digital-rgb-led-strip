// Main configuration file for the project.
// To override anything locally, create a `config_override.h` in the same folder
// and define only the parameters you want to change. Do NOT track that file
// in version control.

#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>
#include "palette.h"  // brings in PREDEFINED_PALETTES + typedefs

/* ─────────── Debug flag ─────────────── */
#define DEBUG 0  // set to 1 in config_override.h for verbose logs

/* ──────────── Hardware pins ──────────── */
/* If pins are defined, the corresponding device gets loaded */
#define LED_PIN 31  // Arduino digital pin
// #define US_TRIG_PIN 7  // Ultrasound
// #define US_ECHO_PIN 8  // Ultrasound
// #define BRIGHTNESS_KNOB_PIN A0
// #define BPM_KNOB_PIN A0
// #define WAVE_LENGTH_SCALE_KNOB_PIN A0

/* ────────── LED configuration ────────── */
#define LED_TYPE WS2812B
#define LED_STRIP_COLOR_CORRECTION TypicalLEDStrip
#define COLOR_ORDER GRB
#define NUM_LEDS 300  // total LEDs on the strip

/* ─────────── Animation defaults ─────── */
#define BRIGHTNESS 220  // 0-255
#define BPM 5.0
#define WAVE_LENGTH_SCALE 1.0
#define RESOLUTION 1
#define REVERSED false
#define PALETTE_INDEX 6

/* ────────── Animation limits ────────── */
#define BRIGHTNESS_MIN 0
#define BRIGHTNESS_MAX 255
#define BRIGHTNESS_CHANGE_THRESHOLD 5
#define BPM_MIN 0.01
#define BPM_MAX 35.0
#define BPM_CHANGE_THRESHOLD 0.6
#define WAVE_LENGTH_SCALE_MIN 0.01
#define WAVE_LENGTH_SCALE_MAX 8.0
#define WAVE_LENGTH_SCALE_CHANGE_THRESHOLD 0.075

/* ────────── Sensor config ────────── */
// US_MAX_DISTANCE_CM Significantly influences peformance.
// Example for US_MAX_DISTANCE_CM = 50 cm
// MAX_CM = US_MAX_DISTANCE_CM + 5 = 55 cm; v.sound = 0.0343 cm/µs
// blocking time: 3.2 ms (if no echo is received)
// US_MAX_DISTANCE_CM = 40 => blocking time: 2.6 ms
// US_MAX_DISTANCE_CM = 30 => blocking time: 2.0 ms
#define US_MAX_DISTANCE_CM 40
#define US_MIN_DISTANCE_CM 5
#define KNOB_5V 1023
#define KNOB_3_3V 675

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
