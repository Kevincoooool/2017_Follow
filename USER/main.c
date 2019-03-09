#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "key.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "control.h"
#include "Data_deal.h"
#include "ANO_DT.h"
#include "oled.h"
#include "myiic.h"

// TIM3 PA6  PA7  PB0  PB1
// TIM4 PB6  PB7  PB8  PB9
// TIM5 PA0  PA1  PA2  PA3
u8 Fly_Mode=0;
u8 Star_Fly_Flag=0,End_Fly_Flag=0;
u8 PIDStart_flag = 0, S_flag = 0, SStart_flag = 0, PID_flag = 0, mang_flag = 0, next_flag = 0;
int i;

void UART_Show(void);
void OLED_Show(void);

int main(void)
{ 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	uart2_init(9600);
	uart3_init(115200);
	LED_Init();
	KEY_Init();
	IIC_Init();       //IIC初始化
	OLED_Init();
	OLED_Clear();
	TIM3_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz.    
	TIM4_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz. 	
	TIM2_Int_Init(10-1,8400-1);
	
	while(1)
	{
		Fly_Mode = 0;
		OLED_Clear();
		while(!Fly_Mode)
		{
			Fly_Mode=KEY_Scan();
			OLED_ShowNum(0, 2, Fly_Mode, 1, 16);
			OLED_ShowUnNum(0, 0, hight, 3, 16);
		}
		OLED_ShowNum(0, 2, Fly_Mode, 1, 16);
		LED = 0;
		delay_ms(200);
		LED = 1;
		switch (Fly_Mode)
		{
		case 1:
			Take_off();
			Start_Fixed_high();
			while(1)
			{					
				if(PID_flag == 1)
				{
					PID_flag = 0;
					PositionPID();
					High_fix();
				}	
				SStart_flag = 1;
				if(S_flag == 1)
				{
					S_flag = 0;
					i++;
				}
				if(i == 300)
				{
					i = 0;
					SStart_flag = 0;
					Land_down();
					break;
				}
			}
			break;
		case 2:
			Take_off();
			Start_Fixed_high();
			while(1)
			{					
				if(PID_flag == 1)
				{
					PID_flag = 0;
					PositionPID();
					High_fix();
				}	
				SStart_flag = 1;
				
				//1s开始计时
				if(S_flag == 1)
				{
					S_flag = 0;
					i++;
				}
				//计时盲飞
				if(i == 100)
				{
					mang_flag = 1;
				}
				
				//忙飞结束
				if(i == 120)
				{
					mang_flag = 0;	
				}
				
				//计时降落
				if(i == 200)
				{
					i = 0;
					SStart_flag = 0;
					Land_down();
					break;
				}
			}
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			Land_down();
			break;		
		}
	}
}


void UART_Show(void)
{
	printf("%5d  ", (int)hight);
	printf("%2d  ",pixX);
	printf("%2d  ",pixY);
	printf("\n");
}

void OLED_Show(void)
{
	OLED_ShowUnNum(0, 0, hight, 3, 16);
	OLED_ShowNum(40, 0, pixX, 2, 16);
	OLED_ShowNum(80, 0, pixY, 2, 16);
	OLED_ShowNum(50, 2, i, 2, 16);
}

