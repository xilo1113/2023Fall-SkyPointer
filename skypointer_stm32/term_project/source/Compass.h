#ifndef COMPASS_H
#define COMPASS_H
#include "MPU9250.h"
#include "mbed.h"
#include "stm32l475xx.h"
#include <chrono>

class Compass
{
    public:
        Compass();
        void magcalMPU9250(float *dest1);
        void setup();
        void measure(float *MPU_angle);
    private:
        MPU9250 mpu9250;
        Timer t;
        float sum;
        uint32_t sumCount;
        uint64_t count;
};
#endif