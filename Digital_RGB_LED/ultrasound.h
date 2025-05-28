#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include <Arduino.h>

/* ------------ pin selection (override in config_override.h if you like) ---------- */

void  ultrasoundSetup();                // call in setup()
void  ultrasoundUpdate();               // call every loop()
bool  ultrasoundHasReading();           // true when a fresh value is ready
float ultrasoundRead_mm();              // last distance in millimetres (mm)


#endif //ULTRASOUND_H
