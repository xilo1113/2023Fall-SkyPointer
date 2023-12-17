#include "Stepper.h"
using namespace std;
Stepper::Stepper(PinName EN, PinName MS1, PinName MS2, PinName MS3, PinName STEP, PinName DIR)
    :en(EN),
    ms1(MS1),
    ms2(MS2),
    ms3(MS3),
    stepPin(STEP),
    direction(DIR)
{
    disable();
}
void Stepper::step(int microstep, int dir, float speed)
{
    switch(microstep)
    {
        case 1:
            ms1 = 0;
            ms2 = 0;
            ms3 = 0;
            break;
        case 2:
            ms1 = 1;
            ms2 = 0;
            ms3 = 0;
            break;
        case 4:
            ms1 = 0;
            ms2 = 1;
            ms3 = 0;
            break;
        case 8:
            ms1 = 1;
            ms2 = 1;
            ms3 = 0;
            break;
        case 16:
            ms1 = 1;
            ms2 = 1;
            ms3 = 1;
            break;
        default:
            printf("Invalid microstep number.\n");
            return;
    }
    direction = dir;
    stepPin = 1;
    wait_us(1000000/speed);
    stepPin = 0;
    wait_us(1000000/speed);
}
void Stepper::enable()
{
    en = 0;
}
void Stepper::disable()
{
    en = 1;
}