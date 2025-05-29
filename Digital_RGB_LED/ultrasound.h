// Ultrasound distance sensor driver for Digital_RGB_LED
//
// This module provides a simple interface for using an HC-SR04 (or compatible) ultrasonic distance sensor.
//
// ──────────────────────────────────────────────────────────────
// CONFIGURATION:
//   All configuration is done in the config.h (or config_override.h) file.
//   You must define the following macros to enable the driver:
//     #define US_TRIG_PIN <pin>   // Arduino digital pin connected to sensor TRIG
//     #define US_ECHO_PIN <pin>   // Arduino digital pin connected to sensor ECHO
//   Example:
//     #define US_TRIG_PIN 7
//     #define US_ECHO_PIN 8
//
//   Additional parameters (see config.h):
//     US_MAX_DISTANCE_CM   // Maximum distance to measure (affects blocking time)
//     US_MIN_DISTANCE_CM   // Minimum distance to consider valid
//
// ──────────────────────────────────────────────────────────────
// WIRING:
//   HC-SR04 (or compatible) sensor:
//     VCC  -> 5V
//     GND  -> GND
//     TRIG -> Arduino digital pin (US_TRIG_PIN)
//     ECHO -> Arduino digital pin (US_ECHO_PIN)
//
//   Note: ECHO pin outputs 5V. If using a 3.3V microcontroller, use a voltage divider or level shifter.
//
// ──────────────────────────────────────────────────────────────
// USAGE:
//   Call ultrasoundSetup() in setup().
//   Call ultrasoundUpdate() in loop().
//   Use ultrasoundHasReading() to check for a new value.
//   Use ultrasoundRead_cm() to get the last measured distance in centimeters.
//
//   If the pins are not defined, all functions are stubbed and do nothing.
//
#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include <Arduino.h>

/* ─── Only build the real driver when both pins are defined ─── */
#if defined(US_TRIG_PIN) && defined(US_ECHO_PIN)


void ultrasoundSetup();       // call in setup()
void ultrasoundUpdate();      // call every loop()
bool ultrasoundHasReading();  // true when a fresh value is ready
float ultrasoundRead_cm();    // last distance in centimeters (cm)


#else
/* ─── Stubs: generate *no* code, keep the sketch compiling ─── */
inline void ultrasoundSetup() {}
inline void ultrasoundUpdate() {}
inline bool ultrasoundHasReading() {
  return false;
}
inline float ultrasoundRead_cm() {
  return 0.0f;
}

#endif
#endif  //ULTRASOUND_H
