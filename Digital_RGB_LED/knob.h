#ifndef KNOB_H
#define KNOB_H

#include <Arduino.h>

// Returns the mapped knob value for the given pin and range, does not mutate any variable
// knobPin: analog pin to read from
// varMin/varMax: the min/max value for the variable
// knobMin/knobMax: the min/max value the knob can output (default 0-1023)
template<typename T>
T calculateKnobValueForPin(uint8_t knobPin, T varMin, T varMax, int knobMin = 0, int knobMax = 1023) {
  int knobValue = analogRead(knobPin);
  float normalized = (float)(knobValue - knobMin) / (float)(knobMax - knobMin);
  if (normalized < 0.0f) normalized = 0.0f;
  if (normalized > 1.0f) normalized = 1.0f;
  return (T)(varMin + normalized * (varMax - varMin));
}

#endif  // KNOB_H
