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
* 文件名称          cm7_0_isr
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-1-9      pudding            first version
* 2024-5-14     pudding            新增12个pit周期中断 增加部分注释说明
********************************************************************************************************************/

#include "zf_common_headfile.h"


// **************************** PIT中断函数 ****************************
int32 lastspeed;
int32 speed[30];
int32 sum_speed;
float duty_num_second_sound;
float duty_control=0;
uint16 time_num_cnm=0;
float duty_max;
uint8 servo_flag=0;
uint16 servo_time=0;
uint16 yanshi_time=0;

void pit0_ch0_isr()                     // 定时器通道 0 周期中断服务函数   2ms    
{
    pit_isr_flag_clear(PIT_CH0);
    imu_get();
    yaw_get(); 
//      duty_control=0.05;   //调电机参数时使用


    if(!menu_flag&&get_data[1]!=0&&dian_data[get_data[1]+1].state==1)  //判断进入过菜单&&无线串口收到数据&&收到的数据所代表的点是这辆车打过的点
    {     
        if(gnss.speed<0.5)  //启动时，舵机不打角
        { 
            duty_num_second = duty_num;
            duty_control=(duty_num_second)/100;
        }  
        if(gnss.speed>0.5)
        { 
            if(dian_long>change_distance_num)     //设定与目标点的距离大于x时，切换为gps模式 应比主函数中所设置的x小
            {
                bldc_stop_flag=0;
                servo_flag=0;
                time_num_cnm=0;
                if(dian_long<7)
                {
                    duty_control=0.10 ;      //判断与目标点距离小于x，提前减速
                }
                motor_control.motor_set_dir=REVERSE;       //每次进入gps模式时设置电机转动方向为正传
                servo_contral(servo_gps);
                if(fabs(bias_angle)>70)    //判断与目标点角度偏差值大于xx°时，减速以防打滑。     这一段后续如果加入速度系数可以删除
                {
                    duty_control=0.10;
                }
                else   
                {
                duty_control=(duty_num_second)/100;
                }
            }
            else{       //切换为声音信标模式
                servo_flag=1;
//                sound_gps_flag=1;
                servo_time++;
//                if(motor_control.motor_speed_filter>30000*(float)sound_duty_num/100-2500&&time_num_cnm==0)   //0.4s延时，防止舵机在模式切换时猛打  这个值根据循环体内所给出的duty_control*电机最大转速
                if(gnss.speed>gnss_speed_num&&time_num_cnm==0) 
                {   
//                    duty_control=duty_control
                    motor_control.motor_set_dir = FORWARD;
                    sound_gps_flag=0;
                    bldc_stop_flag=1;
                    pwm_set_duty(servo,dir_smotor_mid);          
                    duty_control=0.035;
//                    mos_close();
                }
                else if(servo_time>50)
                {
//                  motor_control.motor_set_dir = REVERSE;
                  if(sound_gps_flag<0)
                  {
                    fwj=0;
                  }
                  sound_gps_flag++;
                    servo_time=0;
                    time_num_cnm=1;    //减速完成标志位，在每次重新进入gps模式时重新赋值
                    //*************************************************************************                               倒车模块
                    if(fabs(fwj)>100)  //判断方位角大于A+°时，倒转
                    {
                        motor_control.motor_set_dir = FORWARD;
                    }
                    else if(fabs(fwj)<40)   //设置一个死区，使得fwj在低于A+时不会立刻跳变为正转，使fwj低于A-°时跳变
                    {
                        motor_control.motor_set_dir = REVERSE;
                    }
                    //****************************************************************************************
                    servo_contral(servo_sound);  
                    duty_num_second_sound=sound_duty_num;
                    duty_control=duty_num_second_sound/100;            
                }
            }
        }
    }
    else
    {
//        if(dian_long>1&&get_data[1]!=0)  //判断需要到达中心点的条件，注释时一般需要与定时中断2中的控制电机不转部分同时注释
//        {
//            servo_contral(servo_gps);
//            duty_num_second = duty_num;
//            duty_control=(duty_num_second)/100;
//        }
//        else
//        {
      yanshi_time++;
      if(yanshi_time>1000)
      {
            yanshi_time=0;
            duty=0;
            pwm_set_duty(servo,dir_smotor_mid);      
        }
    }
	
    
    
}

void pit0_ch1_isr()                     // 定时器通道 1 周期中断服务函数      1ms
{
    pit_isr_flag_clear(PIT_CH1);
    
        menu_keyget();       
        if(gnss_flag)
        {
            gnss_flag = 0;
            gps_over=1;
            gnss_data_parse();
            motor_test();
            dadian_flag=1;
        }      
}

