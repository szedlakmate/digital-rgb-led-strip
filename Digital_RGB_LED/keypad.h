#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>

struct KeypadButtonConfig {
  uint8_t pin;
  char label;      // e.g. 'U' for up, 'D' for down
  bool activeLow;  // true if button is active low
};

struct KeypadDebounceState {
  unsigned long lastChange = 0;
  bool lastState = false;
  bool reported = false;
};

extern KeypadButtonConfig keypadButtons[];
extern const size_t keypadButtonCount;

void setupKeypadPins();

// Example: read all buttons, return label of pressed button, or 0 if none
inline char readKeypadButton(const KeypadButtonConfig* configs, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    bool pressed = configs[i].activeLow ? digitalRead(configs[i].pin) == LOW : digitalRead(configs[i].pin) == HIGH;
    if (pressed) return configs[i].label;
  }
  return 0;
}

// Debounced one-shot read: returns label of the first debounced new press, or 0 if none
inline char readDebouncedKeypadButton(const KeypadButtonConfig* configs, KeypadDebounceState* states, size_t count, unsigned long debounceMs = 30) {
  unsigned long now = millis();
  for (size_t i = 0; i < count; ++i) {
    bool raw = configs[i].activeLow ? digitalRead(configs[i].pin) == LOW : digitalRead(configs[i].pin) == HIGH;
    if (raw != states[i].lastState) {
      states[i].lastChange = now;
      states[i].lastState = raw;
    }
    if (states[i].lastState && (now - states[i].lastChange) >= debounceMs) {
      if (!states[i].reported) {
        states[i].reported = true;
        return configs[i].label;
      }
    } else {
      states[i].reported = false;
    }
  }
  return 0;
}

#endif  // KEYPAD_H
