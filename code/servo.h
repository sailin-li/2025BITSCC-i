/*
 * servo.h
 *
 *  Created on: 2024年10月27日
 *      Author: peril
 * Function: 舵机控制
 */

#include "zf_common_headfile.h"
#include "pid.h"

#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_

#define SERVO_MOTOR_PWM (ATOM1_CH1_P33_9) // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ (50)             // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MID (1500)      // 舵机中值（us），示例值，需用测量校准
#define SERVO_DELTA_MAX (400) // 舵机打角限幅（us），示例值
#define SERVO_MIN (SERVO_MID - SERVO_DELTA_MAX)
#define SERVO_MAX (SERVO_MID + SERVO_DELTA_MAX)

#define SERVO_INIT_P (4.0f)
#define SERVO_INIT_I (0.0f)
#define SERVO_INIT_D (0.02f)

void initServo(UTimeStamp clock);                 // 初始化舵机
float updateServo(float input, UTimeStamp clock); // 更新舵机
void setTargetA(float newAngle);                  // 设置舵机目标角度

extern PIDObject servoPID;

#endif /* CODE_SERVO_H_ */
