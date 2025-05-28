#pragma once
#include <Arduino.h>

/* ─── Only build the real driver when both pins are defined ─── */
#if defined(US_TRIG_PIN) && defined(US_ECHO_PIN)

void  ultrasoundSetup();          // call once in setup()
void  ultrasoundUpdate();         // call every loop()
bool  ultrasoundHasReading();     // true after a new ping
float ultrasoundRead_mm();        // distance in millimetres

#else
/* ─── Stubs: generate *no* code, keep the sketch compiling ─── */
inline void  ultrasoundSetup()          {}
inline void  ultrasoundUpdate()         {}
inline bool  ultrasoundHasReading()     { return false; }
inline float ultrasoundRead_mm()        { return 0.0f;  }

#endif
