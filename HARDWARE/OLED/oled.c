#include "oled.h"
#include "stm32f4xx_gpio.h"
#include "oledfont.h"
#include "delay.h"
#include "myiic.h"



void OLED_GPIO_Init(void)
{
	GPIO_Init_Pins(GPIOD,GPIO_Pin_0,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_1,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_3,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_2,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_4,GPIO_Mode_OUT);
}

void XINGC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
}


/**
  * @brief  向SSD1106写入一个字节  主要用于寄存器的配置
  *
  * @param  dat:要写入的数据/命令
  * @param  cmd:数据/命令标志 0,表示命令;1,表示数据;
  *     @arg 
  * @retval None 
  */
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   IIC_Send_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   IIC_Send_Byte(0x78);            //Slave address,SA0=0
	if(IIC_Wait_Ack())
	{
		while(1);
	}
   IIC_Send_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   IIC_Send_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)  
{	
	uint8_t i;			 
	
	if(cmd)
	  Write_IIC_Data(dat);
	else
		 Write_IIC_Command(dat);
} 

/**
  * @brief
  *
  * @param
  *     @arg 
  * @retval None
  */
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xb0+(y & 0x07),OLED_CMD);/* set page start address */
	OLED_WR_Byte(x & 0x0f,OLED_CMD); /* set page start address */
	OLED_WR_Byte(((x & 0xf0)>>4)|0x10,OLED_CMD); /* set higher nibble of the column address */
}   	  
/**
  * @brief  开启OLED显示    
  *
  * @param
  *
  * @retval None
  */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
  * @brief  关闭OLED显示    
  *
  * @param
  *
  * @retval None
  */    
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
	

}		   		

/**
  * @brief 清屏函数,清完屏,整个屏幕是黑色的,和没点亮一样
  *
  * @param  None
  * 
  * @retval None
  */
void OLED_Clear(void)  
{  
	uint8_t page,x;	    
	for(page=0; page<PAGE; page++)  
	{  
		OLED_WR_Byte (0xb0 + page,OLED_CMD);    				//设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      							//设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      							//设置显示位置—列高地址   
		
		for(x=0; x<X_WIDTH; x++){
			OLED_WR_Byte(0,OLED_DATA); 
		}
	}

}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);				
		}
}

//m^n函数
u32 OLED_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
	
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowUnNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
}

/*
函数功能：显示无符号num数字
参数：xy坐标，num显示数字，len是数字长度，size数字大小模式
返回值：无
*/
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size)
{
	if(num>=0)
	{
		OLED_ShowChar(x,y,' ',size);
		OLED_ShowUnNum(x+(size/2),y,num,len,size);
		return;
	}
	
	OLED_ShowChar(x,y,'-',size);
	OLED_ShowUnNum(x+(size/2),y,-num,len,size);
}

/*
函数功能：显示无符号float数字
参数：xy坐标，num显示数字，precise显示数字的精度，size数字大小模式
返回值：无
*/
void OLED_ShowUnFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size)
{
	u8 i = 0;

	u32 integer;
	double decimal;
	
	integer = (int)num;//整数部分
	decimal = (double)(num - integer);//小数部分
	
	OLED_ShowNum(x,y,integer,precisenum,size);//显示整数部分
	OLED_ShowChar(x+(size/2)*(precisenum+1),y,'.',size);//显示小数点
	
	x = x+(size/2)*(precisenum+2);
	while (precisefloat)//显示几位小数
	{
		decimal = decimal * 10;
		integer = (int)decimal;//取出一位小数
		decimal = (double)(decimal - integer);
		OLED_ShowChar(x+(size/2)*i, y, integer + '0', size);//循环显示每位数字字符，从高位显示
		i++;
		precisefloat--;
	}
}

/*显示float型数据,含负数*/
void OLED_ShowFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size)
{
	if (num < 0)
	{
		OLED_ShowChar(x, y, '-', size);
		num = -num;
		OLED_ShowUnFloat(x+(size/2),y,num,precisenum,precisefloat,size);
		return;
	}
	OLED_ShowChar(x, y, ' ', size);
	OLED_ShowUnFloat(x+(size/2),y,num,precisenum,precisefloat,size);
}

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
			if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
     }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
    }					
}


/**
  * @brief  初始化SSD1306			
  *
  * @param
  *     @arg 
  * @retval None
  */	    
void OLED_Init(void)
{ 	

	delay_ms(500);				  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

	OLED_Clear();
	OLED_Set_Pos(0,0); 	

}  































void GPIO_Init_Pins(GPIO_TypeDef * GPIOx,
										uint16_t GPIO_Pin,
										GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	  /* Enable GPIOx, clock */  
  switch((uint32_t)GPIOx)
  {
		case GPIOA_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			break;
    }
		case GPIOB_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			break;
    }
		case GPIOC_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			break;
    }
		case GPIOD_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			break;
    }
		case GPIOE_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
			break;
    }
		case GPIOF_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
			break;
    }
		case GPIOG_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
			break;
    }
		case GPIOH_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
			break;
    }
		case GPIOI_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
			break;
    }	
		default: break;
  }
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode;

	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
	GPIO_Init(GPIOx,&GPIO_InitStructure);	
	GPIO_SetBits(GPIOx,GPIO_Pin);
}









