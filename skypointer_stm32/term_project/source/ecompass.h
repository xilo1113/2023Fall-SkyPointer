#include "ThisThread.h"
#include "Thread.h"
#include "fast_math_functions_f16.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include "arm_math.h"
#include "mbed.h"
#define M_PI 3.14159265358979323846
#include "cmath"
//rtos::Thread t1;
const float soft_iron[3][3] = {
    {  0.989,  0.043, -0.003 },
    {  0.043,  1.033, -0.000 },
    { -0.003, -0.000,  0.980 }
};
const float hard_iron[3] = { -12.46, 8.24, -17.40};
float* calculate_three_angle(int16_t *acco_data, int16_t *mego_data){
    int hi_cal[3];
    for (int i = 0; i < 3; i++ ) {
        hi_cal[i] = mego_data[i] - hard_iron[i];
    }
    for (int i = 0; i < 3; i++  ) {
        mego_data[i] = (soft_iron[i][0] * hi_cal[0])+
                       (soft_iron[i][1] * hi_cal[1])+ 
                       (soft_iron[i][2] * hi_cal[2]);
    }
    float *angle = new float[3];
    float16_t roll, yaw;
    arm_atan2_f16((float16_t)acco_data[1],(float16_t)acco_data[2],&roll);
    float pitch = asinf(acco_data[0]/sqrt(acco_data[0]*acco_data[0]+acco_data[1]*acco_data[1]+acco_data[2]*acco_data[2]));
    arm_atan2_f16((float16_t)(mego_data[0]*cos(pitch)+mego_data[1]*sin(roll)*sin(pitch)+mego_data[2]*sin(pitch)*cos(roll)),(float16_t)mego_data[2]*sin(roll)-mego_data[1]*cos(roll),&yaw);
    //float yaw = (float)atan2(mego_data[0],mego_data[1]);
    angle[0] = yaw;
    angle[1] = pitch;
    angle[2] = roll;
    return angle;
}
float calculate_Azimuth(int16_t *acco_data, int16_t *mego_data){
    int hi_cal[3];
    for (int i = 0; i < 3; i++ ) {
        hi_cal[i] = mego_data[i] - hard_iron[i];
    }
    for (int i = 0; i < 3; i++  ) {
        mego_data[i] = (soft_iron[i][0] * hi_cal[0])+
                       (soft_iron[i][1] * hi_cal[1])+ 
                       (soft_iron[i][2] * hi_cal[2]);
    }
    float16_t roll, yaw;
    arm_atan2_f16((float16_t)acco_data[1],(float16_t)acco_data[2],&roll);
    float pitch = asinf(acco_data[0]/sqrt(acco_data[0]*acco_data[0]+acco_data[1]*acco_data[1]+acco_data[2]*acco_data[2]));
    arm_atan2_f16((float16_t)(mego_data[0]*cos(pitch)+mego_data[1]*sin(roll)*sin(pitch)+mego_data[2]*sin(pitch)*cos(roll)),(float16_t)mego_data[2]*sin(roll)-mego_data[1]*cos(roll),&yaw);
    //float yaw = (float)atan2(mego_data[0],mego_data[1]);
    float ftemp = - yaw * 180.0f / M_PI;
    ftemp += 90.0f;
    if(ftemp < 0.0f) ftemp += 360.0f;
    printf("x: %d  y:%d",mego_data[0],mego_data[1]);
    printf("roll: %f  ",roll* 180.0f / M_PI);
    printf("pitch: %f  ",pitch* 180.0f / M_PI);
    // printf("roll: %f\n",roll* 180.0f / M_PI);
    // printf("pitch: %f\n",pitch* 180.0f / M_PI);
    // if (ftemp > 0.0f) ftemp -= 360.0f;
    // ftemp = 0.0f -ftemp;
    // ftemp += 90.0f;
    // if (ftemp >= 360.0f) ftemp -= 360.0f;
    return ftemp;
}

void raw_callibrate(){
    int16_t acco_data[3] = {0};
    int16_t mego_data[3] = {0};
    float gyro_data[3] = {0};
    BSP_ACCELERO_AccGetXYZ(acco_data);
    BSP_MAGNETO_GetXYZ(mego_data);
    BSP_GYRO_GetXYZ(gyro_data);
    printf("Raw:%d,%d,%d,%d,%d,%d,%d,%d,%d\n",acco_data[0],acco_data[1],acco_data[2],(int)(gyro_data[0]/1000),(int)(gyro_data[1]/1000),(int)(gyro_data[2]/1000),mego_data[0],mego_data[1],mego_data[2]);

}
float get_Azimuth(){
    // ref:https://blog.csdn.net/cp1300/article/details/105854682
    int16_t acco_data[3] = {0};
    int16_t mego_data[3] = {0};
    BSP_ACCELERO_AccGetXYZ(acco_data);
    BSP_MAGNETO_GetXYZ(mego_data);
    // printf("acco:::0:%8d 1:%8d 2:%8d",acco_data[0],acco_data[1],acco_data[2]);
    // printf("mego:::0:%8d 1:%8d 2:%8d",mego_data[0],mego_data[1],mego_data[2]);
    float azimuth = calculate_Azimuth(acco_data,mego_data);
    printf("Azimuth: %f\n", azimuth);
    return azimuth;
}
void get_avg_Azimuth(){
    float temp = 0.0f;
    while(1){
        for(int i = 0; i < 100; i++){
            temp += get_Azimuth();
            wait_us(10000);
        }
        printf("Azimuth: %f\n",temp/100);
        temp = 0.0f;
    }
    return;
}
// int main()
// {
//     BSP_ACCELERO_Init();
//     BSP_MAGNETO_Init();
//     BSP_GYRO_Init();
//     //t1.start(get_avg_Azimuth);
//     while(1){
        
//         get_Azimuth();
//         rtos::ThisThread::sleep_for(500);
//         //raw_callibrate();
//     }
    
// }