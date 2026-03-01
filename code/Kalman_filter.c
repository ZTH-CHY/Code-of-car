#include "zf_common_headfile.h"

kf_struct kfs_distance;
kf_struct kfs_angle;

void kf_struct_init(kf_struct kfs)
{
	kfs.x_last	=0;
	kfs.P_now	=0;
	kfs.P_last	=0;
	kfs.K		=0;
	kfs.Q_cov	=0.005;//过程激励噪声协方差,参数可调
	kfs.R_cov	=0.5;//测量噪声协方差，与仪器测量的性质有关，参数可调
	kfs.x_out	=0;
}

float kmfilter(kf_struct kfs,float z)
{
    kfs.P_now = kfs.P_last + kfs.Q_cov;
    kfs.K = kfs.P_now / (kfs.P_now + kfs.R_cov );
    kfs.x_out = kfs.x_last + kfs.K * (z - kfs.x_last);
    kfs.P_last = (1 - kfs.K)* kfs.P_now;
    kfs.x_last=kfs.x_out;
    
    return kfs.x_out;
}

                          
                        
