#ifndef _pin_h_
#define _pin_h_
#include"zf_common_headfile.h"
#define servo TCPWM_CH49_P14_1 //舵机
//#define motor TCPWM_CH00_P06_1 //无刷电机
#define EN_PIN     (P05_4) //使能引脚
#define PWM_IN      (TCPWM_CH32_P10_4)
#define DIR_IN      (P08_1)
#define UART_TX_PIN   (UART1_TX_P04_1)
#define UART_RX_PIN   (UART1_RX_P04_0)
#define Beep     (P18_5)
#define echo      (P18_6)
#define trig      (P18_7)
#endif