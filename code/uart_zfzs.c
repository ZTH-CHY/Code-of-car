#include"uart_zfzs.h"
#include"zf_common_headfile.h"
seekfree_assistant_oscilloscope_struct oscilloscope_data;

uint8 xinbiao_hc[64];
uint8 xinbiao_data[6];  
uint8 get_data_last=0;
uint32 fifo_xinbiao_data_count = 0; 
fifo_struct                                     wireless_uart_fifo;
 uint8                                           wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];
 uint8 WIRELESS_UART_BUFFER_NOW =0;
 uint8 xinbiao_now=0;
 uint8 xinbiao_flag=0;

 uint8 get_data[4];                 // 接收数据缓冲数组                               
uint8 get_num = 0;                 
uint8 receive_num;                 // 当前收到的序号
uint8 receive_num_last;            // 上一次收到的序号
void communication_receive(void)
{
    uint8 temp_data;
    
    if(uart_query_byte(WIRELESS_UART_INDEX, &temp_data))
    {
          if(temp_data == 0x66)
          {
              get_num = 0;
          }
          get_data[get_num] = temp_data;
          if(get_data[get_num] == 0x88 && get_num == 3)
          {
              receive_num_last = receive_num;
              receive_num = get_data[1];
             get_num=0;
          }
          if(get_data[1]>15)
          {
          get_data[1]=0;
          }
          get_num ++;
    }
    
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手发送初始化函数
// 参数说明     seekfree_assistant_transfer_device_enum transfer_device          发送的方式
// 返回参数     
// 使用示例     zfzs_init(SEEKFREE_ASSISTANT_DEBUG_UART)；
//---------------------------------------------------------------------------------------------------------------
void zfzs_init(seekfree_assistant_transfer_device_enum transfer_device){
 seekfree_assistant_interface_init (transfer_device);
 //通道数据初始化
 oscilloscope_data.data[0] = 0; 
 oscilloscope_data.data[1] = 0;
 oscilloscope_data.data[2] = 0;
 oscilloscope_data.data[3] = 0;
 oscilloscope_data.data[4] = 0;
 oscilloscope_data.data[5] = 0;

 oscilloscope_data.channel_num = 6;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手发送函数
// 参数说明     
// 返回参数     
// 使用示例     zfzs_transfer();
//---------------------------------------------------------------------------------------------------------------
void zfzs_transfer(){
 oscilloscope_data.data[0] =dian_long_first; 
 oscilloscope_data.data[1] = dian_long;
 oscilloscope_data.data[2] = target_angle_first;
 oscilloscope_data.data[3] = target_angle;
 oscilloscope_data.data[4] = 0;
 oscilloscope_data.data[5] = 0;
 seekfree_assistant_oscilloscope_send(&oscilloscope_data);
}
void xinbiao_uart_get()
{
  fifo_xinbiao_data_count = fifo_used(&wireless_uart_fifo);
  if(fifo_xinbiao_data_count != 0)
    fifo_read_buffer(&wireless_uart_fifo, xinbiao_data, &fifo_xinbiao_data_count, FIFO_READ_AND_CLEAN); 
}


void xinbiao_uart_init()
{
    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // 初始化串口
    uart_rx_interrupt(WIRELESS_UART_INDEX, 1);   //打开串口中断
    uart_tx_interrupt(WIRELESS_UART_INDEX, 0);
    set_wireless_type(WIRELESS_UART, communication_receive);                               // 设置回调函数
}

