#ifndef _sound_get_h_
#define _sound_get_h_
#include "zf_common_headfile.h"

#define FFT_SIZE          2048                  // 用于FFT计算的数据长度
#define MIC_RAW_DATA_LEN  2500                  // 用于采集硅麦的循环数组长度(需要大于 FFT_SIZE 防止数据异常)
#define mic_delay_len          10
#define DELAY_SIZE             5
#define fwj_len                50
#define FWJ_SIZE               40
#define CHANNEL_NUMBER          (4)

#define ADC_CHANNEL0            (ADC2_CH01_P18_1)
#define ADC_CHANNEL1            (ADC2_CH02_P18_2)
#define ADC_CHANNEL2            (ADC2_CH00_P18_0)
#define ADC_CHANNEL3            (ADC2_CH03_P18_3)

extern int16 mic_raw_data[4][MIC_RAW_DATA_LEN];        // 定义采集需要用到的循环数组
extern int16 mic_raw_data_count;                       // 定义循环数组计数位
extern int16 mic_delay_count;
extern int16 fwj_count;  
extern int16 mic_delay_data_count_save;                //定义时延差数组计数位保存值
extern int16 fwj_data_count_save;                     //定义方位角数组计数位保存值
extern int16 mic_raw_data_count_save;                  // 定义循环数组计数位-保存值(用于取出最近的 FFT_SIZE 个数据)
extern int16 mic_delay_data[2][mic_delay_len];  
extern float  fwj_data[fwj_len];                       //定义存放方位角数据的数组 
extern float fft_signal[4][2*FFT_SIZE];                // 定义取出数据的保存位置
extern float delay_signal[2][DELAY_SIZE];
extern float fwj_signal[FWJ_SIZE];
extern float fwj_real;
extern uint8 channel_index ;
extern adc_channel_enum channel_list[CHANNEL_NUMBER] ;

void sound_adc_init(void);
void mic_data_get ();
void mic_data_copy();
void mic_delay_data_get(int x,int mic_delay);
void mic_delay_data_copy(int x);
void fwj_data_get(float fwj_in);
void fwj_data_copy();
void fwj_final();
#endif








