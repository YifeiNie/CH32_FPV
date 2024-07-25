#ifndef  PRINT_STATUS
#define  PRINT_STATUS
#include "debug.h"
#include "uart.h"
#include "stdio.h"
#include "../apps/inc/Crsf.h"
#include "MPU6050.h"
#include "control_handle.h"
#include "tim.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IMU_handle.h"
#include "MTF01.h"

#define PRINT_DELAY_TIME 10 //两次输出的间隔

/*发送数据包的结构体*/
typedef struct
{
    int head;    //0xEF
    /*IMU信息*/
    float IMU_yaw;  //IMU角度
    float IMU_pitch;
    float IMU_roll;
    int IMU_av_yaw; //IMU加速度
    int IMU_av_pitch;
    int IMU_av_roll;
    /*光流信息*/
    float MTF01_roll;   //光流
    float MTF01_pitch;
    int MTF01_ToF_status;
    int MTF01_distance; //距离
    float MTF01_Px;     //位置&加速度
    float MTF01_Py;
    float MTF01_Vx;
    float MTF01_Vy;
    /*油门信息*/
    int PWM1;
    int PWM2;
    int PWM3;
    int PWM4;
    int Throttle_isLOCK;
    /*状态信息*/
    int flight_mode;
    int control_mode;
    /*遥控器信息*/
    int CrsfChannels[CRSF_NUM_CHANNELS];

    
    int end;     //0x5A
}__attribute__((packed)) SendPackType;



extern void Print_status_task(void *pvParameters);

#endif
