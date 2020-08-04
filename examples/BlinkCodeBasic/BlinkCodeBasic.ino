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
