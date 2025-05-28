#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include <Arduino.h>

/* ─── Only build the real driver when both pins are defined ─── */
#if defined(US_TRIG_PIN) && defined(US_ECHO_PIN)


void ultrasoundSetup();       // call in setup()
void ultrasoundUpdate();      // call every loop()
bool ultrasoundHasReading();  // true when a fresh value is ready
float ultrasoundRead_cm();    // last distance in millimetres (cm)


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
