#include "mbed.h"
#include "ThisThread.h"
#include "my_wifi.cpp"
#include <cstdio>
Thread t1;

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
        printf("var1: %f\n", var1);
        printf("var2: %f\n", var2);
        printf("\n\n");
    }
}
int main(){
    handle_wifi();
}
