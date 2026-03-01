/*
 * dadian.h
 *
 *  Created on: 2024Äê3ÔÂ7ÈÕ
 *      Author: aw
 */

#ifndef CODE_DADIAN_H_
#define CODE_DADIAN_H_
#include "zf_common_headfile.h"

typedef struct
{
    float smoothing_num;
    int num;
}dadian_smoothing_struct;


extern uint32 dian_num;
extern uint32 dian_numfirst;
extern uint32 dian_truely_num;
extern gnss_info_struct dian_data[10];
extern gnss_info_struct dian_datafirst[100];
extern char dadian_flag;
extern dadian_smoothing_struct dadian_smoothing[50];
extern dadian_smoothing_struct dadian_smoothing_two[50];
extern float duty_num_second;

void dian_central_set();
void dian_add();
void dian_minus();
void dian_num_zero();
void dian_jilu();
void da_dian_fankui();
void dadian_smoothing_f();
void dian_shuaxin(int num);

#endif /* CODE_DADIAN_H_ */
