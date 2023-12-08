#include "SafeBool.h"
#include "mbed.h"
#include "ThisThread.h"
#include "my_wifi.cpp"
#include <cstdio>
#include "ecompass.h"
#include "motor_angle.h"
Thread wifi_thread;
Thread compass_thread;
volatile float star_alt, star_az;
volatile float board_angle[3];
static events::EventQueue event_queue(32 * EVENTS_EVENT_SIZE);

void handle_wifi(){
    char wifi_buffer[100];
    MySocket *mysocket = new MySocket();
    mysocket->connect_wifi();
    mysocket->connect_socket();
    while(1){

        mysocket->get_data_2(wifi_buffer);
        printf("wifi_buffer:%s\n",wifi_buffer);
        float var1, var2;
        sscanf(wifi_buffer, "%f %f", &var1, &var2);
        // printf("var1: %f\n", var1);
        // printf("var2: %f\n", var2);
        // printf("\n\n");
        star_alt = var1;
        star_az = var2;
    }
}
void calculate_motor(){
    float *motor_angle;
    motor_angle = rotation(board_angle[0],board_angle[1],board_angle[2],star_alt,star_az);    
}
void handle_ecompass(){
    int16_t acco_data[3] = {0};
    int16_t mego_data[3] = {0};
    while(1){
        float total[3] = {0.0};
        for(int i = 0; i < 10; i++){
            BSP_ACCELERO_AccGetXYZ(acco_data);
            BSP_MAGNETO_GetXYZ(mego_data);
            float *temp = calculate_three_angle(acco_data,mego_data);
            total[0] += temp[0];
            total[1] += temp[1];
            total[2] += temp[2];
            wait_us(10 * 1000);
         }
        board_angle[0] = total[0] / 10;
        board_angle[1] = total[1] / 10;
        board_angle[2] = total[2] / 10;
    }
    event_queue.call(calculate_motor);
    
}
int main(){
    BSP_ACCELERO_Init();
    BSP_MAGNETO_Init();
    BSP_GYRO_Init();
    event_queue.event(&calculate_motor);
    wifi_thread.start(handle_wifi);
    compass_thread.start(handle_ecompass);
    event_queue.dispatch_forever();


}
