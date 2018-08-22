#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f10x.h"


void USART1_Init(u32 baudrate);
void USART1_TX(u8 data);
void USART1_TXStr(u8 *str);

void USART1_SetRXCount(u32 count);
u16 USART1_GetRXCount(void);
void USART1_GetRXData(u8 *tmp, u16 data_len);

#endif
