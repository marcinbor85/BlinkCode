#ifndef BLINK_CODES_H
#define BLINK_CODES_H

#include "BlinkCodeDefs.h"

namespace blinkcode {

class BlinkCode {

public:
    enum class OutputType {
        NORMAL_LOW,
        NORMAL_HIGH
    };

    using PulseHandler = void (*)(BlinkCode *blinkCode, bool state);

    explicit BlinkCode(int pin, BlinkCode::OutputType outputType = BlinkCode::OutputType::NORMAL_LOW);
    explicit BlinkCode(BlinkCode::PulseHandler pulseHandler);

    void begin();
    void end();

    void trigger(unsigned int code, unsigned long delay = DEFAULT_TRIGGER_DELAY);
    void cancel();

    static void service();

private:
    BlinkCode() = delete;
    BlinkCode(const BlinkCode&) = delete;
    BlinkCode& operator=(const BlinkCode&) = delete;

    enum class State {
        IDLE,
        DELAY,
        PREPARE_PULSE,
        PULSE_LONG,
        PULSE_SHORT,
        GAP
    };

    BlinkCode(
        int pin,
        BlinkCode::OutputType outputType,
        BlinkCode::PulseHandler pulseHandler,
        unsigned long pulseGapTime = DEFAULT_PULSE_GAP_TIME,
        unsigned long pulseShortTime = DEFAULT_PULSE_SHORT_TIME,
        unsigned long pulseLongTime = DEFAULT_PULSE_LONG_TIME
    );

    void process();
    void setOutputState(bool state);

    const unsigned long pulseGapTime;
    const unsigned long pulseShortTime;
    const unsigned long pulseLongTime;
    
    const int pin;
    const BlinkCode::OutputType outputType;
    const BlinkCode::PulseHandler pulseHandler;

    unsigned long delay;
    unsigned long lastTick;
    unsigned int code;
    BlinkCode::State state;

    BlinkCode *next;
};

extern BlinkCode *_blinkCodes;

};

#endif /* BLINK_CODES_H */
