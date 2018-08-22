#include "usart1.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"

#define RX_BUF_LEN (8*1024)    //8K

static u32 nRxCnt = 0;
static u8 aRxBuf[RX_BUF_LEN] __attribute__ ((at(0X20001000)));    


/* usart init */
void USART1_Init(u32 baudrate)
{
//	
	GPIO_InitTypeDef GPIO_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化IO
	//TX PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//RX PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//NVIC 3:3
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	//USART1
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

}

/* send one byte */
void USART1_TX(u8 data)
{
	
	USART_SendData(USART1, data);
	while(SET != USART_GetFlagStatus(USART1, USART_FLAG_TC));
}


/* send string */
void USART1_TXStr(u8 *str)
{
	while( '\0' != *str)
	{
		USART1_TX(*str++);
	}
	
}

/* read receive byte count */
u16 USART1_GetRXCount(void)
{
	return nRxCnt;
}

/* setting eive byte count */
void USART1_SetRXCount(u32 count)
{
	nRxCnt = count;
}

/* getting receive data put to tmp */
void USART1_GetRXData(u8 *tmp, u16 data_len)
{
	u32 i = 0;
	
	for(; i < data_len; i++)
		tmp[i] = aRxBuf[i];
}


/* receive byte fot interrupt */
void USART1_IRQHandler(void)
{
	if(SET == USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
	{
		USART1_TX('B');
		aRxBuf[nRxCnt++] = USART_ReceiveData(USART1);
		if(nRxCnt >= RX_BUF_LEN) nRxCnt = 0;
	}
}
