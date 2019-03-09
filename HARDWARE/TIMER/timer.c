#include "timer.h"
#include "led.h"
#include "usart.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
#include "Data_deal.h"
#include "control.h"
#include "sys.h"

TIM_ICInitTypeDef  TIM5_ICInitStructure;

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u32 ms = 0, s = 0, hz = 0;
extern u8 PIDStart_flag, S_flag, SStart_flag;

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
		ms++;
		if(ms == 20)
		{
			PIDStart_flag = 1;
			ms = 0;
			if(SStart_flag == 1)
				s++;
			if(SStart_flag == 0)
				s = 0;
		}
		if(s == 5)
		{
			S_flag = 1;
			s = 0;
		}	
	}
	
}


 
void TIM7_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 64;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM7, ENABLE);
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) 
	{

	}
}
