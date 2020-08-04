#ifndef BLINK_CODE_DEFS_H
#define BLINK_CODE_DEFS_H

#include <Arduino.h>

namespace blinkcode {

constexpr unsigned long DEFAULT_TRIGGER_DELAY = 500UL;
constexpr unsigned long DEFAULT_PULSE_GAP_TIME = 500UL;
constexpr unsigned long DEFAULT_PULSE_SHORT_TIME = 100UL;
constexpr unsigned long DEFAULT_PULSE_LONG_TIME = 1000UL;

constexpr unsigned long (*getTickValue)() = millis;
constexpr void (*setGpioState)(int, bool) = digitalWrite;

};

#endif /* BLINK_CODE_DEFS_H */
