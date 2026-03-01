#ifndef _BLDC_H_
#define _BLDC_H_
#include "zf_common_headfile.h"

void BLDCcontrol(float duty);
void BLDCcontrol_init(void);

extern uint32 duty_num;

#endif
