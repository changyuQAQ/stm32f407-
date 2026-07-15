#include "gpio_config.h"


void GPIO_QuickInit_Output(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //端口配置输出初始化
    // 时钟使能：根据端口基地址计算偏移位
    // GPIOA=0x40020000, GPIOB=0x40020400, ...
    // (GPIOx - GPIOA_BASE) / 0x400  => 0,1,2...9
    u32 index = ((u32)GPIOx - GPIOA_BASE) / 0x400;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << index, ENABLE);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE); //使能端口时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //输入模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//管脚设置
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化结构体
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //输入模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//管脚设置
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化结构体
}

void delay(ut32 i)
{
	while(i--);
}


