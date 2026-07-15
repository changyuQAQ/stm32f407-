#include "bsp_key.h"
Key_TypeDef KeyList[] = {
    {GPIOA, GPIO_Pin_0, 1},  // KEY_UP：下拉输入，高有效
    {GPIOE, GPIO_Pin_2, 0},  // KEY0：上拉输入，低有效
    {GPIOE, GPIO_Pin_3, 0},  // KEY1：上拉输入，低有效
    {GPIOE, GPIO_Pin_4, 0},  // KEY2：上拉输入，低有效
};

// 读取原始电平并转换为“按下=1”
uint8_t Key_ReadRaw(uint8_t index)
{
    uint8_t level = GPIO_ReadInputDataBit(KeyList[index].port, KeyList[index].pin);
    return (level == KeyList[index].activeLevel) ? 1 : 0;
}

// 简单阻塞延时消抖（示例，实际项目建议非阻塞）
uint8_t Key_Scan(void)
{
    static uint8_t lastState[KEY_NUM] = {0};
    uint8_t currentState[KEY_NUM];
    uint8_t i;

    // 第一次读取
    for (i = 0; i < KEY_NUM; i++) {
        currentState[i] = Key_ReadRaw(i);
    }

    delay(20);

    // 第二次读取并检测下降沿（按下瞬间）
    for (i = 0; i < KEY_NUM; i++) {
        currentState[i] = Key_ReadRaw(i);
        if (currentState[i] == 1 && lastState[i] == 0) {
            lastState[i] = currentState[i];   // 更新状态
            return i+1;                         // 返回按键编号
        }
        lastState[i] = currentState[i];
    }
    return 0;
}
