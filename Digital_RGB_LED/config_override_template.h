// This file allows you to locally override configuration parameters from config.h.
// Only include the settings you want to change. Do not track this file in version control.

#ifndef CONFIG_OVERRIDE_H
#define CONFIG_OVERRIDE_H

#undef DEBUG
#define DEBUG 1

/* ──────────── Hardware pins ──────────── */
/* If pins are defined, the corresponding device gets loaded */
#define US_TRIG_PIN 7
#define US_ECHO_PIN 8
#define BRIGHTNESS_KNOB_PIN A0
#define BPM_KNOB_PIN A0
#define WAVE_LENGTH_SCALE_KNOB_PIN A0

// Animation parameters
#define BRIGHTNESS 220  // max: 255
#define BPM 3.0
#define WAVE_LENGTH_SCALE 1.5
#define RESOLUTION 1
#define REVERSED false

#endif  // CONFIG_OVERRIDE_H
