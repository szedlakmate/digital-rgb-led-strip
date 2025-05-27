#pragma once
#include "config.h"
#include <Arduino.h>

#ifndef DEBUG
#   define DEBUG 0
#endif

namespace dbg {
    constexpr bool enabled = DEBUG;

    [[gnu::always_inline]]
    inline void begin(unsigned long baud = 115200) { Serial.begin(baud); }

    template<class... Ts>
    [[gnu::always_inline]]
    inline void print(Ts&&... xs) {
        if constexpr (enabled) (Serial.print(xs), ...);
    }

    template<class... Ts>
    [[gnu::always_inline]]
    inline void println(Ts&&... xs) {
        if constexpr (enabled) { (Serial.print(xs), ...); Serial.println(); }
    }
}
