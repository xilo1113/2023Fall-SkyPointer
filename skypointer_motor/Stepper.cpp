#include "Stepper.h"
#include "mbed.h"

stepper::stepper(PinName _en, PinName ms1, PinName ms2, PinName ms3, PinName _stepPin, PinName dir):en(_en),
    microstepping(ms1, ms2, ms3),
    stepPin(_stepPin),
    direction(dir)
{
}

void stepper::step(int microstep, int dir, float speed)
{
    if (microstep == 1) {
        microstepping = 0;
    } else if (microstep <= 4) {
        microstepping = microstep / 2;
    } else if (microstep > 4) {
        microstepping = (microstep / 2) - 1;
    }
    if (dir == 1) {
        direction = 0;
    } else if (dir == 0) {
        direction = 1;
    }
    
    //  Step...
    stepPin = 1;
    wait_us(1000000/speed);
    stepPin = 0;
    wait_us(1000000/speed);
}

void stepper::enable()
{
    en = 0;
}

void stepper::disable()
{
    en = 1;
}