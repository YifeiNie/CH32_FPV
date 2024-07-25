#ifndef __MPU6050_H_
#define __MPU6050_H_
#include "stdint.h"
//是否启用DMP
#define DMP

//MPU6050地址
#define MPU6050_ADDR  (0x68<<1)

//MPU6050端口
#define I2C_PORT   GPIOB
#define I2C_SCL_PIN    GPIO_Pin_10
#define I2C_SDA_PIN    GPIO_Pin_11

#define MPU6050_REG_ACCEL_OFFS		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
#define MPU6050_REG_PROD_ID			0X0C	//prod id寄存器,在寄存器手册未提到
#define MPU6050_REG_SELF_TESTX		0X0D	//自检寄存器X
#define MPU6050_REG_SELF_TESTY		0X0E	//自检寄存器Y
#define MPU6050_REG_SELF_TESTZ		0X0F	//自检寄存器Z
#define MPU6050_REG_SELF_TESTA		0X10	//自检寄存器A
#define MPU6050_REG_SMPLRT_DIV		0X19	//采样频率分频器
#define MPU6050_REG_CONFIG			0X1A	//配置寄存器
#define MPU6050_REG_GYRO_CONFIG 	0X1B	//陀螺仪配置寄存器
#define MPU6050_REG_ACCEL_CONFIG	0X1C	//加速度计配置寄存器
#define MPU6050_REG_MOTION_DET		0X1F	//运动检测阀值设置寄存器
#define MPU6050_REG_FIFO_EN			0X23	//FIFO使能寄存器
#define MPU6050_REG_I2CMST_CTRL		0X24	//IIC主机控制寄存器
#define MPU6050_REG_I2CSLV0_ADDR	0X25	//IIC从机0器件地址寄存器
#define MPU6050_REG_I2CSLV0			0X26	//IIC从机0数据地址寄存器
#define MPU6050_REG_I2CSLV0_CTRL	0X27	//IIC从机0控制寄存器
#define MPU6050_REG_I2CSLV1_ADDR	0X28	//IIC从机1器件地址寄存器
#define MPU6050_REG_I2CSLV1			0X29	//IIC从机1数据地址寄存器
#define MPU6050_REG_I2CSLV1_CTRL	0X2A	//IIC从机1控制寄存器
#define MPU6050_REG_I2CSLV2_ADDR	0X2B	//IIC从机2器件地址寄存器
#define MPU6050_REG_I2CSLV2			0X2C	//IIC从机2数据地址寄存器
#define MPU6050_REG_I2CSLV2_CTRL	0X2D	//IIC从机2控制寄存器
#define MPU6050_REG_I2CSLV3_ADDR	0X2E	//IIC从机3器件地址寄存器
#define MPU6050_REG_I2CSLV3		  	0X2F	//IIC从机3数据地址寄存器
#define MPU6050_REG_I2CSLV3_CTRL	0X30	//IIC从机3控制寄存器
#define MPU6050_REG_I2CSLV4_ADDR	0X31	//IIC从机4器件地址寄存器
#define MPU6050_REG_I2CSLV4_	  	0X32	//IIC从机4数据地址寄存器
#define MPU6050_REG_I2CSLV4_DO		0X33	//IIC从机4写数据寄存器
#define MPU6050_REG_I2CSLV4_CTRL	0X34	//IIC从机4控制寄存器
#define MPU6050_REG_I2CSLV4_DI		0X35	//IIC从机4读数据寄存器

#define MPU6050_REG_I2CMST_STA		0X36	//IIC主机状态寄存器
#define MPU6050_REG_INTBP_CFG		0X37	//中断/旁路设置寄存器
#define MPU6050_REG_INT_EN    		0X38	//中断使能寄存器
#define MPU6050_REG_INT_STA		 	0X3A	//中断状态寄存器

//加速度寄存器
#define	MPU6050_REG_ACCEL_XOUT_H	0x3B
#define	MPU6050_REG_ACCEL_XOUT_L	0x3C
#define	MPU6050_REG_ACCEL_YOUT_H	0x3D
#define	MPU6050_REG_ACCEL_YOUT_L	0x3E
#define	MPU6050_REG_ACCEL_ZOUT_H	0x3F
#define	MPU6050_REG_ACCEL_ZOUT_L	0x40

//温度寄存器
#define	MPU6050_REG_TEMP_OUT_H		0x41
#define	MPU6050_REG_TEMP_OUT_L		0x42

//角速度寄存器
#define	MPU6050_REG_GYRO_XOUT_H		0x43
#define	MPU6050_REG_GYRO_XOUT_L		0x44
#define	MPU6050_REG_GYRO_YOUT_H		0x45
#define	MPU6050_REG_GYRO_YOUT_L		0x46
#define	MPU6050_REG_GYRO_ZOUT_H		0x47
#define	MPU6050_REG_GYRO_ZOUT_L		0x48

//其它寄存器
#define MPU6050_REG_I2CSLV0_DO		0X63	//IIC从机0数据寄存器
#define MPU6050_REG_I2CSLV1_DO		0X64	//IIC从机1数据寄存器
#define MPU6050_REG_I2CSLV2_DO		0X65	//IIC从机2数据寄存器
#define MPU6050_REG_I2CSLV3_DO		0X66	//IIC从机3数据寄存器

#define MPU6050_REG_I2CMST_DELAY	0X67	//IIC主机延时管理寄存器
#define MPU6050_REG_SIGPATH_RST		0X68	//信号通道复位寄存器
#define MPU6050_REG_MDETECT_CTRL	0X69	//运动检测控制寄存器
#define MPU6050_REG_USER_CTRL		0X6A	//用户控制寄存器
#define MPU6050_REG_PWR_MGMT1		0X6B	//电源管理寄存器1
#define MPU6050_REG_PWR_MGMT2		0X6C	//电源管理寄存器2
#define MPU6050_REG_FIFO_CNTH		0X72	//FIFO计数寄存器高八位
#define MPU6050_REG_FIFO_CNTL		0X73	//FIFO计数寄存器低八位
#define MPU6050_REG_FIFO_RW			0X74	//FIFO读写寄存器
#define MPU6050_REG_DEVICE_ID		0X75	//器件ID寄存器

unsigned char MPU6050_Init(void);
unsigned int MPU6050_Get_Data(unsigned char REG_ADDR);
float MPU6050_Get_Temp(void);

void MPU6050_I2C_Mem_Write(unsigned char DEV_ADDR, unsigned char REG_ADDR, unsigned char len, unsigned char *buf);
void MPU6050_I2C_Mem_Read(unsigned char DEV_ADDR, unsigned char REG_ADDR, unsigned char len, unsigned char *buf);
unsigned char MPU6050_MPU_DMP_GetData(void);
typedef struct
{
	float yaw;
	float pitch;
	float roll;
	int16_t av_yaw;
	int16_t av_pitch;
	int16_t av_roll;
}MPU6050_para_t;


#endif

