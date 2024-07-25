

#include "debug.h"
#include "tim.h"
#include "../General_Files/drivers/MPU6050.h"
#include "../General_Files/drivers/pwm.h"
#include "control_handle.h"
#include "bsp_filter.h"



void TIM3_Interrupt_Init(void)
{
   NVIC_InitTypeDef NVIC_InitStructure={0};
   NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Init( u16 arr, u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

   RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

   TIM_TimeBaseInitStructure.TIM_Period = arr;
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
   TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure);

   TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
   TIM_ARRPreloadConfig( TIM3, ENABLE );
   TIM_Cmd( TIM3, ENABLE );

   TIM3_Interrupt_Init();
}


void TIM3_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
//volatile uint16_t LED_Status = 0;
void TIM3_IRQHandler(void)
{
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);  //清除标志位

    //曾经控制的调度，已清空

}


void TIM4_Interrupt_Init(void)
{
   NVIC_InitTypeDef NVIC_InitStructure={0};
   NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}


void TIM4_Init( u16 arr, u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

   RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );

   TIM_TimeBaseInitStructure.TIM_Period = arr;
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
   TIM_TimeBaseInit( TIM4, &TIM_TimeBaseInitStructure);

   TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
   TIM_ARRPreloadConfig( TIM4, ENABLE );
   TIM_Cmd( TIM4, ENABLE );

   TIM4_Interrupt_Init();
}


void TIM4_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
//volatile uint16_t LED_Status = 0;
void TIM4_IRQHandler(void)
{
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除标志位

    //曾经Print_state的调度，已清空
}





















