#ifndef _sound_fft_h_
#define _sound_fft_h_
#include "zf_common_headfile.h"

#define Sampling_Num 2048
#define FFT_Pointer 4096

#define fft_signal_min  200
#define  fft_signal_max 408

 extern float fwj;
extern int Mic13_Delay;                                //时延差
extern float Mic13_Delay_real;
extern float Mic02_Delay_real;
extern uint8 sound_change_flag;
extern float fwj_old;
extern uint16 fwj_num;
extern uint8 fwj_big_num;


extern   arm_cfft_instance_f32 gm;   //定义FFT对象硅麦1


extern uint32 Max_Array_Num13;            //互相关运算得到的最大值对应的数组下标 13

extern float gm_chengji13[FFT_Pointer]; //FFT运算之后两个信号频域的乘积
extern float Mic_ifftOut13[FFT_Pointer]; //最终经IFFT运算(FFT的逆变换)之后得到的时域信号                  13

extern float mozhi_max13;

void fft_sound(uint8 model);

#endif