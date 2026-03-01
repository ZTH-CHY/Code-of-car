#include "arm_math.h"
#include "zf_common_headfile.h"

float Mic02_Delay_real;
int Mic02_Delay;
int Mic02_Delay_old=0;
  float output_gm1[FFT_Pointer];
float ingm0[FFT_Pointer];                // 定义输入信号 输入信号为复数 所以长度为FFT_Pointer
 float outgm0[FFT_Pointer];              // 定义输出信号 输出信号为复数 所以长度为FFT_Pointer              硅麦0
 float ingm0_c[FFT_Pointer];            //取共轭
 
 float ingm2[FFT_Pointer];               // 定义输入信号 输入信号为复数 所以长度为FFT_Pointer
 float outgm2[FFT_Pointer];               // 定义输出信号 输出信号为复数 所以长度为FFT_Pointer             硅麦2

 float gm_chengji02[FFT_Pointer]; //FFT运算之后两个信号频域的乘积
float Mic_ifftOut02[FFT_Pointer]; //最终经IFFT运算(FFT的逆变换)之后得到的时域信号      02

long float mozhi02[FFT_Pointer]; //互相关运算的结果的模，因为结果可能为复数，参与运算要化为实数    02

float mozhi_max02=0.0;                 //模值最大值
uint32 fft_count_time_us02= 0;
uint32 Max_Array_Num02=0;            //互相关运算得到的最大值对应的数组下标                02
  
     arm_cfft_instance_f32 gm1;   //定义FFT对象硅麦0
//      arm_cfft_instance_f32 gm2;   //定义FFT对象硅麦2
//      arm_cfft_instance_f32 gm02;   //定义FFT对象硅麦02

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     fft计算得出硅麦0，2的时延下标差
// 参数说明     void
// 返回参数     void   
// 使用示例    zjc2();
// 备注信息    
//-------------------------------------------------------------------------------------------------------------------
void zjc2()
{
float delay_sum=0;
mozhi_max02=0;
//mic_data_copy(); 
     //    此处编写需要循环执行的代码
//    for(int i = 0; i < Sampling_Num; i++){
//        ingm0[2*i] =fft_signal[0][i* 2];          //  将硅麦0输入填入偶数项（实部）      
//        ingm2[2*i] = fft_signal[2][i* 2];        //  将硅麦2输入填入偶数项 (实部)         
//        ingm0[2*i + 1] =0  ; //硅麦0奇数项（虚部）全部为0
//        ingm2[2*i + 1] =0; //硅麦2奇数项（虚部）全部为0                           
//    }    
  
   arm_cfft_f32(&gm, fft_signal[0] , 0 , 1);      //Mic1的FFT正变换  
   arm_cfft_f32(&gm, fft_signal[2] , 0 , 1);    //Mic2的FFT正变换  
   
   for(int i=0;i<fft_signal_min;i++)
   {
     fft_signal[0][2*i]=0;
      fft_signal[2][2*i]=0;
     fft_signal[0][2*i+1]=0;
      fft_signal[2][2*i+1]=0;
   }
   for(int i=fft_signal_max;i<2048;i++)
   {
      fft_signal[0][2*i]=0;
      fft_signal[2][2*i]=0;
     fft_signal[0][2*i+1]=0;
      fft_signal[2][2*i+1]=0;
   }
 
     for(int i = 0; i < Sampling_Num; i++)
 {
   fft_signal[0][2*i + 1]=-fft_signal[0][2*i + 1];                  //对硅麦1取共轭
 }
    
 
// for(i=0;i<Sampling_Num;i++)
// {
////    gm_chengji13[2*i]=ingm1_c[2*i]*ingm3[2*i]-ingm1_c[2*i+1]*ingm3[2*i+1];
////    gm_chengji13[2*i+1]=ingm1_c[2*i]*ingm3[2*i+1]+ingm1_c[2*i+1]*ingm3[2*i];
////    gm_chengji13[i]=ingm1_c[i]*ingm3[i];
//  
// }                                                                                        //两个信号频域的乘积
 
  arm_cmplx_mult_cmplx_f32(fft_signal[0],fft_signal[2],gm_chengji02,2048);

arm_cfft_f32(&gm, gm_chengji02 , 1 , 1); //FFT的逆变换




for(int i = 0; i < 2048; i++)           //位置调换
{float t=gm_chengji02[i];
gm_chengji02[i]=gm_chengji02[i+2048];
gm_chengji02[i+2048]=t;
}
arm_cmplx_mag_f32(gm_chengji02,output_gm1,2048);

////// 
 int num;
 for( num=0;num<2048;num++)
 {
    if(output_gm1[num]>mozhi_max02)
    {
        mozhi_max02=output_gm1[num]; //找出幅值最大的点,此即为相关性最好的点
        Max_Array_Num02=num; //相关性最好的点对应的数组下标
        //system_delay_ms(1000);
    }
 }
 if(Max_Array_Num02>1013&&Max_Array_Num02<1035){
// //得出音源距离Mic1与Mic3距离的差值
Mic02_Delay=1024-Max_Array_Num02;
mic_delay_data_get(0,Mic02_Delay);
 mic_delay_data_copy(0);
 for(int i=0;i<39;i++){
   for(int j=0;j<40-i-1;j++){
     if(delay_signal[0][j]>delay_signal[0][j+1]){
                int temp = delay_signal[0][j];
                delay_signal[0][j] = delay_signal[0][j+1];
                delay_signal[0][j+1] = temp;
     }
   }
 }
 for(int i=15;i<25;i++){
   delay_sum+=delay_signal[0][i];
 }
 Mic02_Delay_real=delay_sum/10;
 
// Mic02_Delay=Mic02_Delay_old*0.5+(1024-Max_Array_Num02)*0.5; //得到1,3时延的下标差
// Mic02_Delay_old=Mic02_Delay;  
 }
}





//void Mic_Delay_fillter();
