#include<stdio.h>
#include<math.h>
#include<iostream>
#include "arm_math.h"
#include "arm_math_types.h"
#include "fast_math_functions.h"

using namespace std;
#define pi 3.1415926
void rotation(float Yaw/*g*/, float Pitch/*b*/, float roll /*a*/, float rotate_ver, float rotate_hor, float* rotate)
{
    
    float32_t new_ver, new_hor;
    float32_t z = arm_sin_f32(rotate_hor*pi/180);
    float32_t z_c;
    arm_sqrt_f32((1-z*z),&z_c);
    float32_t x = z_c*arm_cos_f32(rotate_ver*pi/180);
    float32_t y = z_c*arm_sin_f32(rotate_ver*pi/180);
    
    //printf("x y z = %f %f %f\n",x,y,z);

    float32_t new_x = x*arm_cos_f32(Yaw)*arm_cos_f32(Pitch) + y*arm_sin_f32(Yaw)*arm_cos_f32(Pitch) - z*arm_sin_f32(Pitch);
    float32_t new_y = x*(-arm_sin_f32(Yaw)*arm_cos_f32(roll)+arm_cos_f32(Yaw)*arm_sin_f32(Pitch)*arm_sin_f32(roll)) + y*( arm_cos_f32(Yaw)*arm_cos_f32(roll) + arm_sin_f32(Yaw)*arm_sin_f32(Pitch)*arm_sin_f32(roll)) + z*arm_cos_f32(Pitch)*arm_sin_f32(roll);
    float32_t new_z = x*(arm_sin_f32(Yaw)*arm_sin_f32(roll) +arm_cos_f32(Yaw)*arm_sin_f32(Pitch)*arm_cos_f32(roll)) + y*(-arm_cos_f32(Yaw)*arm_sin_f32(roll) + arm_sin_f32(Yaw)*arm_sin_f32(Pitch)*arm_cos_f32(roll)) + z*arm_cos_f32(Pitch)*arm_cos_f32(roll);

    //printf("x y z = %f %f %f\n",new_x,new_y,new_z);
    float32_t new_rotate_ver = acos(new_x/sqrt(new_x*new_x+new_y*new_y))*180/pi;
    float32_t new_rotate_hor = asin(new_z)*180/pi;
    if(new_y<0.001)
    {
        new_rotate_ver =360-new_rotate_ver;
    }
    if(new_rotate_hor > 90)
    {
        new_rotate_hor = 180 - new_rotate_hor;
        new_rotate_ver -= (new_rotate_ver<180)? -180:180;
    }
    rotate[0] = (float)new_rotate_hor;
    rotate[1] = (float)new_rotate_ver;
    
    return;
}
// int main()
// {
//     float *a;
//     a = rotation(pi/2,0,0,45,45);
//     printf("%f %f", a[0],a[1]);
// }