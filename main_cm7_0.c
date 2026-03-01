/*********************************************************************************************************************
* CYT4BB Opensourec Library 即（ CYT4BB 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 CYT4BB 开源库的一部分
*
* CYT4BB 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          main_cm7_0
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-1-4       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************
int check;
double max;
int main(void)
{
    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    debug_info_init();                  // 调试串口信息初始化
    
    // 此处编写用户代码 例如外设初始化代码等

    interrupt_global_disable();                 // 关闭全局中断
           
    xinbiao_uart_init();
    
    ips200_init(IPS200_TYPE_SPI);
    
    PID_init();
    
    gnss_init(1);
    
    menu_flag=1;
    
    key_init(1);
    
    zfzs_init(SEEKFREE_ASSISTANT_DEBUG_UART);
    
    imu_init(); 
    
    yaw_init();

    servo_init();

    data_save_init();

    BLDCcontrol_init(); 
    
//    kf_struct_init( kfs_distance);   //卡尔曼滤波初始化
//     kf_struct_init( kfs_angle);//卡尔曼滤波初始化

    ////////////////////////////////////////////////////////////////
    
    adc_collection_init();                      // ADC采集初始化 
    
    board_gpio_init();                          // 板载GPIO初始化
    
    motor_parameter_init();                     // 电机参数初始化
    
    hall_init();                                // 霍尔采集初始化    
    
    motor_control.motor_set_dir=REVERSE;  //初始化电机转动方向
    /////////////////////////////////////////////////////////////////
    
    sound_adc_init();         
    
    uint32 fft_time=0;
    
    float fwj_sum;
    
    arm_cfft_init_f32(&gm, Sampling_Num);  // 初始化FFT对象 赋予计算长度
   ////////////////////////////////////////////////////////////////////
  
    interrupt_global_enable(0);			// 开启全局中断

    pit_ms_init(PIT_CH0, 3);  //2  
    
    pit_ms_init(PIT_CH1, 1);  //1
    
    pit_ms_init(PIT_CH2, 2);                    // 周期中断初
    
    pit_us_init(PIT_CH10, 100);  // 使用定时器的10通道   创建一个周期频率为10K的周期中断
    pit_ms_init(PIT_CH11, 10); 
     
    // 此处编写用户代码 例如外设初始化代码等
    while(true)
    {
        // 此处编写需要循环执行的代码
      zfzs_transfer();  //逐飞助手发送函数
      if(key_5==KEY_LONG_PRESS)   //长按key5，重新进行屏幕初始化
      {
          ips200_init(IPS200_TYPE_SPI);
      }                         
      if(key_1==KEY_LONG_PRESS)
      {
          menu_flag=1;
          menu_main();
      }
      if(gnss_flag)
      {  
          gnss_flag = 0;
          gps_over=1;
          gnss_data_parse();
          ips200_show_uint(0, 20, dian_long, 10);  ips200_show_uint(100, 20, dian_now_num, 10);
          ips200_show_uint(0, 40, target_angle, 10);
          ips200_show_string(0, 200, "state");   ips200_show_uint(100, 200, gnss.state, 1);      //输出当前定位有效模式 1：定位有效  0：定位无效                   
       }
       ips200_show_int(0, 100, get_data[1], 5);                    
    
/////////////////////////////////////////////////////////////////////////
      if(dian_long<3&&!menu_flag&&get_data[1]!=0&&dian_data[get_data[1]+1].state==1){  //当距离小于x时，开启声音算法，算出fwj 实际需要比中断里设置的x小，使得中断刚刚切换成声音时就有fwj输入
          fwj_old=fwj;
          mic_data_copy();    
          fft_sound(1);  //对硅麦0，2
          fft_sound(2);  //对硅麦1，3
          fwj=atan2(Mic13_Delay_real,Mic02_Delay_real)*180.0 / 3.1415926535898;    
          if(fwj==0&&fwj_num<3)   //防止舵机莫名归中
          {
             fwj_num++;
             fwj=fwj_old;
          }
          else if(fwj!=0)
          {
             fwj_num=0;
          }
          if(fwj==180&&fwj_num<3)   //防止电机莫名正反转
          {
             fwj_big_num++;
             fwj=fwj_old;
          }
          else if(fwj!=180)
          {
             fwj_big_num=0;
          }
          if(fabs(fwj-fwj_old)>320)     //防止舵机突然打大角
          {
             fwj=fwj_old;
          }
          //********************************************************************
//                    if(fabs(fwj-fwj_old)>320)
//          {
//                if(fwj>fwj_old)
//                {
//                      fwj=fwj_old-(180-fwj);
//                }
//                else
//                {
//                      fwj=fwj_old+(fwj+180);
//                }
//                system_delay_ms(1000);
//          }
          //********************************************************************
      }
          ips200_show_float(0, 170, fwj,3,4);
      
      
      if(get_data_last!=get_data[1]&&get_data[1]!=0)//刷新点位数据
      {
        dian_shuaxin(get_data_last+1);
//        sound_gps_flag=0;
      }
      get_data_last=get_data[1];
        // 此处编写需要循环执行的代码

      }
}

// **************************** 代码区域 ****************************
