/*
 * servo.c
 *
 *  Created on: 2024Äê11ÔÂ11ÈÕ
 *      Author: peril
 */

#include "servo.h"

PIDObject servoPID={0,0,0,0,0,0,0,0,0,0,0,posPID};
float servoKp=SERVO_INIT_P;
float servoKi=SERVO_INIT_I;
float servoKd=SERVO_INIT_D;
float dutyS=SERVO_MID;
void initServo(UTimeStamp clock){
    initPid(&servoPID,servoKp,servoKi,servoKd,0,0,clock/1000.0,1e5,posPID);
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, SERVO_MID);
}
float updateServo(float input,UTimeStamp clock){
    float time=clock/1000.0;
    float output=update(&servoPID,input,time);
    dutyS=output+SERVO_MID;
    if(dutyS>SERVO_MAX) dutyS=SERVO_MAX;
    if(dutyS<SERVO_MIN) dutyS=SERVO_MIN;
    pwm_set_duty(SERVO_MOTOR_PWM,dutyS);
    return dutyS;
}
void setTargetA(float newAngle){
    setTarget(&servoPID,newAngle);
}
