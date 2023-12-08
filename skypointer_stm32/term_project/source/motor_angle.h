#include<stdio.h>
#include<math.h>
#include<iostream>
using namespace std;
#define pi 3.1415926
float* rotation(float Yaw/*g*/, float Pitch/*b*/, float roll /*a*/, float rotate_ver, float rotate_hor)
{
    float* rotate;
    rotate = new float[2];

    float new_ver, new_hor;
    float z = sin(rotate_hor*pi/180);
    float x = sqrt((1-z*z))*cos(rotate_ver*pi/180);
    float y = sqrt((1-z*z))*sin(rotate_ver*pi/180);
    
    //printf("x y z = %f %f %f\n",x,y,z);

    float new_x = x*cos(Yaw)*cos(Pitch) + y*sin(Yaw)*cos(Pitch) - z*sin(Pitch);
    float new_y = x*(-sin(Yaw)*cos(roll)+cos(Yaw)*sin(Pitch)*sin(roll)) + y*( cos(Yaw)*cos(roll) + sin(Yaw)*sin(Pitch)*sin(roll)) + z*cos(Pitch)*sin(roll);
    float new_z = x*(sin(Yaw)*sin(roll) +cos(Yaw)*sin(Pitch)*cos(roll)) + y*(-cos(Yaw)*sin(roll) + sin(Yaw)*sin(Pitch)*cos(roll)) + z*cos(Pitch)*cos(roll);

    //printf("x y z = %f %f %f\n",new_x,new_y,new_z);
    float new_rotate_ver = acos(new_x/sqrt(new_x*new_x+new_y*new_y))*180/pi;
    float new_rotate_hor = asin(new_z)*180/pi;
    if(new_y<0.001)
    {
        new_rotate_ver =360-new_rotate_ver;
    }
    if(new_rotate_hor > 90)
    {
        new_rotate_hor = 180 - new_rotate_hor;
        new_rotate_ver -= (new_rotate_ver<180)? -180:180;
    }
    rotate[0] = new_rotate_ver;
    rotate[1] = new_rotate_hor;
    
    return rotate;
}
// int main()
// {
//     float *a;
//     a = rotation(pi/2,0,0,45,45);
//     printf("%f %f", a[0],a[1]);
// }