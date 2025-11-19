/*
 * encoder.h
 *
 *  Created on: 2024年10月27日
 *      Author: peril
 * Function:
 *   从编码器获取车轮转速信息，用于 pid 控车速
 */

#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_
#include "zf_common_headfile.h"
#include "status.h"
#define ENCODER_2                   (TIM5_ENCODER)
#define ENCODER_2_A                 (TIM5_ENCODER_CH1_P10_3)
#define ENCODER_2_B                 (TIM5_ENCODER_CH2_P10_1)

#define ENCODER_4                   (TIM6_ENCODER)
#define ENCODER_4_A                 (TIM6_ENCODER_CH1_P20_3)
#define ENCODER_4_B                 (TIM6_ENCODER_CH2_P20_0)

#define ENCODER_INT_WIDTH (32)   // encoder返回值一段时间内值的累加，此乃该区间长度，此值应当小于等于256

typedef struct{
    int16 l,r;
} EncoderReturnValue;
void initEncoder(void);
EncoderReturnValue encoderRes(void);


#endif /* CODE_ENCODER_H_ */
