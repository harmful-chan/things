#include "usart1.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"

#define RX_BUF_LEN (64*1024)    //64K

static u32 nRxCnt = 0;
static u32 aRxBuf[RX_BUF_LEN] = {0};    


/* usart init */
void USART1_Init(u32 baudrate)
{
	
	GPIO_InitTypeDef GPIO_InitStruct; 
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	//使能时钟
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化IO
	//TX PA9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//RX PA10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//NVIC 3:3
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//USART1
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);  
	
}

/* send one byte */
void USART1_TX(u8 data)
{
	while(SET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
	USART_SendData(USART1, data);
	while(SET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
}


/* send string */
void USART1_TXStr(u8 *str)
{
	while( '\0' != *str)
		USART1_TX(*str++);
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
void USART1_IRHandler(void)
{
	if(SET == USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
	{
		aRxBuf[nRxCnt++] = USART_ReceiveData(USART1);
		if(nRxCnt >= RX_BUF_LEN) nRxCnt = 0;
	}
}
