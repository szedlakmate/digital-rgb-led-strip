// Knob (potentiometer) input utility for Digital_RGB_LED
//
// This module provides a template function to map analog knob (potentiometer) readings
// to a desired variable range. It is designed for use with Arduino analog inputs.
//
// ──────────────────────────────────────────────────────────────
// CONFIGURATION:
//   All configuration is done in the config.h (or config_override.h) file.
//   You must define the analog pin(s) to use for each knob:
//     #define BRIGHTNESS_KNOB_PIN A0
//     #define BPM_KNOB_PIN A1
//     #define WAVE_LENGTH_SCALE_KNOB_PIN A2
//   (Pin numbers are examples; use your actual wiring.)
//
//   The expected voltage range for the knob can be set using:
//     KNOB_5V   // 1023 (default for 5V systems)
//     KNOB_3_3V // 675  (for 3.3V systems)
//   These are defined in config.h and can be used as the knobMax argument.
//
// ──────────────────────────────────────────────────────────────
// WIRING:
//   Typical potentiometer wiring:
//     One end  -> GND
//     Other end -> 5V (or 3.3V)
//     Wiper (middle pin) -> Arduino analog input (e.g., A0)
//
// ──────────────────────────────────────────────────────────────
// USAGE:
//   Use the calculateKnobValueForPin<T>() template to map the analog reading to your variable's range.
//   Example:
//     int brightness = calculateKnobValueForPin<int>(A0, 0, 255, 0, KNOB_5V);
//     float bpm = calculateKnobValueForPin<float>(A1, 0.01, 20.0, 0, KNOB_5V);
//
//   - knobPin: analog pin to read from
//   - varMin/varMax: the min/max value for the variable
//   - knobMin/knobMax: the min/max value the knob can output (default 0-1023)
//
//   This function does not mutate any global state.
//

#ifndef KNOB_H
#define KNOB_H

#include <Arduino.h>
#include "config.h"

// Returns the mapped knob value for the given pin and range, does not mutate any variable
// knobPin: analog pin to read from
// varMin/varMax: the min/max value for the variable
// knobMin/knobMax: the min/max value the knob can output (default 0-1023)
template<typename T>
T calculateKnobValueForPin(uint8_t knobPin, T varMin, T varMax, int knobMin = 0, int knobMax = KNOB_5V) {
  int knobValue = analogRead(knobPin);
  float normalized = (float)(knobValue - knobMin) / (float)(knobMax - knobMin);
  if (normalized < 0.0f) normalized = 0.0f;
  if (normalized > 1.0f) normalized = 1.0f;
  return (T)(varMin + normalized * (varMax - varMin));
}

#endif  // KNOB_H
