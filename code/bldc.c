#include"bldc.h"
#include"zf_common_headfile.h"

float  TargetDuty;     //设定的占空比
uint32 duty_num=0;


void BLDCcontrol_init(void){
          pwm_init(PWM_IN, 1000, 0);   ///*TCPWM_CH00_P10_4为pwm输出引脚
          gpio_init(DIR_IN, GPO, 0, GPO_PUSH_PULL); //*P08_1为方向引脚
         // encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);
}


void BLDCcontrol(float duty){
    pwm_set_duty(PWM_IN, duty*100); 
    gpio_set_level(DIR_IN, 0);
}