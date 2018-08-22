#include "led.h"
#include "delay.h"
#include "usart1.h"


int main()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();	
	delay_init();
	USART1_Init(115200);
	
	while(1)
	{
		
	}
	
	
	return 0;
}





