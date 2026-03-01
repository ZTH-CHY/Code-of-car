#ifndef _pose_h_
#define _pose_h_

#include"myheadfile.h"
typedef struct Pose_Flag
{
    unsigned char run;
    unsigned char use_mag;
}Pose_Flag;

typedef struct Pose_Data
{
    float yaw;
    float rol;
    float pit;

    float rotate_matrix[3][3];  //Ðý×ª¾ØÕó

}Pose_Data;
typedef struct Pose_DInterface
{
    float *a_x;
    float *a_y;
    float *a_z;

    float *g_x;
    float *g_y;
    float *g_z;

    float *m_x;
    float *m_y;
    float *m_z;
}Pose_DInterface;

typedef struct Pose_Interface
{
    Pose_DInterface data;
}Pose_Interface;

typedef struct Pose_Module
{
    Pose_Flag flag;
    Pose_Interface interface;
    Pose_Data data;
}Pose_Module;
extern Pose_Module pose;
#endif