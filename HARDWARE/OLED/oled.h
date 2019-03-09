#ifndef __OLED_H_
#define __OLED_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "stdlib.h"	





#define SIZE							16 //   8/16可选，8小1页一组数  16大2页一组数
#if SIZE==8
#define SIZEGAP           14
#else
#define SIZEGAP           18
#endif
#define XLevelL						0x00
#define XLevelH						0x10
#define Max_Column				128
#define Max_Row						64
#define	Brightness				0xCF 
#define X_WIDTH 					128
#define Y_WIDTH 					64	
#define PAGE							8

#define OLED_CMD  				0					//写命令
#define OLED_DATA 				1					//写数据




void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Set_Pos(uint8_t x, uint8_t y) ;
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void) ;
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowUnNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size);
void OLED_ShowUnFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size);
void OLED_ShowFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Init(void);
void XINGC_Init(void);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);

void OLED_GPIO_Init(void);
void GPIO_Init_Pins(GPIO_TypeDef * GPIOx,
										uint16_t GPIO_Pin,
										GPIOMode_TypeDef GPIO_Mode);

void Write_IIC_Data(unsigned char IIC_Data);

#endif

