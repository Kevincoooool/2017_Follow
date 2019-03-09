#ifndef __USART3_H
#define __USART3_H

#include "sys.h"
	  	
extern u8  USART3_RX_BUF[]; 			//接收缓冲,最大USART_REC_LEN个字节. 
extern u8  USART3_Flage;				//接收完成标志
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
void uart3_init(u32 bound);
#endif	   
