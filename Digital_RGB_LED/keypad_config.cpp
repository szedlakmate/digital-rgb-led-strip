#include "keypad.h"

KeypadButtonConfig keypadButtons[] = {
  { 2, 'U', true },  // Pin 2, 'Up', active low
  { 3, 'D', true }   // Pin 3, 'Down', active low
};
const size_t keypadButtonCount = sizeof(keypadButtons) / sizeof(KeypadButtonConfig);

void setupKeypadPins() {
  for (size_t i = 0; i < keypadButtonCount; ++i) {
    pinMode(keypadButtons[i].pin, keypadButtons[i].activeLow ? INPUT_PULLUP : INPUT);
  }
}
