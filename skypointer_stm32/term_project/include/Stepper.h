#ifndef MBED_STEPPER_H
#define MBED_STEPPER_H
#include "DigitalOut.h"


#include "mbed.h"

class Stepper
{
    public:
        Stepper(PinName EN=D1, PinName MS1=D2, PinName MS2=D3, PinName MS3=D4, PinName STEP=D10, PinName DIR=D0);
        void step(int microstep, int dir, float speed);
        void enable();
        void disable();
    private:
        DigitalOut en;
        DigitalOut ms1;
        DigitalOut ms2;
        DigitalOut ms3;
        DigitalOut stepPin;
        DigitalOut direction;
};
#endif