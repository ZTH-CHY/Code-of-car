/*
 * Kalman_filter.h
 *
 *  Created on: 2024年7月8日
 *      Author: aw
 *借鉴自csdn 原文链接：https://blog.csdn.net/m0_56116736/article/details/123328989
 */

#ifndef CODE_KALMAN_FILTER_H_
#define CODE_KALMAN_FILTER_H_
#include "zf_common_headfile.h"

typedef struct
{
    float x_last;
    float P_now;
    float P_last;
    float K;
    float R_cov;
    float Q_cov;
    float x_out;

}kf_struct;

extern kf_struct kfs_distance;
extern kf_struct kfs_angle;

void kf_struct_init(kf_struct kfs);
float kmfilter(kf_struct kfs,float z);


#endif