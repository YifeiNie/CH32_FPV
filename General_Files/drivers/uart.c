
#include "debug.h"
#include "uart.h"
#include "MTF01.h"
#include "stdio.h"
#include "../apps/inc/Crsf.h"
#include "MPU6050.h"
#include "control_handle.h"
#include "tim.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IMU_handle.h"

//extern u8 is_locked;           // 电机锁
//extern u8 flight_mode;         //飞行模式
//extern u8 is_landing;          //自动降落

void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));      // 串口中断必须配置这个，否则只会进一次中断

void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART2_InitStructure = {0};
    USART_InitTypeDef UART5_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);    // 注意重映射时不仅仅需要使能外设和GPIO的时钟，还要使能AFIO的时钟

    GPIO_PinRemapConfig(GPIO_FullRemap_USART5, ENABLE);   //串口5的完全重映射

    // 串口2
    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 串口5
    /* UART5 TX-->E.8  RX-->E.9 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    USART2_InitStructure.USART_BaudRate = 420000;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_Parity = USART_Parity_No;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART2_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    UART5_InitStructure.USART_BaudRate = 115200;
    UART5_InitStructure.USART_WordLength = USART_WordLength_8b;
    UART5_InitStructure.USART_StopBits = USART_StopBits_1;
    UART5_InitStructure.USART_Parity = USART_Parity_No;
    UART5_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART5_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
    USART_Init(USART2, &USART2_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(UART5, ENABLE);
    USART_Init(UART5, &UART5_InitStructure);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
}


//void USART2_IRQHandler(void)
//{
//
//    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET && USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
//    {
//        RxBuffer[0]=USART_ReceiveData(USART2);
//        RxBuf[RxBuf_Index++] = RxBuffer[0];
//        HandleByteReceived();
//        if (RxBuf_Index == (sizeof(RxBuf)/sizeof(RxBuf[0])))
//        {
//            RxBuf_Index = 0;
//        }
//
//        USART_ClearFlag(USART2, USART_IT_RXNE);
//
////        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
////        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//
//    }
////    else if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
////    {
////        USART_ReceiveData(USART2);
////        isUsart2RxFlag = true;
////    }
//
//}




void USART2_IRQHandler(void)
{

    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET && USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
    {
        USART2_RxCpltCallback(USART2);
    }
}

void USART2_RxCpltCallback(USART_TypeDef *USARTx)
{
    RxBuf[RxBuf_Index++] =USART_ReceiveData(USARTx);

    if (RxBuf_Index == (sizeof(RxBuf)/sizeof(RxBuf[0])))
    {
        RxBuf_Index = 0;
    }
    if(IMU_IO_STATUS == IMU_IO_IDLE) HandleByteReceived();  //仅在IMU不读写时解包
    USART_ClearFlag(USARTx, USART_IT_RXNE);
//    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
}

void UART5_IRQHandler(void)
{

    if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET && USART_GetFlagStatus(UART5, USART_FLAG_RXNE) != RESET)
    {
        UART5_RxCpltCallback(UART5);
    }
}

void UART5_RxCpltCallback(USART_TypeDef *USARTx)
{
    micolink_decode(USART_ReceiveData(USARTx));
    USART_ClearFlag(USARTx, USART_IT_RXNE);
}


