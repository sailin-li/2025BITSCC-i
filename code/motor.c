/*
 * motor.c
 *
 *  Created on: 2024年11月2日
 *      Author: peril
 */
#include "motor.h"
PIDObject motorPIDL={0,0,0,0,0,0,0,0,0,0,0,incPID};
PIDObject motorPIDR={0,0,0,0,0,0,0,0,0,0,0,incPID};
float motorKp=MOTOR_INIT_P;
float motorKi=MOTOR_INIT_I;
float motorKd=MOTOR_INIT_D;
#define DUTY_MAX 65
float dutyL=0,dutyR=0;
void initMotor(UTimeStamp clock){
    initPid(&motorPIDL,motorKp,motorKi,motorKd,0.0,0,clock/1000.0,1e5,incPID);
    initPid(&motorPIDR,motorKp,motorKi,motorKd,0.0,0,clock/1000.0,1e5,incPID);
    gpio_init(DIR_R1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R1, 17000, 0);                                                 // PWM 通道初始化频率 17KHz 占空比初始为 0
    gpio_init(DIR_L1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L1, 17000, 0);                                                 // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_R2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R2, 17000, 0);                                                 // PWM 通道初始化频率 17KHz 占空比初始为 0
    gpio_init(DIR_L2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                           // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L2, 17000, 0);                                                 // PWM 通道初始化频率 17KHz 占空比初始为 0
}
void updateMotorL(float input,UTimeStamp clock){
    float time=clock/1000.0;
    float output=update(&motorPIDL,input,time);

    if(dutyL<output){   // smoother curve
        if(output-dutyL>1) dutyL+=1;
        else dutyL=output;
    }else if(dutyL>output){
        if(dutyL-output>1) dutyL-=1;
        else dutyL=output;
    }
    dutyL=output;
    if(dutyL>DUTY_MAX) dutyL=DUTY_MAX;
    if(dutyL<-DUTY_MAX) dutyL=-DUTY_MAX;
    if(dutyL>0){
        gpio_set_level(DIR_L1, GPIO_HIGH);                                  // DIR输出高电平
        pwm_set_duty(PWM_L1, dutyL * (PWM_DUTY_MAX / 100));                  // 计算占空比
        gpio_set_level(DIR_L2, GPIO_HIGH);                                  // DIR输出高电平
        pwm_set_duty(PWM_L2, dutyL * (PWM_DUTY_MAX / 100));                  // 计算占空比
    }else{
        gpio_set_level(DIR_L1, GPIO_LOW);                                   // DIR输出低电平
        pwm_set_duty(PWM_L1, (-dutyL) * (PWM_DUTY_MAX / 100));               // 计算占空比
        gpio_set_level(DIR_L2, GPIO_LOW);                                   // DIR输出低电平
        pwm_set_duty(PWM_L2, (-dutyL) * (PWM_DUTY_MAX / 100));               // 计算占空比
    }
}
void updateMotorR(float input,UTimeStamp clock){
    float time=clock/1000.0;
    float output=update(&motorPIDR,input,time);

    if(dutyR<output){   // smoother curve
        if(output-dutyR>1) dutyR+=1;
        else dutyR=output;
    }else if(dutyR>output){
        if(dutyR-output>1) dutyR-=1;
        else dutyR=output;
    }
    dutyR=output;
    if(dutyR>DUTY_MAX) dutyR=DUTY_MAX;
    if(dutyR<-DUTY_MAX) dutyR=-DUTY_MAX;
    if(dutyR>0){
        gpio_set_level(DIR_R1, GPIO_HIGH);                                  // DIR输出高电平
        pwm_set_duty(PWM_R1, dutyR * (PWM_DUTY_MAX / 100));                  // 计算占空比
        gpio_set_level(DIR_R2, GPIO_HIGH);                                  // DIR输出高电平
        pwm_set_duty(PWM_R2, dutyR * (PWM_DUTY_MAX / 100));                  // 计算占空比
    }else{
        gpio_set_level(DIR_R1, GPIO_LOW);                                   // DIR输出低电平
        pwm_set_duty(PWM_R1, (-dutyR) * (PWM_DUTY_MAX / 100));               // 计算占空比
        gpio_set_level(DIR_R2, GPIO_LOW);                                   // DIR输出低电平
        pwm_set_duty(PWM_R2, (-dutyR) * (PWM_DUTY_MAX / 100));               // 计算占空比
    }
}
void updP(float v){
    setP(&motorPIDL,v);
    setP(&motorPIDR,v);
}
void updI(float v){
    setI(&motorPIDL,v);
    setI(&motorPIDR,v);
}
void updD(float v){
    setD(&motorPIDL,v);
    setD(&motorPIDR,v);
}
void setTargetSpeedL(float newSpeed){
    setTarget(&motorPIDL,newSpeed);
}
void setTargetSpeedR(float newSpeed){
    setTarget(&motorPIDR,newSpeed);
}
