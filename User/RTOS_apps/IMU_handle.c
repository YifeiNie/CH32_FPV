/****************************IMU_handle.c***************************************
负责维护IMU

读取IMU的数据，滤波之后将其存储在MPU6050_para_filted中

*******************************************************************************/
#include "IMU_handle.h"

FilterBuf_STRUCT gyro_filter[6];    //IMU平均值滤波结构体
MPU6050_para_t MPU6050_para =       //从IMU获取到的原始数据
{
	0,//yaw
	0,//pitch
	0,//row
	0,//av_yaw 角速度
	0,//av_pitch
	0,//av_roll
};
MPU6050_para_t MPU6050_para_filted; //滤波之后的IMU数据
int IMU_IO_STATUS = IMU_IO_IDLE;    //IMU的读写状态，在操作IMU时禁用中断操作

void IMU_task(void *pvParameters);
void load_filter_data();
void calc_IMU_filter();

void IMU_task(void *pvParameters)
{
    while(1)
    {
        IMU_IO_STATUS = IMU_IO_BUSY;    //更新状态
        if(MPU6050_MPU_DMP_GetData() == RESET)
        {
            load_filter_data();
            calc_IMU_filter();
        }
        IMU_IO_STATUS = IMU_IO_IDLE;    //更新状态
        vTaskDelay(IMU_READ_DELAY);
    }
}

/*IMU滤波相关*/
void load_filter_data()
{
    FilterSample(&gyro_filter[0], MPU6050_para.yaw);
    FilterSample(&gyro_filter[1], MPU6050_para.pitch);
    FilterSample(&gyro_filter[2], MPU6050_para.roll);
    FilterSample(&gyro_filter[3], (float)MPU6050_para.av_yaw);
    FilterSample(&gyro_filter[4], (float)MPU6050_para.av_pitch);
    FilterSample(&gyro_filter[5], (float)MPU6050_para.av_roll);
}

void calc_IMU_filter()
{
    /*!Debug 调换了Pitch和Roll*/
    MPU6050_para_filted.yaw = FilterAverage(&gyro_filter[0]);
    MPU6050_para_filted.pitch = FilterAverage(&gyro_filter[2]);
    MPU6050_para_filted.roll = FilterAverage(&gyro_filter[1]);
    MPU6050_para_filted.av_yaw = FilterAverage(&gyro_filter[3]);
    MPU6050_para_filted.av_pitch = FilterAverage(&gyro_filter[4]);
    MPU6050_para_filted.av_roll = FilterAverage(&gyro_filter[5]);
    if(MPU6050_para_filted.av_roll<=15&&MPU6050_para_filted.av_roll>=-15)
    {
        MPU6050_para_filted.av_roll=0;
    }
    if(MPU6050_para_filted.av_pitch<=15&&MPU6050_para_filted.av_pitch>=-15)
    {
        MPU6050_para_filted.av_pitch=0;
    }

}
