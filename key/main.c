#include "stm32f4xx.h"
#include "gpio_config.h"
#include "bsp_key.h"
int main(void)
{    
    GPIO_QuickInit_Output(GPIOF, GPIO_Pin_9);
		GPIO_SetBits(GPIOF, GPIO_Pin_9);
		Key_Init();
    while (1)
    {
        u8 keyId = Key_Scan();

        if (keyId != 0)
        {
            // 无论哪个按键按下，都翻转 LED
             GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
        }
    }
}
