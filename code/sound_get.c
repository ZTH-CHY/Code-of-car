#include "zf_common_headfile.h"
#include "sound_get.h"

#define CHANNEL_NUMBER          (4)           //定义adc初始化通道数

uint8 channel_index = 0;
adc_channel_enum channel_list[CHANNEL_NUMBER] =
{
    ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3
};

int16 mic_raw_data[4][MIC_RAW_DATA_LEN];        // 定义采集需要用到的循环数组
int16 mic_raw_data_count;                       // 定义循环数组计数位
int16 mic_raw_data_count_save;                  // 定义循环数组计数位-保存值(用于取出最近的 FFT_SIZE 个数据)
int16 mic_delay_data_count_save;                //定义时延差数组计数位保存值
int16 fwj_data_count_save;                     //定义方位角数组计数位保存值
int16 mic_delay_data[2][mic_delay_len];              //定义存放延时数据的数组  
float fwj_data[fwj_len];                       //定义存放方位角数据的数组 
float fft_signal[4][2*FFT_SIZE];                // 定义取出数据的保存位置
float delay_signal[2][DELAY_SIZE];
float fwj_signal[FWJ_SIZE];
int16 mic_delay_count=0;                 
int16 fwj_count=0;    
float fwj_real;


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     采集麦克风数据
// 参数说明     void
// 返回参数     void   
// 使用示例     mic_data_get();
// 备注信息     该函数在中断中调用
//-------------------------------------------------------------------------------------------------------------------
void mic_data_get (void)
{  //adc1main();
    // 因为在做FFT计算的时候我们需要减去直流偏量，因此这里固定减2048，实际应该自己通过测量没有声音时的数值来确定这里应该减去多少
    // 当没有声音的时候，减去偏置后得到的数值应该是在0附近，说明减的偏置就是正确的
    mic_raw_data[0][mic_raw_data_count] = adc_mean_filter_convert(ADC_CHANNEL0,5)-1960 ;
    mic_raw_data[1][mic_raw_data_count] = adc_mean_filter_convert(ADC_CHANNEL1,5)-1960 ;
    mic_raw_data[2][mic_raw_data_count] = adc_mean_filter_convert(ADC_CHANNEL2,5)-1960 ;
    mic_raw_data[3][mic_raw_data_count] = adc_mean_filter_convert(ADC_CHANNEL3,5)-1960 ;
    // 采集点数加一
    mic_raw_data_count++;

    if(mic_raw_data_count >= 2500)       // 判断当前采集点数是否已经达到数组末尾
    {
        mic_raw_data_count = 0;          // 采集次数清零  实现循环
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     复制硅麦数据(最近的 FFT_SIZE 个)
// 参数说明     void
// 返回参数     void   
// 使用示例     mic_data_copy();
// 备注信息     取出循环数组最近的2048个数据用于互相关计算
//-------------------------------------------------------------------------------------------------------------------
void mic_data_copy(void)
{
    uint16 mic_raw_data_count_temp = 0;         
    uint16 mic_data_count = 0;
    
    mic_raw_data_count_save = mic_raw_data_count;       // 保存当前时刻的采集位置( mic_raw_data_count 会在中断被持续更新，保存以防止数据异常）
    
    if(mic_raw_data_count_save < FFT_SIZE)              // 若当前采集位置小于需复制的长度(无法一个循环完成复制)，则分为两端分别复制数据
    {
        mic_raw_data_count_temp = MIC_RAW_DATA_LEN - (FFT_SIZE - mic_raw_data_count_save);      
        
        for(int16 i = mic_raw_data_count_temp;i < MIC_RAW_DATA_LEN; i ++)
        {
            fft_signal[0][mic_data_count * 2] = mic_raw_data[0][i];             // 保存用于FFT计算的数据 实部赋值
            fft_signal[1][mic_data_count * 2] = mic_raw_data[1][i];
            fft_signal[2][mic_data_count * 2] = mic_raw_data[2][i];
            fft_signal[3][mic_data_count * 2] = mic_raw_data[3][i];
            
            fft_signal[0][mic_data_count * 2 + 1] = 0;                          // 保存用于FFT计算的数据 虚部赋值
            fft_signal[1][mic_data_count * 2 + 1] = 0;
            fft_signal[2][mic_data_count * 2 + 1] = 0;
            fft_signal[3][mic_data_count * 2 + 1] = 0;
            mic_data_count ++;
        }
        for(int16 i = 0;i < mic_raw_data_count_save; i ++)
        {
            fft_signal[0][mic_data_count * 2] = mic_raw_data[0][i];             // 保存用于FFT计算的数据 实部赋值
            fft_signal[1][mic_data_count * 2] = mic_raw_data[1][i];
            fft_signal[2][mic_data_count * 2] = mic_raw_data[2][i];
            fft_signal[3][mic_data_count * 2] = mic_raw_data[3][i];
            
            fft_signal[0][mic_data_count * 2 + 1] = 0;                          // 保存用于FFT计算的数据 虚部赋值
            fft_signal[1][mic_data_count * 2 + 1] = 0;
            fft_signal[2][mic_data_count * 2 + 1] = 0;
            fft_signal[3][mic_data_count * 2 + 1] = 0;
            mic_data_count ++;
        }
    }
    else                                                                        // 若当前采集位置大于需复制的长度，则直接一个循环完成复制
    {
        for(int16 i = mic_raw_data_count_save-FFT_SIZE;i < mic_raw_data_count_save; i ++)
        {
            fft_signal[0][mic_data_count * 2] = mic_raw_data[0][i];                          // 保存用于FFT计算的数据 实部赋值
            fft_signal[1][mic_data_count * 2] = mic_raw_data[1][i];
            fft_signal[2][mic_data_count * 2] = mic_raw_data[1][i];
            fft_signal[3][mic_data_count * 2] = mic_raw_data[1][i];

            fft_signal[0][mic_data_count * 2 + 1] = 0;                                       // 保存用于FFT计算的数据 虚部赋值
            fft_signal[1][mic_data_count * 2 + 1] = 0; 
            fft_signal[2][mic_data_count * 2 + 1] = 0; 
            fft_signal[3][mic_data_count * 2 + 1] = 0; 
           mic_data_count ++;
        }
    }
}

void sound_adc_init(void)
{
   adc_init(ADC_CHANNEL0, ADC_12BIT);                                          //  初始化对应 ADC 通道为对应精度
    adc_init(ADC_CHANNEL1, ADC_12BIT);                                          //  初始化对应 ADC 通道为对应精度
    adc_init(ADC_CHANNEL2, ADC_12BIT);                                          //  初始化对应 ADC 通道为对应精度
    adc_init(ADC_CHANNEL3, ADC_12BIT);                                           //  初始化对应 ADC 通道为对应精度



}

void mic_delay_data_copy(int x){
    uint16 mic_delay_data_count_temp = 0;         
    uint16 mic_delay_copy_count = 0;
    mic_delay_data_count_save=mic_delay_count;   // 保存当前时刻的采集位置( mic_delay_count 会在中断被持续更新，保存以防止数据异常）
    if(mic_delay_data_count_save < DELAY_SIZE){    // 若当前采集位置小于需复制的长度(无法一个循环完成复制)，则分为两端分别复制数据
      mic_delay_data_count_temp=mic_delay_len-(DELAY_SIZE-mic_delay_data_count_save);
        for(int16 i = mic_delay_data_count_temp;i < mic_delay_len; i ++)
        {
            delay_signal[x][mic_delay_copy_count] = mic_delay_data[x][i];             // 保存用于时延差滤波的值
            mic_delay_copy_count ++;
        }
        for(int16 i = 0;i < mic_delay_data_count_save; i ++)
        {
            delay_signal[x][mic_delay_copy_count] = mic_delay_data[x][i];             // 保存用于时延差滤波的值
            mic_delay_copy_count ++;
        }
    }
    else{
        for(int16 i = mic_delay_data_count_save-DELAY_SIZE;i < mic_delay_data_count_save; i ++)
        {
            delay_signal[x][mic_delay_copy_count] = mic_delay_data[x][i];           //保存用于时延差滤波的值
            mic_delay_copy_count ++;
         }
    }
    
}


//时延差存入数组函数
void mic_delay_data_get(int x,int mic_delay){
   mic_delay_data[x][mic_delay_count]=mic_delay;
   mic_delay_count++;
   if(mic_delay_count>=mic_delay_len){
     mic_delay_count=0;
   }
}

void fwj_data_copy(void){
    uint16 fwj_data_count_temp = 0;         
    uint16 fwj_copy_count = 0;
    fwj_data_count_save=fwj_count;   // 保存当前时刻的采集位置(fwj_count 会在中断被持续更新，保存以防止数据异常）
    if(fwj_data_count_save < FWJ_SIZE){    // 若当前采集位置小于需复制的长度(无法一个循环完成复制)，则分为两端分别复制数据
      fwj_data_count_temp=fwj_len-(FWJ_SIZE-fwj_data_count_save);
        for(int16 i = fwj_data_count_temp;i < fwj_len; i ++)
        {
            fwj_signal[fwj_copy_count] = fwj_data[i];             // 保存用于时延差滤波的值
            fwj_copy_count ++;
        }
        for(int16 i = 0;i < fwj_data_count_save; i ++)
        {
            fwj_signal[fwj_copy_count] = fwj_data[i];             // 保存用于时延差滤波的值
            fwj_copy_count ++;
        }
    }
    else{
        for(int16 i = fwj_data_count_save-FWJ_SIZE;i < fwj_data_count_save; i ++)
        {
            fwj_signal[fwj_copy_count] = fwj_data[i];           //保存用于方位角滤波的值
            fwj_copy_count ++;
         }
    }
    
}




//方位角存入数组函数
void fwj_data_get(float fwj_in){
fwj_data[fwj_count]=fwj_in;
fwj_count++;
if(fwj_count>=fwj_len){
fwj_count=0;
 }
}

void fwj_final(void){   
    float fwj_sum=0;
      for(int i=0;i<39;i++){
      for(int j=0;j<40-i-1;j++){
       if(fwj_signal[j]>fwj_signal[j+1]){
                int temp = fwj_signal[j];
                fwj_signal[j] = fwj_signal[j+1];
                fwj_signal[j+1] = temp;
     }
   }
 }
  for(int i=15;i<25;i++){
   fwj_sum+=fwj_signal[i];
  }
 fwj_real=fwj_sum/10; 
}
