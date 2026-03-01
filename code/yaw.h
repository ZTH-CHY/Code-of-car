#ifndef _yaw_h_
#define _yaw_h_

#include"zf_common_headfile.h"

extern unsigned char gps_over;
extern unsigned char yawflag;
extern float yaw;
extern float yaw_old;
extern float yaw_increase;
void yaw_init(void);
void yaw_get(void);

#endif