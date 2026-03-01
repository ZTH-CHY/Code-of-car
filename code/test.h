/*
 * menu.h
 *
 *  Created on: 2024Äê1ÔÂ29ÈÕ
 *      Author: aw
 */

#ifndef CODE_TEST_H_
#define CODE_TEST_H_
#include"zf_common_headfile.h"

extern uint8 change_flag;

extern char time_flag,dian_over_flag;
    extern int time_num;
    extern double dian_long;
    extern uint32 dian_now_num;
    extern  double dian_long_first;
  extern  float gnss_latitude_filter;
  extern float gnss_longitude_filter;
  extern float gnss_latitude_last;
  extern float gnss_longitude_last;
    void motor_test();
    void gnss_data_filter();



#endif /* CODE_TEST_H_ */
