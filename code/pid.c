/*
 * pid.c
 *
 *  Created on: 2024年11月2日
 *      Author: peril
 */
#include "pid.h"
PIDObject newPid(float kp, float ki, float kd, float current, float target, float time, float IMAX, PIDType pidType)
{
    float curTime = time;
    return (PIDObject){IMAX, kp, ki, kd, current, target, 0, 0, 0, curTime, curTime, pidType};
}
void initPid(PIDObject *obj, float kp, float ki, float kd, float current, float target, float time, float IMAX, PIDType pidType)
{
    *obj = newPid(kp, ki, kd, current, target, time, IMAX, pidType);
}
void setP(PIDObject *obj, float kp) { obj->kp = kp; }
void setI(PIDObject *obj, float ki) { obj->ki = ki; }
void setD(PIDObject *obj, float kd) { obj->kd = kd; }
void setTarget(PIDObject *obj, float target) { obj->tar = target; }
void setIMAX(PIDObject *obj, float IMAX) { obj->IMAX = IMAX; } // dangerous method
void clearAll(PIDObject *obj, float time)
{
    obj->intErr = 0;
    obj->output = 0;
    obj->lastErr = 0;
    obj->lastTime = obj->beginTime = time;
}
float update(PIDObject *obj, float curVal, float time)
{
    // 考的就是这个
    float dt = (float)(time - obj->lastTime);
    if (dt <= 0)
        return obj->output;
    float err = obj->tar - curVal;
    if (obj->pidType == posPID)
    {
        obj->intErr += err * dt;
        // 限幅
        if (obj->intErr > obj->IMAX)
            obj->intErr = obj->IMAX;
        if (obj->intErr < -obj->IMAX)
            obj->intErr = -obj->IMAX;
        float der = (err - obj->lastErr) / dt;
        obj->output = obj->kp * err + obj->ki * obj->intErr + obj->kd * der;
        obj->lastErr = err;
    }
    else
    { // incPID
        float deltaErr = err - obj->lastErr;
        float der = deltaErr / dt;
        obj->output += obj->kp * deltaErr + obj->ki * err * dt + obj->kd * der;
        obj->lastErr = err;
    }
    obj->lastTime = time;
    return obj->output;
}
float getOutput(PIDObject *obj) { return obj->output; } // don't need to use this
