#include "debug.h"
#include "ultrasound.h"
#include "config.h"

#if defined(US_TRIG_PIN) && defined(US_ECHO_PIN)

constexpr unsigned long MIN_PERIOD_MS = 60;   // sensor spec
constexpr unsigned long MAX_ECHO_US = 30000;  // 5 m timeout
constexpr float US_TO_MM = 0.170f;            // 1 µs → 0.17 mm

enum class UsState : uint8_t { IDLE,
                               TRIG,
                               WAIT_HIGH,
                               WAIT_LOW };

static UsState state = UsState::IDLE;
static unsigned long tState = 0;  // micros or millis depending on state
static float last_mm = 0;
static bool fresh = false;

void ultrasoundSetup() {
  dbg::println("Starting ultrasound device");
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);
  digitalWrite(US_TRIG_PIN, LOW);
  state = UsState::IDLE;
  tState = millis();
}

void ultrasoundUpdate() {
  switch (state) {

    case UsState::IDLE:  // dwell between pings
      if ((long)(millis() - tState) >= MIN_PERIOD_MS) {
        digitalWrite(US_TRIG_PIN, HIGH);
        tState = micros();
        state = UsState::TRIG;
      }
      break;

    case UsState::TRIG:  // 10 µs trigger pulse
      if ((long)(micros() - tState) >= 10) {
        digitalWrite(US_TRIG_PIN, LOW);
        state = UsState::WAIT_HIGH;
      }
      break;

    case UsState::WAIT_HIGH:  // wait for echo HIGH
      if (digitalRead(US_ECHO_PIN)) {
        tState = micros();
        state = UsState::WAIT_LOW;
      } else if ((long)(micros() - tState) >= MAX_ECHO_US) {
        state = UsState::IDLE;  // timeout
        tState = millis();
      }
      break;

    case UsState::WAIT_LOW:
      {  // measure HIGH time
        unsigned long width = micros() - tState;
        if (!digitalRead(US_ECHO_PIN)) {
          last_mm = width * US_TO_MM;
          fresh = true;
          state = UsState::IDLE;
          tState = millis();
        } else if (width >= MAX_ECHO_US) {  // safety timeout
          state = UsState::IDLE;
          tState = millis();
        }
      }
      break;
  }
}

bool ultrasoundHasReading() {
  return fresh;
}
float ultrasoundRead_mm() {
  fresh = false;
  return last_mm;
}

#endif  // pins defined
