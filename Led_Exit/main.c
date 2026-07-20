#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "bsp_key.h"
int main(void)
{
GPIO_QuickInit_Output(GPIOA, GPIO_Pin_0);
	GPIO_QuickInit_Output(GPIOF, GPIO_Pin_9);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
  KEY_EXTI_Init();
	while (1)
  {
		
  }
}
