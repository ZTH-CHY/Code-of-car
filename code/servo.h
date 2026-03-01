#ifndef _servo_h_
#define _servo_h_

#include"zf_common_headfile.h"
#define servo_gps 1
#define servo_sound 2
#define servo_change 3

extern uint16 dir_smotor_max;
extern uint16 dir_smotor_min;
extern uint16 dir_smotor_mid;
extern int16 servo_duty;
extern int16 servo_basis;
extern double target_angle;
extern double turn_flag;
extern double bias_angle;
extern double bias_angle_old;
extern double eskf_angle;
extern uint8 bias_flag;
extern uint16 bias_change_flag;
extern float fwj_receive;
extern double target_angle_first;
extern uint8 sound_gps_flag;

void servo_init(void);
void Servo_Duty(int32 duty,uint8 direct);
void get_basis(float target_angle);
void servo_contral(uint8 servo_patern);

#endif