#include"pid.h"

//PID_IncTypeDef BLDC;
//
/*
函数名称：PID_Inc_Init(PID_IncTypeDef *sptr, float kp, float ki, float kd)
函数功能：初始化PID参数
函数变量：*sptr：pid参数结构体
例子：
*/
void PID_Inc_Init(PID_IncTypeDef *sptr, float kp, float ki, float kd)
{
    sptr->Ek1 = 0; // 上次偏差值初始化
    sptr->Ek2 = 0; // 上上次偏差值初始化
    sptr->Kp = kp; // 比例常数
    sptr->Ki = ki; // 积分常数
    sptr->Kd = kd; // 微分常数
    sptr->OUT = 0;
}

/*
函数名称：Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I)
函数功能：位置式pid
函数变量：*sptr：pid参数结构体
          SetValue：设定值
          ActualValue：实际值
          Max_I：最大误差积分
例子：
*/
float Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I)
{
    float PIDInc;
    PID->Ek = SetValue - ActualValue;
    PID->Ek2 += PID->Ek;

    if (PID->Ek2 > Max_I)
        PID->Ek2 = Max_I;
    if (PID->Ek2 < -Max_I)
        PID->Ek2 = -Max_I;

    PIDInc = (PID->Kp * PID->Ek) +
             (PID->Ki * PID->Ek2) +
             (PID->Kd * (PID->Ek - PID->Ek1));
    PID->Ek1 = PID->Ek;
    return PIDInc;
}
/*
函数名称：PID_clear(PID_IncTypeDef *sptr)
函数功能：清空误差
函数变量：*sptr：pid参数结构体
例子：
*/
void PID_clear(PID_IncTypeDef *sptr)
{
    sptr->Ek1 = 0; // 上次偏差值初始化
    sptr->Ek2 = 0; // 上上次偏差值初始化
    //    sptr->OUT = 0;
}
/*
函数名称：set_pid_target(PID_IncTypeDef *pid, float temp_val)
函数功能：设置目标值
函数变量：*sptr：pid参数结构体
函数返回： 无
例子：
*/
void set_pid_target(PID_IncTypeDef *pid, float temp_val)
{
    pid->target_val = temp_val; // 设置当前的目标值
}

/*
函数名称：get_pid_target(PID_IncTypeDef *pid)
函数功能：设置pid参数
函数变量：*sptr：pid参数结构体
函数返回： pid->target_val 当前目标值
例子：
*/
float get_pid_target(PID_IncTypeDef *pid)
{
    return pid->target_val; // 获取当前的目标值
}

/*
函数名称：set_p_i_d(PID_IncTypeDef *pid, float p, float i, float d)
函数功能：设置pid参数
函数变量：*sptr：pid参数结构体
例子：
*/
void set_p_i_d(PID_IncTypeDef *pid, float p, float i, float d)
{
    pid->Kp = p; // 设置比例系数 P
    pid->Ki = i; // 设置积分系数 I
    pid->Kd = d; // 设置微分系数 D
}

void PID_init(){
//   PID_Inc_Init(&Servo_turn,4.5, 0, 0);
//   PID_Inc_Init(&Servo_straight,1.5, 0.4, 0);
//   PID_Inc_Init(&BLDC,0.43,0.05, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PID Servo_turn= {
        4.5,//P
        0,// I 0.4
        0,//D
        1500,//MAX                                                                                                                                                                                                                                                                      MAX
        -1500,//MIN
};
PID Servo_straight= {
        1.5,//P
        0.4,// I 0.4
        0,//D
        1500,//MAX                                                                                                                                                                                                                                                                      MAX
        -1500,//MIN
};
//PID BLDC= {
//        0.38,//P   p=0.03  speed_max=23
//        0.04,// I 0.4
//        0.55,//D
//        3000,//MAX                                                                                                                                                                                                                                                                      MAX
//        0,//MIN
//};
//二车
PID BLDC= {
        0.34,//P  0.34   0.02   0.05  
        0.02,// 
        0.05,//
        3000,//MAX                                                                                                                                                                                                                                                                      MAX
        0,//MIN
};
uint8 bldc_stop_flag=0;
PID BLDC_stop= {
        5,//     刹车用极限pid，采用极限p值快速刹车 
        0.02,// 
        0.05,//
        6000,//MAX                                                                                                                                                                                                                                                                      MAX
        -6000,//MIN
};

float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask)//增量式PID
{
    PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为上两次误差
    PID_Ptr->Ad_value = PID_Ptr->P *(PID_Ptr->error[0] - PID_Ptr->error[1]) +
                      PID_Ptr->I * PID_Ptr->error[0] +
                      PID_Ptr->D * (PID_Ptr->error[0] - 2 * PID_Ptr->error[1] + PID_Ptr->error[2]);//计算PID增量

    PID_Ptr->error[2] = PID_Ptr->error[1];//这两句话顺序对换会出错
    PID_Ptr->error[1] = PID_Ptr->error[0];

    PID_Ptr->PID_Out += PID_Ptr->Ad_value;//计算PID

    if(PID_Ptr->PID_Out >PID_Ptr->MAX) PID_Ptr->PID_Out =PID_Ptr-> MAX;//PID限幅
    if(PID_Ptr->PID_Out <PID_Ptr->MIN) PID_Ptr->PID_Out =PID_Ptr-> MIN;

    return PID_Ptr->PID_Out;
}
//
float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask)//位置式PID
{
    float Positional = 0;
    PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为误差积分

    PID_Ptr->error[2] += PID_Ptr->error[0];
    if(PID_Ptr->error[2] >  50) PID_Ptr->error[2] = 50;//一般这里还会有积分限幅
    if(PID_Ptr->error[2]  <  -50) PID_Ptr->error[2] = -50;

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