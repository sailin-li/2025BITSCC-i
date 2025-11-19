/*
 * pid.h
 *
 *  Created on: 2024年11月2日
 *      Author: peril
 * Function:
 *   PID 控制器
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#define UTimeStamp double
typedef enum{
    posPID,incPID
}PIDType;
typedef struct{
    float IMAX; // IMAX to restrict integral
    float kp,ki,kd; // pid constants
    float cur,tar; // current value and target value
    float lastErr,intErr; // last error and (posType)integral of all error or (incType) pre-last error
    float output; // final controling output
    float lastTime,beginTime;
    PIDType pidType;
}PIDObject;
PIDObject newPid(float kp,float ki,float kd,float current,float target,float time,float IMAX,PIDType pidType);
void initPid(PIDObject *obj,float kp, float ki,float kd,float current,float target,float time,float IMAX,PIDType pidType);
void setP(PIDObject *obj,float kp);
void setI(PIDObject *obj,float ki);
void setD(PIDObject *obj,float kd);
void setTarget(PIDObject *obj,float target);
void setIMAX(PIDObject *obj,float IMAX);
void clearAll(PIDObject *obj,float time);
float update(PIDObject *obj,float curVal,float time);
float getOutput(PIDObject *obj);
#endif /* CODE_PID_H_ */
