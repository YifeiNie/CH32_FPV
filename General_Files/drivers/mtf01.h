#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "bsp_filter.h"

#define MICOLINK_MSG_HEAD            0xEF
#define MICOLINK_MAX_PAYLOAD_LEN     64
#define MICOLINK_MAX_LEN             MICOLINK_MAX_PAYLOAD_LEN + 7


void calc_mtf01_filter();
void mtf01_load_filter_data();
void micolink_decode(uint8_t data);

/*
    消息ID定义
*/
enum
{
    MICOLINK_MSG_ID_RANGE_SENSOR = 0x51,     // 测距传感器
};

/*
    消息结构体定义
*/
typedef struct
{
    uint8_t head;                      
    uint8_t dev_id;                          
    uint8_t sys_id;						
    uint8_t msg_id;                        
    uint8_t seq;                          
    uint8_t len;                               
    uint8_t payload[MICOLINK_MAX_PAYLOAD_LEN]; 
    uint8_t checksum;                          

    uint8_t status;                           
    uint8_t payload_cnt;                       
} MICOLINK_MSG_t;

/*
    数据负载定义
*/
#pragma pack (1)
// 测距传感器
typedef struct
{
    uint32_t  time_ms;			    // 系统时间 ms
    uint32_t  distance;			    // 距离(mm) 最小值为10，0表示数据不可用
    uint8_t   strength;	            // 信号强度
    uint8_t   precision;	        // 精度
    uint8_t   tof_status;	        // 状态
    uint8_t  reserved1;			    // 预留
    int16_t   flow_vel_x;	        // 光流速度x轴
    int16_t   flow_vel_y;	        // 光流速度y轴
    float     Vx;                   // 真实的x轴速度cm/s
    float     Vy;                   // 真实的y轴速度cm/s
    float     Px;                   // 真实的x轴位置cm
    float     Py;                   // 真实的y轴位置cm
    uint8_t   flow_quality;	        // 光流质量
    uint8_t   flow_status;	        // 光流状态
    uint16_t  reserved2;	        // 预留
} MICOLINK_PAYLOAD_RANGE_SENSOR_t;
#pragma pack ()



