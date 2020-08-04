# BlinkCode Library
BlinkCode Library is used for indicating flashing fault codes or various numeric settings.
It uses long pulses for indicating tens and short pulses for indicating onces.
Library is implemented in C++ with build-in Arduino port, but it is easy to
port to different architecture.

## Features
* asynchronous non-blocking API with background processing
* hardware independent high-level architecture
* works well with digital, analogs and other signal output backends
* easy to use with Arduino boards
* support 1-99 codes numbers
* configurable pulse times

## Example

Basic code trigger:

```cpp
#include <Arduino.h>

#include <BlinkCode.h>

constexpr int LED_PIN = 13;

using namespace blinkcode;

BlinkCode code(LED_PIN);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led

    code.begin();                       // Initialize and register blink code object
    code.trigger(12);                   // Start flash code - single long, double short
}

void loop()
{
    BlinkCode::service();               // Asynchronous service routine, should be called periodically
}
```
