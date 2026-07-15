//key.h
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define KEY_NUM 4

#include "stm32f4xx.h"
#include "gpio_config.h"

typedef struct {
    GPIO_TypeDef* port;    // GPIO端口
	  uint16_t pin;          // 引脚号
    uint8_t activeLevel;   // 有效电平：0=低电平有效，1=高电平有效
} Key_TypeDef;



// 函数声明
uint8_t Key_ReadRaw(uint8_t index);
uint8_t Key_Scan(void);    // 返回值：按键ID（1~4），0表示无按键

#endif
