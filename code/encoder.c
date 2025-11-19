/*
 * encoder.c
 *
 *  Created on: 2024年10月27日
 *      Author: peril
 */

#include "encoder.h"


EncoderReturnValue preEncoder[256],sum;
uint8 tag;

void initEncoder(){
    encoder_dir_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                      // 初始化编码器模块与引脚 方向解码编码器模式
    encoder_dir_init(ENCODER_4, ENCODER_4_A, ENCODER_4_B);                      // 初始化编码器模块与引脚 方向解码编码器模式
    sum=(EncoderReturnValue){0,0}; tag=0;
    memset(preEncoder,0,sizeof(preEncoder));
}

EncoderReturnValue encoderRes(){
    int16 encoder_data_2 = encoder_get_count(ENCODER_2);                              // 获取编码器计数
    encoder_clear_count(ENCODER_2);                                             // 清空编码器计数

    int16 encoder_data_4 = encoder_get_count(ENCODER_4);                              // 获取编码器计数
    encoder_clear_count(ENCODER_4);                                             // 清空编码器计数
    // 使得encoder返回值一段时间内值的累加
    sum=(EncoderReturnValue){sum.l-preEncoder[tag].l,sum.r-preEncoder[tag].r};
    preEncoder[tag]=(EncoderReturnValue){encoder_data_2,encoder_data_4};
    sum=(EncoderReturnValue){sum.l+preEncoder[tag].l,sum.r+preEncoder[tag].r};
    tag++; if(tag>ENCODER_INT_WIDTH) tag=0;

    return sum;
}
