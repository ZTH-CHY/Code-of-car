
#include "arm_math.h"
#include "zf_common_headfile.h"

#define fft_signal_min  200
#define  fft_signal_max 408

float fwj_old;
float Mic13_Delay_real;
float Mic02_Delay_real;
int Mic13_Delay;                                //时延差
int Mic13_Delay_old=0; 

  float gm_chengji13[FFT_Pointer]; //FFT运算之后两个信号频域的乘积
  float output_gm[FFT_Pointer];
float Mic_ifftOut13[FFT_Pointer]; //最终经IFFT运算(FFT的逆变换)之后得到的时域信号      13

 
float mozhi_max13=0;                  //模值最大值
uint32 fft_count_time_us13 = 0;
uint32 Max_Array_Num13=0;            //互相关运算得到的最大值对应的数组下标                13
float fwj;
uint16 fwj_num=0;
uint8 fwj_big_num=0;
    arm_cfft_instance_f32 gm;   //定义FFT对象硅麦1
    
    uint8 sound_change_flag;
    uint8 change_start_flag=0;
    float data_max_old;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     fft计算得出硅麦1，3的时延下标差
// 参数说明     void
// 返回参数     void   
// 使用示例    zjc();
// 备注信息    
//-------------------------------------------------------------------------------------------------------------------
    uint32 fft_time;
void fft_sound(uint8 model)  //model = 1计算硅麦0.2   model = 2计算硅麦1.3
{
   
  float delay_sum=0;
  
  mozhi_max13=0;  //最大值手动清零确保数据准确
  
if(model ==1)
{

     arm_cfft_f32(&gm, fft_signal[0] , 0 , 1);      //Mic1的FFT正变换  
   arm_cfft_f32(&gm, fft_signal[2] , 0 , 1);    //Mic2的FFT正变换  
 
        
   for(int i=0;i<fft_signal_min;i++)
   {
     fft_signal[0][2*i]=0;
      fft_signal[2][2*i]=0;
   }
   for(int i=fft_signal_max;i<2048;i++)
   {
      fft_signal[0][2*i]=0;
      fft_signal[2][2*i]=0;
   }
   
   
     for(int i = 0; i < Sampling_Num; i++)
 {
   fft_signal[2][2*i + 1]=-fft_signal[2][2*i + 1];                  //对硅麦0取共轭
 }
 
  arm_cmplx_mult_cmplx_f32(fft_signal[0],fft_signal[2],gm_chengji13,2048);
}

 else
 {
    arm_cfft_f32(&gm, fft_signal[1] , 0 , 1);      //Mic1的FFT正变换  
   arm_cfft_f32(&gm, fft_signal[3] , 0 , 1);    //Mic2的FFT正变换  
   
   for(int i=0;i<fft_signal_min;i++)
   {
     fft_signal[1][2*i]=0;
      fft_signal[3][2*i]=0;
     fft_signal[1][2*i+1]=0;
      fft_signal[3][2*i+1]=0;
   }
   for(int i=fft_signal_max;i<2048;i++)
   {
      fft_signal[1][2*i]=0;
      fft_signal[3][2*i]=0;
     fft_signal[1][2*i+1]=0;
      fft_signal[3][2*i+1]=0;
   }
 
     for(int i = 0; i < Sampling_Num; i++)
 {
   fft_signal[3][2*i + 1]=-fft_signal[3][2*i + 1];                  //对硅麦1取共轭
 }
 
  arm_cmplx_mult_cmplx_f32(fft_signal[1],fft_signal[3],gm_chengji13,2048);
 }

arm_cfft_f32(&gm, gm_chengji13 , 1 , 1); //FFT的逆变换




for(int i = 0; i < 2048; i++)           //位置调换
{float t=gm_chengji13[i];
gm_chengji13[i]=gm_chengji13[i+2048];
gm_chengji13[i+2048]=t;
}

arm_cmplx_mag_f32(gm_chengji13,output_gm,2048);

 int num;
 for( num=0;num<2048;num++)
 {
    if(output_gm[num]>mozhi_max13)
    {
        mozhi_max13=output_gm[num]; //找出幅值最大的点,此即为相关性最好的点
        Max_Array_Num13=num; //相关性最好的点对应的数组下标
        
    }
 }
 
 if(change_start_flag>=1)
 {
   if((data_max_old- mozhi_max13)>8000)
     sound_change_flag=1;
 }
 data_max_old=mozhi_max13;
 change_start_flag++;
 
 if(Max_Array_Num13>1013&&Max_Array_Num13<1035)

 Mic13_Delay=1024-Max_Array_Num13; 
mic_delay_data_get(model-1,Mic13_Delay);
 mic_delay_data_copy(model-1);
 for(int i=0;i<DELAY_SIZE;i++){
   for(int j=0;j<DELAY_SIZE-i-1;j++){
     if(delay_signal[model-1][j]>delay_signal[model-1][j+1]){
                int temp = delay_signal[model-1][j];
                delay_signal[model-1][j] = delay_signal[model-1][j+1];
                delay_signal[model-1][j+1] = temp;
     }
   }
 }
 for(int i=1;i<4;i++){
   delay_sum+=delay_signal[model-1][i];
 }
 
 switch(model)
  {
  case 1 : Mic02_Delay_real=delay_sum/3;break;
            
  case 2 : Mic13_Delay_real=delay_sum/3;break;
    
  }

 }





