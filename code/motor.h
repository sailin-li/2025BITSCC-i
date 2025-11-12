/*
 * motor.h
 *
 *  Created on: 2024年11月2日
 *      Author: peril
 * Function:
 *   通过此中间件控制马达
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_
#include "zf_common_headfile.h"
#include "pid.h"
#include "status.h"

#define MOTOR_INIT_P (0.8f)
#define MOTOR_INIT_I (0.10f)
#define MOTOR_INIT_D (0.01f)

#define MAX_DUTY (50) // 最大 MAX_DUTY% 占空比，限幅非常重要，一定要添加，在尝试让车子能跑的阶段不要改限幅。
#define DIR_R1 (P02_4)
#define PWM_R1 (ATOM0_CH5_P02_5)
#define DIR_L1 (P02_6)
#define PWM_L1 (ATOM0_CH7_P02_7)

#define DIR_R2 (P21_2)
#define PWM_R2 (ATOM0_CH1_P21_3)
#define DIR_L2 (P21_4)
#define PWM_L2 (ATOM0_CH3_P21_5)

void initMotor(UTimeStamp clock);                 // 初始化马达
void updateMotorL(float input, UTimeStamp clock); // **控制**: 用左轮转速与时间戳pid更新马达转速，返回改变后的马达duty
void updateMotorR(float input, UTimeStamp clock); // **控制**: 用右轮转速与时间戳pid更新马达转速，返回改变后的马达duty
void updP(float v);
void updI(float v);
void updD(float v);                   // **设定**: 设定马达PID参数
void setTargetSpeedL(float newSpeed); // **设定**: 设定左马达目标转速，需要update以实际控制
void setTargetSpeedR(float newSpeed); // **设定**: 设定左马达目标转速，需要update以实际控制

extern PIDObject motorPIDL; // 左轮PID对象
extern PIDObject motorPIDR; // 右轮PID对象

#endif /* CODE_MOTOR_H_ */
