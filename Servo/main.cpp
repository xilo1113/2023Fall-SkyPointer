#include "PinNameAliases.h"
#include "PwmOut.h"
#include "mbed.h"
#include "mbed_wait_api.h"
PwmOut servo(D3);
int main(){
    servo.period(0.002);
    while(1){
        servo.write(0.1); // servo position determined by a pulsewidth between 1-2ms
        wait_us(1000*1000);
        servo.write(0.25); // servo position determined by a pulsewidth between 1-2ms
        wait_us(1000*1000);
        servo.write(0.5);
        wait_us(1000*1000);
        servo.write(0.75); // servo position determined by a pulsewidth between 1-2ms
        wait_us(1000*1000);
        servo.write(0.95); // servo position determined by a pulsewidth between 1-2ms
        wait_us(1000*1000);
    }
}
