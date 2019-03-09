#include "key.h"
#include "delay.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE0,1,2,3,4 
} 
//按键处理函数
//返回按键值
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(void)
{	
	u8 mode=0;
	if(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0)
	{
		delay_ms(10);//去抖动 
		if(KEY0==0)mode=1;
		else if(KEY1==0)mode=2;
		else if(KEY2==0)mode=3;
		else if(KEY3==0)mode=4;
		else if(KEY4==0)mode=5;
		else mode=0;
	}
	while(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0); 
	return mode;
}




















