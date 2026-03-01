#ifndef _pid_h_
#define _pid_h_

#include"zf_common_headfile.h"
typedef struct
{
    float P;
    float I;
    float D;
    float MAX;
    float MIN;
    float error[3];
    float Dev[2];
    float Ad_value;
    float PID_Out;
    float SumError;//累计误差
}PID;
typedef struct {
    float target_val;             //目标值，相当于SetValue
    float Kp;                       //比例系数Proportional
    float Ki;                       //积分系数Integral
    float Kd;                       //微分系数Derivative
    float Ek;                       //当前误差
    float Ek1;                      //前一次误差 e(k-1)
    float Ek2;                      //误差积分
    float OUT;                      //PID输出
    float OUT1;
} PID_IncTypeDef;

extern PID Servo_straight;
extern PID Servo_turn;
extern PID BLDC;
extern PID BLDC_stop;
extern uint8 bldc_stop_flag;

void PID_init();
void set_pid_target(PID_IncTypeDef *pid, float temp_val);
float get_pid_target(PID_IncTypeDef *pid);
void set_p_i_d(PID_IncTypeDef *pid, float p, float i, float d);
void PID_clear(PID_IncTypeDef *sptr);
void PID_Inc_Init(PID_IncTypeDef *sptr, float kp, float ki, float kd);
float Positional_PID(PID_IncTypeDef *PID, float SetValue, float ActualValue, float Max_I);
float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask);//不一定要和我这样这么写，只是给你一个参考
float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask);
float PID_Positional_Servo(PID* PID_Ptr, float bias_real, float bias_set);

#endif