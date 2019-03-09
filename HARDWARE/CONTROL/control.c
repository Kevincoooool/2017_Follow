#include "control.h"
#include "delay.h"
#include "usart3.h"
#include "pwm.h"
#include "usart.h"
#include "ANO_DT.h"
#include "oled.h"

extern float hight,speed;
extern int pixX,pixY;

void Vision_datadeal(void);

///*****************************************************************
//设置电机的pwm值
//******************************************************************/
/*黑点悬停控制*/
void PositionPID(void)
{
	static float lastVxErro,lastVyErro;
	static float pidVx_pOut,pidVx_dOut,pidVx_iOut;
	static float pidVy_pOut,pidVy_dOut,pidVy_iOut;
	static float pidVx_value,pidVy_value;
	static unsigned char flag_Y,flag_X;
	/***************X轴PID参数**ROLL************/
	float Vxkp=0.086f;//
	float Vxki=0.0004f;//0.001f;
	float Vxkd=0.027f;//-0.000531;
	/***************Y轴PID参数*PITCH*************/
	float Vykp=0.086f;
	float Vyki=0.0004f;//0.001f;
	float Vykd= 0.024f;
	
	/*X轴位移速度调整*/
	float vxErro=(float)(0.0f-(-pixX*hight/100));
	float vxErroDelta=(vxErro-lastVxErro)/0.016f;
	lastVxErro=vxErro;
	/*X轴积分分离处理*/
	if(vxErro <= 50.0f&&vxErro >= -50.0f)
	{
		flag_X = 0;
	}
	else
	{
		flag_X = 1;
		
	}
	pidVx_pOut=Vxkp * vxErro;
	pidVx_dOut=Vxkd * vxErroDelta;
	pidVx_iOut+=Vxki * vxErro;
	if(pidVx_iOut>2.5f)//1.5
		pidVx_iOut=2.5f;
	if(pidVx_iOut<-2.5f)
		pidVx_iOut=-2.5f;
	
	pidVx_value=pidVx_pOut+pidVx_dOut+flag_X*pidVx_iOut;
	
	if(pidVx_value>10)
		pidVx_value=10;
	if(pidVx_value<-10)
		pidVx_value=-10;
	
	pidVx_value*=22;
	
	
	/***************Y轴PID调节***************/	
	/*X轴位移速度调整*/
	float vyErro=(float)(0.0f-(-pixY*hight/100));
	float vyErroDelta=(vyErro-lastVyErro)/0.016f;
	lastVyErro=vyErro;
	/*Y轴积分分离处理*/
	if(vyErro <= 50.0f&&vyErro >= -50.0f)
	{
		flag_Y = 0;
	}
	else
	{
		flag_Y = 1;
	}
	pidVy_pOut=Vykp * vyErro;
	pidVy_dOut=Vykd * vyErroDelta;
	pidVy_iOut+=Vyki * vyErro;
	/*Y轴积分限幅处理*/
	if(pidVy_iOut>2.5f)
		pidVy_iOut=2.5f;
	if(pidVy_iOut<-2.5f)
		pidVy_iOut=-2.5f;
	
	pidVy_value=pidVy_pOut+pidVy_dOut+flag_Y*pidVy_iOut;
	/*Y轴输出限幅处理*/
	if(pidVy_value>10)
		pidVy_value=10;
	if(pidVy_value<-10)
		pidVy_value=-10;
	
	pidVy_value*=22;
	
	/************PWM赋值***************/	
	TIM_SetCompare1(TIM3,1500+pidVx_value); 	//Pitch		CH1 PB4 
	TIM_SetCompare3(TIM3,1500+pidVy_value);		//Roll		CH3 PB0 			
	TIM_SetCompare4(TIM3,1505);					//Yaw		CH4 PB1
	
	
}

void High_fix(void)//超声波辅助定高  气压计会飘
{
	if (hight>90)
	{
		TIM_SetCompare2(TIM3,1300);
	}
	else if (hight<80)
	{
		TIM_SetCompare2(TIM3,1740);
	}
	else
	{
		TIM_SetCompare2(TIM3,1500);
	}
}

void lock(void)
{
	TIM_SetCompare1(TIM3,1500); 	//Pitch		CH1 PB4 
	TIM_SetCompare2(TIM3,1000);		//Throttle	CH2 PB5
	TIM_SetCompare3(TIM3,1500);		//Roll		CH3 PB0 
	TIM_SetCompare4(TIM3,1000);		//Yaw		CH4 PB1
	TIM_SetCompare1(TIM4,1000);		//			CH5 PD12
	TIM_SetCompare2(TIM4,1000);		//			CH6 PD13
	TIM_SetCompare3(TIM4,1000);		//			CH7 PD14
	TIM_SetCompare4(TIM4,1000);		//			CH8 PD15
	delay_ms(3000);
}

void Unlock(void)
{
	TIM_SetCompare1(TIM3,1500); 	//Pitch		CH1 PA6 
	TIM_SetCompare2(TIM3,1000);		//Throttle	CH2 PA7
	TIM_SetCompare3(TIM3,1500);		//Roll		CH3 PB0 
	TIM_SetCompare4(TIM3,2000);		//Yaw		CH4 PB1
	TIM_SetCompare1(TIM4,1000);		//			CH5 PB6
	TIM_SetCompare2(TIM4,1000);		//			CH6 PB7
	TIM_SetCompare3(TIM4,1000);		//			CH7 PB8
	TIM_SetCompare4(TIM4,1000);		//			CH8 PB9
	delay_ms(4000);
	TIM_SetCompare4(TIM3,1500);		//Yaw		CH4 PB1
	delay_ms(1000);
	TIM_SetCompare2(TIM3,1300);		//Throttle	CH2 PA7		
	delay_ms(500);
	TIM_SetCompare2(TIM3,1000);		//Throttle	CH2 PA7
}

void Take_off(void)
{
	int Throttle=1000,Throttle_Increase=25,Hight_Last;
	OLED_ShowString(0,4,"Start",16);
	delay_ms(3000);
	OLED_ShowString(0,4,"Unlock",16);
	Unlock();
	
	Hight_Last=hight;
	while (hight<25)
	{
		TIM_SetCompare2(TIM3,Throttle);
		Throttle+=Throttle_Increase;
		if(Throttle>=1800)Throttle=1800;
		delay_ms(100);
		if (hight-Hight_Last>1)
		{
			Throttle_Increase=0;
			Throttle-=10;
		}
		if(hight>25)
		{
			PositionPID();		
		}
		Hight_Last=hight;
		TIM_SetCompare1(TIM3,1540); 	//Pitch		CH1 PB4 
		TIM_SetCompare3(TIM3,1530);		//Roll		CH3 PB0 
	}
//	TIM_SetCompare1(TIM3,1500); 	//Pitch		CH1 PB4 
//	TIM_SetCompare3(TIM3,1500);		//Roll		CH3 PB0 
}

void Land_down(void)
{
	OLED_ShowString(0,4,"Land_down",16);
	int j = 0;
	while(j <= 200)
	{
		j++;
		TIM_SetCompare2(TIM3,1280);		//Throttle	CH2 PB5
		delay_ms(20);
		PositionPID();
	}
	
	lock();
}
void Start_Fixed_high(void)
{
	OLED_ShowString(0,4,"Highfix",16);
	TIM_SetCompare1(TIM4,1500);		//			CH5 PD12
	delay_ms(5);
	TIM_SetCompare2(TIM3,1500);		//Throttle	CH2 PB5
	TIM_SetCompare2(TIM3,1300);
	delay_ms(15);
	TIM_SetCompare2(TIM3,1500);
}
