/*
 * buzzer.h
 *
 *  Created on: 2024年10月27日
 *      Author: peril
 * Function:
 *   本头文件提供了主板上蜂鸣器的诸多操作。
 */

#ifndef CODE_BUZZER_H_
#define CODE_BUZZER_H_
#include "zf_common_headfile.h"
#define BUZZER_PIN              (P33_10)
#define LEN_UNIT 100
#define SHORT_BUZZ LEN_UNIT
#define LONG_BUZZ (LEN_UNIT*3)
#define WAIT_SPACING LEN_UNIT
void initBuzzer(void);          // 初始化蜂鸣器
void startBuzzer(void);         // 开始蜂鸣
void stopBuzzer(void);          // 停止蜂鸣
void shortBuzz(void);           // 短鸣一次
void longBuzz(void);            // 长鸣一次
void buzzLetter(char c);        // 以摩斯电码发出一个字母
void buzzSentence(char *str);   // 以摩斯电码发出一短字符串

#endif /* CODE_BUZZER_H_ */