void pit0_ch2_isr()                     // 定时器通道 2 周期中断服务函数      2ms
{
    pit_isr_flag_clear(PIT_CH2);
    
    adc_read();                                 // 读取ADC数据
      
    board_key_scan();                           // 板载按键扫描 控制模式切换
      
    board_led_output(0.001);                    // 板载LED控制 输入当前中断周期用于LED闪烁时间控制
    
    move_filter_calc(&speed_filter, motor_control.motor_speed);                 // 电机转速滑动滤波 使转速信息顺滑
    
    motor_control.motor_speed_filter = speed_filter.data_average;               // 获取滤波后的转速
    
    motor_speed_output();                       // 电机转速输出 用户可通过方向编码器方式读取
       
    if(!menu_flag&&get_data[1]!=0&&dian_data[get_data[1]+1].state==1)
    {
        if(USER_INPUT == motor_control.run_model)   // 判断当前控制模式 
        {
              lastspeed = motor_control.set_speed;
              motor_control.set_speed = motor_control.max_speed * duty_control;    
              if(duty_control!=0)
              {
                  for(uint8 i=0;i<29;i++)
                  {
                      speed[i]=speed[i+1];
                  }
                  speed[29]=motor_control.set_speed;
                  sum_speed=0;
                  for(uint8 i=0;i<30;i++)
                  {
                      sum_speed+=speed[i];
                  }
                  motor_control.set_speed =sum_speed/30;
               }
              if(motor_control.motor_set_dir==REVERSE)    //正转
              {
                  if(servo_flag==0&&bldc_stop_flag==0)
                  {
                  duty=(int16)PID_Incremental(&BLDC, (float)speed_filter.data_average, (float)motor_control.set_speed);
                  }
                  else if(servo_flag==0&&bldc_stop_flag==1)
                  {
                  duty=(int16)PID_Incremental(&BLDC_stop, (float)speed_filter.data_average, (float)motor_control.set_speed);
                  }
                  {
                  duty=duty_control*10000;
                  }
              }
              else{   //反转
                  duty=duty_control*10000;
              }
              motor_control.motor_duty = (uint16)((float)PWM_PRIOD_LOAD * duty/10000);         // 用户输入模式则使用捕获值控制电机占空比
        }
        else   //板载电位器模式，由于使用了判断信标信号，所以需要使用板载电位器模式时应注释掉最大的判断条件
        { 
            motor_control.motor_duty = (uint16)((float)PWM_PRIOD_LOAD * adc_information.current_board/4096);     // 板载电位器模式则使用板载电位器AD值控制电机占空比
        }   
    }
    else if(!menu_flag)//未收到信标信号或收到未打点信号时
    {
//        if(dian_long>1 &&get_data[1]!=0)  //收到未打点信号时
//        {
//            motor_control.motor_set_dir = REVERSE;
//            lastspeed = motor_control.set_speed;
//            motor_control.set_speed = motor_control.max_speed * duty_control;
//            for(uint8 i=0;i<29;i++)
//            {
//                speed[i]=speed[i+1];
//            }
//            speed[29]=motor_control.set_speed;
//            sum_speed=0;
//            for(uint8 i=0;i<30;i++)
//            {
//                sum_speed+=speed[i];
//            }
//            motor_control.set_speed =sum_speed/30;
//            duty=(int16)PID_Incremental(&BLDC, (float)speed_filter.data_average, (float)motor_control.set_speed);
//            motor_control.motor_duty = (uint16)((float)PWM_PRIOD_LOAD * duty/10000);         // 用户输入模式则使用捕获值控制电机占空比
//        }
//        else    //未收到信标信号时
//        {   
    
            motor_control.motor_duty=0;     
        
            
//        }
    }
}

void pit0_ch10_isr()                    // 定时器通道 10 周期中断服务函数      100us
{
    pit_isr_flag_clear(PIT_CH10);
    mic_data_get();	
}

void pit0_ch11_isr()                    // 定时器通道 11 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH11);
    	key_scanner();
}

void pit0_ch12_isr()                    // 定时器通道 12 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH12);
    
}

void pit0_ch13_isr()                    // 定时器通道 13 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH13);
    
}

void pit0_ch14_isr()                    // 定时器通道 14 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH14);
    
}

void pit0_ch15_isr()                    // 定时器通道 15 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH15);
    
}

void pit0_ch16_isr()                    // 定时器通道 16 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH16);
    
}

void pit0_ch17_isr()                    // 定时器通道 17 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH17);
    
}

