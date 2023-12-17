// #include "mbed.h"
// #include "Stepper.h"
// PinName A4988STEP = D10;
// PinName A4988DIR = D0;
// PinName MS1 = D2;
// PinName MS2 = D3;
// PinName MS3 = D4;
// PinName EN = D1;
// int a = 0;
// Stepper base_stepper(EN, MS1, MS2, MS3, A4988STEP, A4988DIR);
// void add_motor(float degree, int direction){
//     float speed = 0;
//     int microstep = 16;
//     float a = degree/ 360 * 3200;
//     float i = 0;
//     base_stepper.enable();
//     while(i<a){
//         i++;
//         speed = 1000.0;
//         base_stepper.step(microstep, direction, speed);
//     }
//     base_stepper.disable();
//     return;

// }
// int main() {
//     Stepper base_stepper(EN, MS1, MS2, MS3, A4988STEP, A4988DIR);
//     float speed = 0;
//     int microstep = 16;
//     while (true) {
//         base_stepper.enable();
//         if (a < 3200) // sweep 200 step in dir 1
//         {
//             a++;
//             speed = 1000.0;
//             base_stepper.step(microstep, 0, speed);
//         } 
//         else {
//             a++;
//             speed = 2500.0;
//             base_stepper.step(microstep, 1, speed);

//             if (a > 6400) // sweep 200 in dir 2
//             {
//                 a = 0;
//                 base_stepper.disable();
//                 wait_us(1000000);
//             }
//         }
//     }
// }