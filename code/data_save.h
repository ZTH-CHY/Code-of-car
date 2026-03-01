/*
 * data_save.h
 *
 *  Created on: 2024Äê4ÔÂ11ÈÕ
 *      Author: aw
 */

#ifndef CODE_DATA_SAVE_H_
#define CODE_DATA_SAVE_H_
#include "zf_common_headfile.h"
 void  canshu_save(int num);
 void  canshu_read(int num);
 void data_save_init();
 void speed_read();
 void speed_save();
 void speed_sound_save();
 void speed_sound_read();
 void change_distance_save();
 void change_distance_read();
 void kf_num_read_2();
 void kf_num_save_2();
void gnss_speed_num_read();
void gnss_speed_num_save();
 
 
extern float sound_duty_num;
extern float change_distance_num;

#endif 
