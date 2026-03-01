#ifndef _uart_zfzs_h_
#define _uart_zfzs_h_
#include "seekfree_assistant_interface.h"
#include"zf_common_headfile.h"

extern uint8 xinbiao_hc[64];
extern uint8 xinbiao_data[6];
extern  uint8 WIRELESS_UART_BUFFER_NOW;
extern  uint8 xinbiao_now;
extern uint8 xinbiao_flag;
extern uint8 receive_num_last;
extern uint8 get_data[4];                                      
extern uint8 get_num;                 
extern uint8 receive_num; 
extern uint8 get_data_last;
//extern uint8 uart_xinbiao_data_get=0;
//extern uint32 fifo_xinbiao_data_count = 0; 
extern fifo_struct                                     wireless_uart_fifo;
extern  uint8                                           wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];

void zfzs_init(seekfree_assistant_transfer_device_enum transfer_device);
void zfzs_transfer();
void xinbiao_uart_get();
void xinbiao_uart_init();
void communication_receive();

#endif
