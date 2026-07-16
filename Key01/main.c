#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "gpio_config.h"
#include "bsp_key.h"
int main(void)
{
	GPIO_QuickInit_Output(GPIOF, GPIO_Pin_9);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	Key_Init();
	SysTick_Init();
	static uint32_t lastTime = 0;
  uint32_t now;

  while (1)
  {
    now = GetTick();  // 获取当前时间（例如 HAL_GetTick()）
    if ((now - lastTime) >= 10) {   // 每20ms执行一次扫描
      lastTime = now;        
      if (Key_Scan()) 
  	  {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_9);              
      }
    }
  }
}
