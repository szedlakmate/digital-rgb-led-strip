// Debug logging utilities for Digital_RGB_LED
//
// This module provides lightweight debug print functions that are enabled or disabled
// at compile time using the DEBUG macro (set in config.h or config_override.h).
//
// ──────────────────────────────────────────────────────────────
// CONFIGURATION:
//   - To enable debug output, set DEBUG to 1 in your config_override.h or config.h:
//       #define DEBUG 1
//   - To disable debug output, set DEBUG to 0 (default).
//
// USAGE:
//   - Call dbg::begin() in setup() to initialize Serial (default 115200 baud).
//   - Use dbg::print(...) and dbg::println(...) for debug output (like Serial.print).
//   - When DEBUG is 0, all dbg::print/println calls are compiled out and do nothing.
//
// Example:
//   dbg::println("Hello, debug!");
//
#pragma once
#include "config.h"
#include <Arduino.h>

#ifndef DEBUG
#define DEBUG 0
#endif

namespace dbg {
constexpr bool enabled = DEBUG;

[[gnu::always_inline]] inline void begin(unsigned long baud = 115200) {
  Serial.begin(baud);
}

template<class... Ts>
[[gnu::always_inline]] inline void print(Ts&&... xs) {
  if constexpr (enabled) (Serial.print(xs), ...);
}

template<class... Ts>
[[gnu::always_inline]] inline void println(Ts&&... xs) {
  if constexpr (enabled) {
    (Serial.print(xs), ...);
    Serial.println();
  }
}
}
