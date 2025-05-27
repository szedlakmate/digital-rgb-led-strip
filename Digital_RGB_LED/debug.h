#pragma once
#include "config.h"
#include <Arduino.h>

// Make sure DEBUG is always 0 or 1 so it can be used in constexpr
#ifndef DEBUG
#   define DEBUG 0
#endif

namespace dbg {
    // compile-time flag
    constexpr bool enabled = DEBUG;

    inline void begin(unsigned long baud = 115200) {
        if constexpr (enabled) Serial.begin(baud);
    }

    template<class... Ts>
    inline void print(Ts&&... xs) {
        if constexpr (enabled) (Serial.print(xs), ...);
    }

    template<class... Ts>
    inline void println(Ts&&... xs) {
        if constexpr (enabled) { (Serial.print(xs), ...); Serial.println(); }
    }
}
