

#include "system.h"
#include "debug.h"
#include "../apps/inc/Crsf.h"
#include "../General_Files/drivers/uart.h"
#include "../General_Files/drivers/MPU6050.h"
#include "../General_Files/drivers/BMP280.h"
#include "../General_Files/drivers/pwm.h"
#include "../General_Files/drivers/adc.h"
#include "../General_Files/drivers/tim.h"
#include "../General_Files/drivers/gpio.h"

extern void Flight_control();
extern void PIDSTRUCT_Init();

void System_Init()
{
    USARTx_CFG();
    while(MPU6050_Init());
    PWM_Init();
    Bmp_Init();
    BAT_ADC_Init();
    PIDSTRUCT_Init();
    LED_Init();
    printf("System_Init OK!\r\n");
}

void System_Loop()
{

//    if(MPU6050_MPU_DMP_GetData() == RESET)
//    {
//        printf("yaw=%f\r\n",MPU6050_para.yaw);
//        printf("pitch=%f\r\n",MPU6050_para.pitch);
//        printf("roll=%f\r\n",MPU6050_para.roll);
//        printf("av_yaw=%d\r\n",MPU6050_para.av_yaw);
//        printf("av_pitch=%d\r\n",MPU6050_para.av_pitch);
//        printf("av_roll=%d\r\n",MPU6050_para.av_roll);
//        printf("temp=%f\r\n\n",MPU6050_Get_Temp());
//        Flight_control();
//        Delay_Ms(50);
//    }
////    while(MPU6050_MPU_DMP_GetData());
////    printf("pressure:%f\r\n",BMP280_Get_Pressure());
////    Delay_Ms(100);
////    printf("ch1:%d\r\n",CrsfChannels[0]);
////    printf("ch2:%d\r\n",CrsfChannels[1]);
////    printf("ch3:%d\r\n",CrsfChannels[2]);
////    printf("ch4:%d\r\n",CrsfChannels[3]);
////    printf("ch5:%d\r\n",CrsfChannels[4]);
////    printf("ch6:%d\r\n",CrsfChannels[5]);
////    printf("ch7:%d\r\n",CrsfChannels[6]);
////    printf("ch8:%d\r\n",CrsfChannels[7]);
////    printf("ch9:%d\r\n",CrsfChannels[8]);
////    printf("ch10:%d\r\n",CrsfChannels[9]);
////    printf("ch11:%d\r\n",CrsfChannels[10]);
////    printf("ch12:%d\r\n\n",CrsfChannels[11]);
////    Delay_Ms(20);


}





