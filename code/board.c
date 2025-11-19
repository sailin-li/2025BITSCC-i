/*
 * board.c
 *
 *  Created on: 2024年11月2日
 *      Author: peril
 */
#include "board.h"

void initBoard(){
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);        // 初始化 LED1 输出 默认高电平 推挽输出模式 , 摄像头未准备灯
    gpio_init(LED2, GPO, GPIO_LOW, GPO_PUSH_PULL);         // 初始化 LED2 输出 默认低电平 推挽输出模式
    gpio_init(LED3, GPO, GPIO_LOW, GPO_PUSH_PULL);         // 初始化 LED3 输出 默认低电平 推挽输出模式
    gpio_init(LED4, GPO, GPIO_LOW, GPO_PUSH_PULL);         // 初始化 LED4 输出 默认低电平 推挽输出模式

    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认高电平 上拉输入
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY2 输入 默认高电平 上拉输入
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY3 输入 默认高电平 上拉输入
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY4 输入 默认高电平 上拉输入

    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_PULL_UP);        // 初始化 SWITCH1 输入 默认高电平 上拉输入
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_PULL_UP);        // 初始化 SWITCH2 输入 默认高电平 上拉输入
}
