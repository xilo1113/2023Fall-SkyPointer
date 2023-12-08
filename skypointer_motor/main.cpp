// #include "PinNameAliases.h"
// #include "PinNames.h"
// #include "mbed.h"
// #include "Stepper.h"
// #include "ws_bootstrap.h"
// PinName my_step(D10);
// PinName dir(D0);
// PinName enable(D1), MS1(D2), MS2(D3), MS3(D4);
// // main() runs in its own thread in the OS
// stepper mystepper(enable,MS1,MS2,MS3,my_step,dir);
// int main()
// {   
//     mystepper.enable();
//     while (true) {
//         mystepper.step(2,1,2.0);
//     }
// }
#include "mbed.h"

DigitalOut A4988STEP(D10);
DigitalOut A4988DIR(D0);
DigitalOut MS1(D2);
int a=0;

int main() {
    A4988DIR = 0;
    A4988STEP = 0;
    MS1 = 1;
    while(1) {
      if (a <  200)  //sweep 200 step in dir 1
       {
        a++;
        A4988STEP = 1;
        wait_us(8000);
        A4988STEP = 0;
        wait_us(8000);
       }
      else 
       {
        A4988DIR = 1;
        a++;
        A4988STEP = 1;
        wait_us(8000);
        A4988STEP = 0;
        wait_us(8000);
        
        if (a>400)    //sweep 200 in dir 2
         {
          a = 0;
        A4988DIR = 0;
         }
        }
    }
}