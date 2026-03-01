#include "pid_bldc.h"

//PID BLDC= {
//        0.02,//P
//        0.0045,// I 0.4
//        0,//D
//        5000,//MAX                                                                                                                                                                                                                                                                      MAX
//        0,//MIN
};
float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask)//增量式PID
{

    if(speed_real<0)speed_real=-speed_real;
    PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为上两次误差
    PID_Ptr->Ad_value = PID_Ptr->P *(PID_Ptr->error[0] - PID_Ptr->error[1]) +
                      PID_Ptr->I * PID_Ptr->error[0] +
                      PID_Ptr->D * (PID_Ptr->error[0] - 2 * PID_Ptr->error[1] + PID_Ptr->error[2]);//计算PID增量

    PID_Ptr->error[2] = PID_Ptr->error[1];//这两句话顺序对换会出错
    PID_Ptr->error[1] = PID_Ptr->error[0];

    PID_Ptr->PID_Out += PID_Ptr->Ad_value;//计算PID

    if(PID_Ptr->PID_Out >PID_Ptr->MAX) PID_Ptr->PID_Out =PID_Ptr-> MAX;//PID限幅
    if(PID_Ptr->PID_Out <PID_Ptr->MIN) PID_Ptr->PID_Out =PID_Ptr-> MIN;
    if(speed_ask>500){
        if(PID_Ptr->PID_Out<360)PID_Ptr->PID_Out=360;
    }
    if(speed_ask<500){
        PID_Ptr->PID_Out=0;
    }
    return PID_Ptr->PID_Out;
}
float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask)//位置式PID
{
    float Positional = 0;
    PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为误差积分

    PID_Ptr->error[2] += PID_Ptr->error[0];
    if(PID_Ptr->error[2] >  220000) PID_Ptr->error[2] = 220000;//一般这里还会有积分限幅
    if(PID_Ptr->error[2]  <  -220000) PID_Ptr->error[2] = -220000;

    if(speed_ask==0){
        PID_Ptr->error[2]=0;
    }

    Positional = PID_Ptr->P * PID_Ptr->error[0] +
                     PID_Ptr->I * PID_Ptr->error[2] +
                     PID_Ptr->D * (PID_Ptr->error[0] - PID_Ptr->error[1]);//计算PID


    PID_Ptr->error[1] = PID_Ptr->error[0];
    if(Positional >PID_Ptr-> MAX) Positional =PID_Ptr-> MAX;//PID限幅
    if(Positional <PID_Ptr-> MIN) Positional =PID_Ptr-> MIN;

    return  Positional;
}

float PID_Positional_Servo(PID* PID_Ptr, float bias_real, float bias_set)//舵机位置式PID
{
    float Positional = 0;
    PID_Ptr->error[0] =1.0*(bias_set - bias_real);//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为误差积分

    Positional = PID_Ptr->P * PID_Ptr->error[0]+
                     PID_Ptr->D * (PID_Ptr->error[0] - PID_Ptr->error[1]);//计算PID

    PID_Ptr->error[1] = PID_Ptr->error[0];
    if(Positional >PID_Ptr-> MAX) Positional =PID_Ptr-> MAX;//PID限幅
    if(Positional <PID_Ptr-> MIN) Positional =PID_Ptr-> MIN;

    return  Positional;
}