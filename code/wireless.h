/*
 * wireless.h
 *
 *  Created on: 2024年11月10日
 *      Author: peril
 * Function: 用于初始化通信模块，提供通信模块函数
 */

#ifndef CODE_WIRELESS_H_
#define CODE_WIRELESS_H_
#include "zf_common_headfile.h"
#include "board.h"

void initWireless(void);
extern uint8 WlDataBuffer[32];
extern uint8 WlDataLen;

#endif /* CODE_WIRELESS_H_ */
