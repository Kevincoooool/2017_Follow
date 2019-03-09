#include "pwm.h"
#include "led.h"
#include "usart.h"


//=============================================================================
//函数名称:TIM2_PWM_Init
//功能概要:初始化定时器2的配置与其通道的pwm端口
//参数名称:
//				arr：自动重装值。
//				psc：时钟预分频数
//函数返回:无
//=============================================================================
void TIM3_PWM_Init(u32 arr,u32 psc)
{	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//使能TIM2时钟，GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟和复用功能时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA0复用为定时器2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA1复用为定时器2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOA2复用为定时器2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOA3复用为定时器2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;           //GPIOA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;           //GPIOA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PA
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器2
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM2
 
}

void TIM4_PWM_Init(u32 arr,u32 psc)
{	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//使能TIM2时钟，GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA时钟和复用功能时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOA0复用为定时器2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOA1复用为定时器2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOA2复用为定时器2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOA3复用为定时器2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);				//初始化PA
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;     //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器2
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 4OC1
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM2
 
}


