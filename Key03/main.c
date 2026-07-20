#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "bsp_key.h"
int main(void)
{
GPIO_QuickInit_Output(GPIOF, GPIO_Pin_9);
  GPIO_SetBits(GPIOF, GPIO_Pin_9);
	HseClock_Init(8, 336, 2, 7); 
	SysTick_Config(SystemCoreClock / 1000); 

  while(1) {
    GPIO_ToggleBits(GPIOF, GPIO_Pin_9); 
    Delay_ms(500); 
  }
}
//对时钟树初步了解自己初步配置hse使能初始化函数
