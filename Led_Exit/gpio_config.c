#include "gpio_config.h"


void GPIO_QuickInit_Output(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};  // 定义并清空GPIO初始化结构体（防止残留值污染）

    u32 index = ((u32)GPIOx - GPIOA_BASE) / 0x400;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << index, ENABLE);

    /* ----- 2. 配置GPIO结构体参数 ----- */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;           // 指定要配置的引脚（支持多引脚用 | 连接）
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     // 模式：通用输出模式
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    // 输出类型：推挽输出（既能输出高电平，也能输出低电平）
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;  // 翻转速度：2MHz（适合普通IO，省电且EMI干扰小）
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  // 上下拉：无（输出模式下一般不需要上下拉）

    /* ----- 3. 调用库函数写入寄存器，完成初始化 ----- */
    GPIO_Init(GPIOx, &GPIO_InitStruct);            // 将配置写入GPIO的 MODER, OTYPER, OSPEEDR, PUPDR 寄存器
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


void KEY_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;   // GPIO 配置结构体
    EXTI_InitTypeDef EXTI_InitStructure;   // EXTI 配置结构体
    NVIC_InitTypeDef NVIC_InitStructure;   // NVIC（中断控制器）配置结构体

    /* ================= 步骤1：使能相关时钟 ================= */
    // GPIOA 时钟：GPIO 模块必须有时钟才能工作（STM32 所有外设默认时钟关闭，以省电）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // SYSCFG 时钟：EXTI 的引脚映射功能由 SYSCFG（系统配置控制器）提供，必须开启
    // 注意：SYSCFG 挂在 APB2 总线上，不要搞错总线
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* ================= 步骤2：配置 GPIO 引脚为输入模式 ================= */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;          // 选择 PA0
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;        // 输入模式（因为要读取外部信号）
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;      // 下拉输入：当外部无信号（悬浮）时，引脚被内部下拉电阻拉到低电平
                                                          // 因为我们用的是 KEY_UP（按下为高），所以空闲时应该为低，下拉最合适
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // 输入模式下速度不影响功能，但习惯上配置一个值
    GPIO_Init(GPIOA, &GPIO_InitStructure);               // 将配置写入 GPIOA 的寄存器

    /* ================= 步骤3：将 PA0 连接到 EXTI Line 0 ================= */
    // 外部中断线（EXTI Line）共有 16 条（Line0~Line15），每个 Line 可以连接到一个 GPIO 引脚。
    // 但哪个 GPIO 的哪个引脚连接到 Line0 是可配置的（通过 SYSCFG 寄存器）。
    // 这里将 PA0 映射到 EXTI Line0，即 PA0 的电平变化将触发 Line0 的中断。
    // 参数：端口源 = GPIOA，引脚源 = Pin0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    /* ================= 步骤4：配置 EXTI Line0 的触发方式 ================= */
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;                 // 选择 EXTI Line0
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        // 中断模式（还有事件模式，但中断模式最常用）
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        // 上升沿触发：当 PA0 从低电平变为高电平时（即按键按下瞬间），触发中断
                                                                  // 因为 KEY_UP 高有效，所以采用上升沿（也可用双边沿，但通常只用一边）
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     // 使能该 Line
    EXTI_Init(&EXTI_InitStructure);                               // 将配置写入 EXTI 寄存器

    /* ================= 步骤5：配置 NVIC（嵌套向量中断控制器） ================= */
    // NVIC 负责管理所有中断的优先级和使能。即使 EXTI 配置好了，如果 NVIC 不使能，CPU 仍然不会响应中断。
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;    // EXTI0 的中断通道号（在 stm32f4xx.h 中定义）
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;             // 抢占优先级 = 2（数值越小优先级越高）
                                                                           // 注意：优先级分组默认通常是 4 位抢占 + 0 位子优先级（需在 SystemInit 或 main 中设置）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;             // 子优先级 = 0（当抢占优先级相同时，子优先级决定顺序）
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;        // 使能该中断通道
    NVIC_Init(&NVIC_InitStructure);                                      // 将配置写入 NVIC 寄存器
}

