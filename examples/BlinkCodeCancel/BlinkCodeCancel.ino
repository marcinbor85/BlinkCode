#include <Arduino.h>

#include <BlinkCode.h>

constexpr int LED_PIN = 13;

using namespace blinkcode;

BlinkCode code(LED_PIN);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led

    code.begin();                       // Initialize and register blink code object
    code.trigger(99);                   // Start flash code
}

void loop()
{
    static bool cancelled = false;

    if (cancelled == false) {
        if (millis() > 5000UL) {  
            code.cancel();              // Cancel flashing code after 5s
            cancelled = true;
        }
    }

    BlinkCode::service();               // Asynchronous service routine, should be called periodically
}
