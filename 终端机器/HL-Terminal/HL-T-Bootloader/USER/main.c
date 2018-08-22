#include "led.h"
#include "delay.h"
#include "usart1.h"
#include "usart.h"
#include "flash.h"

int main()
{
	u8 tmp[100] = {0};
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//LED_Init();	
	//delay_init();
	USART1_Init(115200);
	while(1)
	{
		u32 old = 0;
		
		if(USART1_GetRXCount() > 0)
		{
			
			do{
				
				old = USART1_GetRXCount();
				delay_ms(10);
			}while(old != USART1_GetRXCount());
//			USART1_TX(USART1_GetRXCount());
//			tmp[USART1_GetRXCount()] = '\0';
//			USART1_SetRXCount(0);
//			USART1_TXStr(tmp);
		}
	}
	
	
	return 0;
}





