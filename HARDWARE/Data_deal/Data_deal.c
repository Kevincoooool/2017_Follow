#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "Data_deal.h"
#include "MedianFiler.h"

uint16_t US100_Alt_Temp=0,Alt_Last = 0;
float Alt_CuntTmep1 = 0,Alt_CuntTmep2 = 0,Alt_V_CuntTmep1 = 0,Alt_V_CuntTmep2 = 0,US100_Alt_Last = 0;
float hight,speed;
int pixX,pixY, pixY_last;

extern u8 Fly_Mode;
extern u8 PID_flag, mang_flag, next_flag;

void Vision_datadeal(void)
{
	int tmp, tmp1, tmp2;
	
	USART_SendData(USART2, 0x55);  //向串口2发送数据
	PID_flag = 1;
	if(USART3_RX_BUF[0]==0x55 && USART3_RX_BUF[5]==0xAA)
	{		
		/*接收视觉模块发来的信息*/
		if(USART3_Flage)
		{	
			USART3_Flage=0;
			tmp=((int)USART2_RX_BUF[0]<<8) + USART2_RX_BUF[1];
			hight=tmp/10; //除以 1000 转化为单位 M
			{
				/*  Target_Roll */
				tmp1=((int16_t)USART3_RX_BUF[1]<<8) + USART3_RX_BUF[2];
				tmp1 = Median_filer1(tmp1);
				if(tmp1>=32768)
				{
					tmp1 = tmp1-0xffff;
				}					
				else
				{
					tmp1 =tmp1;
				}
				pixX = tmp1;
				/*  Target_Pitch */
				tmp2=((int16_t)USART3_RX_BUF[3]<<8) + USART3_RX_BUF[4];
				tmp2 = Median_filer2(tmp2);
				if(tmp2>=32768)
				{
					tmp2 = tmp2-0xffff;
				}
				else
				{
					tmp2 = tmp2;
				}
				pixY = tmp2;
				
				if(Fly_Mode == 2 && mang_flag == 1)
				{
					if(pixY_last == 0 && pixY - pixY_last < -50)
					{
						mang_flag = 0;
					}
					else
					{
						pixY = -23;
					}
					TIM_SetCompare1(TIM3,1540); 	//Pitch		CH1 PB4 
					TIM_SetCompare3(TIM3,1530);		//Roll		CH3 PB0 
				}
				pixY_last = pixY;
			}							
		}	
	}
}
