#include <MTF01.h>

/*
说明： 用户使用micolink_decode作为串口数据处理函数即可
距离有效值最小为10(mm),为0说明此时距离值不可用
光流速度值单位：cm/s
飞控中只需要将光流速度值*高度，即可得到真实水平位移速度
计算公式：实际速度(cm/s)=光流速度*高度(m)
*/
#define sample_time  0.008
bool micolink_parse_char(MICOLINK_MSG_t* msg, uint8_t data);

FilterBuf_STRUCT mtf01_filter[5];                    // 光流计和雷达平均值滤波结构体
MICOLINK_PAYLOAD_RANGE_SENSOR_t payload;            // 光流计和雷达的数据都存在这里
MICOLINK_PAYLOAD_RANGE_SENSOR_t payload_filtered;    // 均值滤波后光流计和雷达的数据都存在这里


void micolink_decode(uint8_t data)
{
    static MICOLINK_MSG_t msg;

    if(micolink_parse_char(&msg, data) == false)
        return;
    
    switch(msg.msg_id)
    {
        case MICOLINK_MSG_ID_RANGE_SENSOR:
        {
            memcpy(&payload, msg.payload, msg.len);

            /*
                此处可获取传感器数据:
            
                距离        = payload.distance;
                强度        = payload.strength;
                精度        = payload.precision;
                距离状态    = payload.tof_status;
                光流速度x轴 = payload.flow_vel_x;
                光流速度y轴 = payload.flow_vel_y;
                光流质量    = payload.flow_quality;
                光流状态    = payload.flow_status;
            */
            break;
        } 

        default:
            break;
        }
    payload.Vx=payload.flow_vel_x*(int16_t)payload.distance/100.0f;
    payload.Vy=payload.flow_vel_y*(int16_t)payload.distance/100.0f;
    payload.Px=payload.Px+sample_time*payload.Vx;
    payload.Py=payload.Py+sample_time*payload.Vy;
    mtf01_load_filter_data();
    calc_mtf01_filter();
}

bool micolink_check_sum(MICOLINK_MSG_t* msg)
{
    uint8_t length = msg->len + 6;
    uint8_t temp[MICOLINK_MAX_LEN];
    uint8_t checksum = 0;

    memcpy(temp, msg, length);

    for(uint8_t i=0; i<length; i++)
    {
        checksum += temp[i];
    }

    if(checksum == msg->checksum)
        return true;
    else
        return false;
}

bool micolink_parse_char(MICOLINK_MSG_t* msg, uint8_t data)
{
    switch(msg->status)
    {
    case 0:     //帧头
        if(data == MICOLINK_MSG_HEAD)
        {
            msg->head = data;
            msg->status++;
        }
        break;
        
    case 1:     // 设备ID
        msg->dev_id = data;
        msg->status++;
        break;
    
    case 2:     // 系统ID
        msg->sys_id = data;
        msg->status++;
        break;
    
    case 3:     // 消息ID
        msg->msg_id = data;
        msg->status++;
        break;
    
    case 4:     // 包序列
        msg->seq = data;
        msg->status++;
        break;
    
    case 5:     // 负载长度
        msg->len = data;
        if(msg->len == 0)
            msg->status += 2;
        else if(msg->len > MICOLINK_MAX_PAYLOAD_LEN)
            msg->status = 0;
        else
            msg->status++;
        break;
        
    case 6:     // 数据负载接收
        msg->payload[msg->payload_cnt++] = data;
        if(msg->payload_cnt == msg->len)
        {
            msg->payload_cnt = 0;
            msg->status++;
        }
        break;
        
    case 7:     // 帧校验
        msg->checksum = data;
        msg->status = 0;
        if(micolink_check_sum(msg))
        {
            return true;
        }
        break;
    default:
        msg->status = 0;
        msg->payload_cnt = 0;
        break;
    }

    return false;
}

void mtf01_load_filter_data()
{
    FilterSample(&mtf01_filter[0], payload.distance);
    FilterSample(&mtf01_filter[1], payload.Vx);
    FilterSample(&mtf01_filter[2], payload.Vy);
    FilterSample(&mtf01_filter[3], payload.Px);
    FilterSample(&mtf01_filter[4], payload.Py);
}

void calc_mtf01_filter()
{
    payload_filtered.distance = FilterAverage(&mtf01_filter[0]);
    payload_filtered.Vx = FilterAverage(&mtf01_filter[1]);
    payload_filtered.Vy = FilterAverage(&mtf01_filter[2]);
    payload_filtered.Px = FilterAverage(&mtf01_filter[3]);
    payload_filtered.Py = FilterAverage(&mtf01_filter[4]);
}




