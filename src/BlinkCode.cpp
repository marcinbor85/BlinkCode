#include "BlinkCode.h"

namespace blinkcode {

BlinkCode *_blinkCodes = NULL;

BlinkCode::BlinkCode(
        int pin,
        BlinkCode::OutputType outputType,
        BlinkCode::PulseHandler pulseHandler,
        unsigned long pulseGapTime,
        unsigned long pulseShortTime,
        unsigned long pulseLongTime
    ):
        pin(pin),
        outputType(outputType),
        pulseHandler(pulseHandler),
        pulseGapTime(pulseGapTime),
        pulseShortTime(pulseShortTime),
        pulseLongTime(pulseLongTime)
{

}

BlinkCode::BlinkCode(BlinkCode::PulseHandler pulseHandler): BlinkCode(-1, BlinkCode::OutputType::NORMAL_LOW, pulseHandler)
{
    
}

BlinkCode::BlinkCode(int pin, BlinkCode::OutputType outputType): BlinkCode(pin, outputType, NULL)
{
    
}

void BlinkCode::begin()
{
    this->next = _blinkCodes;
    _blinkCodes = this;

    this->state = BlinkCode::State::IDLE;
    this->setOutputState(false);
}

void BlinkCode::end()
{
    BlinkCode *blinkCode = _blinkCodes;
    BlinkCode *prev = NULL;

    while (blinkCode != NULL) {
        if (blinkCode == this) {
            if (prev == NULL) {
                _blinkCodes = blinkCode->next;
            } else {
                prev->next = blinkCode->next;
            }
            break;
        }
        prev = blinkCode;
        blinkCode = blinkCode->next;
    }
}

void BlinkCode::setOutputState(bool state)
{
    bool s;
    if (this->pin >= 0) {
        s = (this->outputType == BlinkCode::OutputType::NORMAL_LOW) ? state : !state;
        setGpioState(this->pin, s);
    } else {
        this->pulseHandler(this, state);
    }
}

void BlinkCode::trigger(unsigned int code, unsigned long delay)
{
    if ((code > 99) || (code < 1))
        return;

    this->lastTick = getTickValue();
    this->delay = delay;
    this->state = BlinkCode::State::DELAY;
    this->code = code;
    this->setOutputState(false);
}

void BlinkCode::cancel()
{
    this->state = BlinkCode::State::IDLE;
    this->setOutputState(false);
}

void BlinkCode::process()
{
    switch (this->state) {
    case BlinkCode::State::IDLE:
        break;
    case BlinkCode::State::DELAY:
        if (getTickValue() - this->lastTick < this->delay)
            break;

        this->state = BlinkCode::State::PREPARE_PULSE;
        break;
    case BlinkCode::State::PREPARE_PULSE:
        if (this->code == 0) {
            this->state = BlinkCode::State::IDLE;
            break;
        }

        if (this->code >= 10) {
            this->state = BlinkCode::State::PULSE_LONG;
            this->code -= 10;
        } else {
            this->state = BlinkCode::State::PULSE_SHORT;
            this->code -= 1;
        }

        this->lastTick = getTickValue();
        this->setOutputState(true);
        break;
    case BlinkCode::State::PULSE_LONG:
        if (getTickValue() - this->lastTick < this->pulseLongTime)
            break;

        this->lastTick = getTickValue();
        this->setOutputState(false);
        this->state = BlinkCode::State::GAP;
        break;
    case BlinkCode::State::PULSE_SHORT:
        if (getTickValue() - this->lastTick < this->pulseShortTime)
            break;

        this->lastTick = getTickValue();
        this->setOutputState(false);
        this->state = BlinkCode::State::GAP;
        break;
    case BlinkCode::State::GAP:
        if (getTickValue() - this->lastTick < this->pulseGapTime)
            break;
        
        this->state = BlinkCode::State::PREPARE_PULSE;
        break;
    }
}

void BlinkCode::service()
{
    BlinkCode *blinkCode = _blinkCodes;

    while (blinkCode != NULL) {
        blinkCode->process();
        blinkCode = blinkCode->next;
    }
}

};
