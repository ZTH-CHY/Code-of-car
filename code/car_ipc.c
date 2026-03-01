#include "zf_common_headfile.h"


void my_ipc_callback_0(uint32 receive_data)
{
  if(receive_data>=1000)
  {
    fwj_receive=receive_data-1000;
    
  }
  else
   fwj_receive=  receive_data;   // 将接收到的数据打印到串口     
}

void my_ipc_callback_1(uint32 receive_data)
{
    sound_change_flag=      receive_data;      // 将接收到的数据打印到串口     
}