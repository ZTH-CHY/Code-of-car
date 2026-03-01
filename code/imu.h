#ifndef _imu_h_
#define _imu_h_

#include"zf_common_headfile.h"

typedef struct
{
    float x;
    float y;
    float z;
}IMU;

extern IMU gyro;
extern uint8 IMUOK;


#endif