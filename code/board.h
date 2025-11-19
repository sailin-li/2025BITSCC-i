/*
 * board.h
 *
 *  Created on: 2024年11月2日
 *      Author: peril
 * Function:
 *   本头文件提供了主板上诸多可操作物理互动元素的定义。
 *   用于控制 LED、按键、开关。
 */

#ifndef CODE_BOARD_H_
#define CODE_BOARD_H_
#include "zf_common_headfile.h"

#define LED1                    (P20_9)
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)

#define KEY1                    (P20_6)
#define KEY2                    (P20_7)
#define KEY3                    (P11_2)
#define KEY4                    (P11_3)

#define SWITCH1                 (P33_11)
#define SWITCH2                 (P33_12)

#define LED_ON 1
#define LED_OFF 0

void initBoard(void);

#endif /* CODE_BOARD_H_ */
