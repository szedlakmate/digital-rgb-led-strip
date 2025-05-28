#include "ultrasound.h"
#define NEWPING_NO_TIMER
#include <NewPing.h>
#include "config.h"

constexpr uint8_t  MAX_CM          = 50;   // working range
constexpr unsigned long LOOP_DELAY = 60;   // ms between pings

static NewPing sonar(US_TRIG_PIN, US_ECHO_PIN, MAX_CM);

static unsigned long nextPing = 0;
static float         last_mm  = 0;
static bool          fresh    = false;

void ultrasoundSetup() { nextPing = millis(); }

void ultrasoundUpdate() {
  if ((long)(millis() - nextPing) >= 0) {
    nextPing += LOOP_DELAY;
    unsigned int cm = sonar.ping_cm();     // blocks ≤ 200 µs
    if (cm) {                              // 0 means out-of-range
      last_mm = cm * 10.0f;
      fresh   = true;
    }
  }
}

bool  ultrasoundHasReading() { return fresh; }
float ultrasoundRead_mm()   { fresh = false; return last_mm; }
