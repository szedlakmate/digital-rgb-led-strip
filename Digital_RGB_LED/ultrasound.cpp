#include "debug.h"
#include "ultrasound.h"
#include <NewPing.h>
#include "config.h"

#if defined(US_TRIG_PIN) && defined(US_ECHO_PIN)

// MAX_CM determines blocking time. See: Configuration -> "Sensor config"
constexpr uint8_t MAX_CM = US_MAX_DISTANCE_CM + 5;  // working range
constexpr unsigned long LOOP_DELAY = 60;            // ms between pings

static NewPing sonar(US_TRIG_PIN, US_ECHO_PIN, MAX_CM);

static unsigned long nextPing = 0;
static float last_cm = 0;
static bool fresh = false;

void ultrasoundSetup() {
  dbg::println("Initializing ultrasound device");
  nextPing = millis();
}

void ultrasoundUpdate() {
  if ((long)(millis() - nextPing) >= 0) {
    nextPing += LOOP_DELAY;
    unsigned int cm = sonar.ping_cm();  // blocking call
    if (cm) {                           // 0 means out-of-range
      last_cm = cm;
      fresh = true;
    }
  }
}

bool ultrasoundHasReading() {
  return fresh;
}
float ultrasoundRead_cm() {
  fresh = false;
  return last_cm;
}

#endif
