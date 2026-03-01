/*
 * menu.h
 *
 *  Created on: 2024Äê1ÔÂ29ÈÕ
 *      Author: aw
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_
#include "zf_common_headfile.h"

extern key_state_enum key_1,key_2,key_3,key_4,key_5;
extern uint8 menu_flag;
extern float can_data[];
extern uint32 gnss_speed_num;

void menu_keyget();
void menu_main();
void menu_one();
void menu_two();
void menu_three();
void  menu_tiaocan_straight();
void  menu_tiaocan_turn();
void menu_speed();
void menu_kf();
void gnss_speed_num_menu();


#endif /* CODE_MENU_H_ */
