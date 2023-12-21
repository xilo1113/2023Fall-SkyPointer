// #include "SafeBool.h"
// #include "mbed.h"
// #include "ThisThread.h"
// #include "my_wifi.cpp"
// #include <cstdio>
// #include "ecompass.h"
// #include "motor_angle.h"
// #include "Stepper.h"
// PwmOut motor_2(D6);
// PinName A4988STEP = D10;
// PinName A4988DIR = D0;
// PinName MS1 = D2;
// PinName MS2 = D3;
// PinName MS3 = D4;
// PinName EN = D1;
// int a = 0;
// Stepper base_stepper(EN, MS1, MS2, MS3, A4988STEP, A4988DIR);
// InterruptIn button(BUTTON1);
// Semaphore one_slot(1);
// Thread wifi_thread;
// Thread compass_thread;
// float star_alt = 0.0f, star_az = 0.0f;
// float board_angle[3];
// float current_motor_angle = 0.0f;
// static events::EventQueue event_queue(32 * EVENTS_EVENT_SIZE);
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
// void calculate_motor(){
//     float motor_angle[2];
//     one_slot.acquire();
//     rotation(board_angle[0],board_angle[1],board_angle[2],star_az,star_alt,motor_angle);
//     one_slot.release();
//     printf("motor_hor:%f\nmotor_ver%f\n",motor_angle[0],motor_angle[1]);
//     float to_wirite_0 = motor_angle[1] - current_motor_angle;
//     if(to_wirite_0 <0){
//         to_wirite_0 += 360.0f;
//     }
//     one_slot.acquire();
//     printf("old motor_angle %f",current_motor_angle);
//     printf("to write %f\n",to_wirite_0);
//     printf("yaw:%f,pitch:%f,row:%f\n",board_angle[0]/pi*180,board_angle[1]/pi*180,board_angle[2]/pi*180);
//     add_motor(to_wirite_0, 0);
//     current_motor_angle = motor_angle[1];
//     printf("new motor_angle %f\n",current_motor_angle);
//     one_slot.release();
//     motor_2.period_ms(20);
//     motor_2.pulsewidth((10+motor_angle[0]/90.0*10)*0.0001);
//     return;
// }
// void handle_wifi(){
//     char wifi_buffer[100];
//     MySocket *mysocket = new MySocket();
//     mysocket->connect_wifi();
//     mysocket->connect_socket();
//     while(1){

//         mysocket->get_data_2(wifi_buffer);
//         printf("wifi_buffer:%s\n",wifi_buffer);
//         float var1, var2;
//         sscanf(wifi_buffer, "%f %f", &var1, &var2);
//         printf("star_alt: %f\n", var1);
//         printf("star_az: %f\n", var2);
//         // printf("\n\n");
//         // var1 = 30.0f;
//         // var2 = 170.0f;
//         one_slot.acquire();
//         star_alt = var1;
//         star_az = var2;
//         one_slot.release();
//         event_queue.call(calculate_motor);
//         wait_us(1000*1000);
//     }
// }

// void re_callibrate(){
//     wait_us(3000*1000);
//     event_queue.call(calculate_motor);
// }
// void handle_ecompass(){
//     int16_t acco_data[3] = {0};
//     int16_t mego_data[3] = {0};
//     float old_angle[3] = {0};
//     int16_t aqr_times = 1000;
//     int16_t cycle_time = 1000;
//     while(1){
//         float total[3] = {0.0};
//         for(int i = 0; i < aqr_times; i++){
//             BSP_ACCELERO_AccGetXYZ(acco_data);
//             BSP_MAGNETO_GetXYZ(mego_data);
//             float temp[3];
//             calculate_three_angle(acco_data,mego_data,temp);
//             total[0] += temp[0];
//             total[1] += temp[1];
//             total[2] += temp[2];
//             wait_us(cycle_time/aqr_times * 1000);
//         }
//         board_angle[0] = total[0] / aqr_times;
//         board_angle[1] = total[1] / aqr_times;
//         board_angle[2] = total[2] / aqr_times;
//         one_slot.acquire();
//         if(abs(old_angle[0]-board_angle[0])>60 || abs(old_angle[1]-board_angle[1])>60 || abs(old_angle[2]-board_angle[2])>60){
//             old_angle[0] = board_angle[0];
//             old_angle[1] = board_angle[1];
//             old_angle[2] = board_angle[2];
//             printf("big change\n");
//             event_queue.call(calculate_motor);
//         }   
//         one_slot.release();
//     }
    
    
// }
// int main(){
//     BSP_ACCELERO_Init();
//     BSP_MAGNETO_Init();
//     BSP_GYRO_Init();
//     event_queue.event(&calculate_motor);
//     wifi_thread.start(handle_wifi);
//     button.rise(&re_callibrate);
//     compass_thread.start(handle_ecompass);
//     event_queue.dispatch_forever();
//     while(1){};


// }