void pit0_ch18_isr()                    // 定时器通道 18 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH18);
    
}

void pit0_ch19_isr()                    // 定时器通道 19 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH19);
    
}

void pit0_ch20_isr()                    // 定时器通道 20 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH20);
    
}

void pit0_ch21_isr()                    // 定时器通道 21 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH21);
    
}
// **************************** PIT中断函数 ****************************


// **************************** 外部中断函数 ****************************
void gpio_0_exti_isr()                  // 外部 GPIO_0 中断服务函数     
{
    
  
  
}

void gpio_1_exti_isr()                  // 外部 GPIO_1 中断服务函数     
{
    if(exti_flag_get(P01_0))		// 示例P1_0端口外部中断判断
    {

      
      
            
    }
    if(exti_flag_get(P01_1))
    {

            
            
    }
}

void gpio_2_exti_isr()                  // 外部 GPIO_2 中断服务函数     
{
    if(exti_flag_get(P02_0))
    {
            
            
    }
    if(exti_flag_get(P02_4))
    {
            
            
    }

}

void gpio_3_exti_isr()                  // 外部 GPIO_3 中断服务函数     
{



}

void gpio_4_exti_isr()                  // 外部 GPIO_4 中断服务函数     
{



}

void gpio_5_exti_isr()                  // 外部 GPIO_5 中断服务函数     
{



}

void gpio_6_exti_isr()                  // 外部 GPIO_6 中断服务函数     
{
	


}

void gpio_7_exti_isr()                  // 外部 GPIO_7 中断服务函数     
{



}

void gpio_8_exti_isr()                  // 外部 GPIO_8 中断服务函数     
{



}

void gpio_9_exti_isr()                  // 外部 GPIO_9 中断服务函数     
{



}

void gpio_10_exti_isr()                  // 外部 GPIO_10 中断服务函数     
{



}

void gpio_11_exti_isr()                  // 外部 GPIO_11 中断服务函数     
{



}

void gpio_12_exti_isr()                  // 外部 GPIO_12 中断服务函数     
{



}

void gpio_13_exti_isr()                  // 外部 GPIO_13 中断服务函数     
{



}

void gpio_14_exti_isr()                  // 外部 GPIO_14 中断服务函数     
{



}

void gpio_15_exti_isr()                  // 外部 GPIO_15 中断服务函数     
{



}

void gpio_16_exti_isr()                  // 外部 GPIO_16 中断服务函数     
{



}

void gpio_17_exti_isr()                  // 外部 GPIO_17 中断服务函数     
{



}

void gpio_18_exti_isr()                  // 外部 GPIO_18 中断服务函数     
{



}

void gpio_19_exti_isr()                  // 外部 GPIO_19 中断服务函数     
{



}

void gpio_20_exti_isr()                  // 外部 GPIO_20 中断服务函数     
{



}

void gpio_21_exti_isr()                  // 外部 GPIO_21 中断服务函数     
{



}

void gpio_22_exti_isr()                  // 外部 GPIO_22 中断服务函数     
{



}

void gpio_23_exti_isr()                  // 外部 GPIO_23 中断服务函数     
{



}
// **************************** 外部中断函数 ****************************

//// **************************** DMA中断函数 ****************************
//void dma_event_callback(void* callback_arg, cyhal_dma_event_t event)
//{
//    CY_UNUSED_PARAMETER(event);
//	
//
//	
//	
//}
// **************************** DMA中断函数 ****************************

// **************************** 串口中断函数 ****************************
// 串口0默认作为调试串口
void uart0_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口0接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_0), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位
        
#if DEBUG_UART_USE_INTERRUPT                        				                // 如果开启 debug 串口中断
        debug_interrupr_handler();                  				                // 调用 debug 串口接收处理函数 数据会被 debug 环形缓冲区读取
#endif                                              				                // 如果修改了 DEBUG_UART_INDEX 那这段代码需要放到对应的串口中断去
      
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_TX_DONE)            // 串口0发送中断
    {           
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_0), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart1_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口1接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_1), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        wireless_module_uart_handler();
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_TX_DONE)            // 串口1发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_1), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart2_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口2接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_2), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        gnss_uart_callback();
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_TX_DONE)            // 串口2发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_2), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart3_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口3接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_3), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_TX_DONE)            // 串口3发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_3), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart4_isr (void)
{
    
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口4接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_4), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        
        uart_receiver_handler();                                                                // 串口接收机回调函数
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_TX_DONE)            // 串口4发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_4), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}
// **************************** 串口中断函数 ****************************