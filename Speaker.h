#include "mbed.h"
class Speaker{
public:
    Speaker(PinName pin) : _pin(pin) {}
    void PlayNote(float frequency, float duration, float volume) {
        _pin.period(1.0/frequency);
        _pin = volume/2.0;
        wait(duration);
        _pin = 0.0;
    }
private:
    PwmOut _pin;
};