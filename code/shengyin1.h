#ifndef _shengyin1_h_
#define _shengyin1_h_
#include "zf_common_headfile.h"

#define Sampling_Num 2048
#define FFT_Pointer 4096
#define fft_signal_max   (408)
#define fft_signal_min  (200)

extern float Mic02_Delay_real;
extern int Mic02_Delay;
extern float output_gm1[FFT_Pointer];

extern float ingm0[FFT_Pointer];                // 定义输入信号 输入信号为复数 所以长度为FFT_Pointer
extern float outgm0[FFT_Pointer];              // 定义输出信号 输出信号为复数 所以长度为FFT_Pointer              硅麦0
extern float ingm0_c[FFT_Pointer];            //取共轭

extern float ingm2[FFT_Pointer];               // 定义输入信号 输入信号为复数 所以长度为FFT_Pointer
extern float outgm2[FFT_Pointer];               // 定义输出信号 输出信号为复数 所以长度为FFT_Pointer             硅麦2
extern float output_gm[FFT_Pointer];
extern   arm_cfft_instance_f32 gm1;   //定义FFT对象硅麦0
//extern   arm_cfft_instance_f32 gm2;   //定义FFT对象硅麦2
//extern   arm_cfft_instance_f32 gm02;   //定义FFT对象硅麦02

extern uint32 Max_Array_Num02;            //互相关运算得到的最大值对应的数组下标 02    

extern float gm_chengji02[FFT_Pointer]; //FFT运算之后两个信号频域的乘积
extern float Mic_ifftOut02[FFT_Pointer]; //最终经IFFT运算(FFT的逆变换)之后得到的时域信号                  02

extern long float mozhi02[FFT_Pointer]; //互相关运算的结果的模，因为结果可能为复数，参与运算要化为实数          02
extern float mozhi_max02;

void zjc2();
#endif