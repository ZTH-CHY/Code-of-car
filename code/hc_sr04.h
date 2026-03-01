#ifndef _HC_SR04_H_
#define _HC_SR04_H_
#include "zf_common_headfile.h"

extern uint8 hc_sr04_flag;

void hc_sr04_init();
void hc_sr04_send();
uint16 hc_sr04_mesure();
void hc_sr04_deal_with();

#endif
