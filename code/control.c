/*
 * controler.c
 *
 *  Created on: 2024年12月1日
 *      Author: peril
 */

#include "control.h"
FPos aimDis = 40;
void getTrackState()
{
    if (lostStatus == 0)
    {
        if (lptsN > rptsN)
            trackState = TrackLeft;
        else if (lptsN < rptsN)
            trackState = TrackRight;
    }
}
float CIRCLE_AIM = 43;
void updCtrlVal()
{

    // 根据速度与状态，调整预瞄点距离
    // 简单策略：随目标速度线性调整预瞄距离
    aimDis = 10.0f + targetV * 2.5f;

    if (tptsN < 2)
    {
        tptsN = 5;
    }
    int l = 0, r = tptsN - 1, mid;
    double x, y; // 预瞄点坐标
    // 获取预瞄点坐标
    // 使用 tptsN 中的点，按距离选择预瞄点
    if (tptsN > 0)
    {
        int idx = (int)fmin((double)(tptsN - 1), fmax(0.0, aimDis / RESAMPLE_DIS));
        x = trackPts[0][idx];
        y = trackPts[1][idx];
    }
    else
    {
        x = FRONTWHEEL_X;
        y = FRONTWHEEL_Y;
    }
    // 计算舵机打角与两轮差速
    curDir = atan((FRONTWHEEL_X - x) / (FRONTWHEEL_Y - y));
    if (FRONTWHEEL_X - x > 1e-10)
    {
        double radius = ((FRONTWHEEL_X - x) * (FRONTWHEEL_X - x) + (FRONTWHEEL_Y - y) * (FRONTWHEEL_Y - y)) / 2 / (FRONTWHEEL_X - x); // 转弯半径，以左转为正
        setTargetSpeedL(targetV * sqrt((radius - CAR_HALF_WIDTH) * (radius - CAR_HALF_WIDTH) + (CAR_HALF_LENGTH) * (CAR_HALF_LENGTH)) / fabs(radius));
        setTargetSpeedR(targetV * sqrt((radius + CAR_HALF_WIDTH) * (radius + CAR_HALF_WIDTH) + (CAR_HALF_LENGTH) * (CAR_HALF_LENGTH)) / fabs(radius));
    }
    else
    {
        setTargetSpeedL(targetV);
        setTargetSpeedR(targetV);
    }
}

inline uint8 lCircleFound()
{
    return (lptVFound() && VId[0] < 37) && (lostStatus & 1) == 0 && (lostStatus & 2) == 0 && (!rptVFound()) && (!rptLFound());
}
inline uint8 rCircleFound()
{
    return (rptVFound() && VId[1] < 37) && (lostStatus & 1) == 0 && (lostStatus & 2) == 0 && (!lptVFound()) && (!lptLFound());
}
inline uint8 crossFound()
{
    if ((rptLFound() && lptLFound()))
    {
        return 1;
    }
    else if (rptLFound() && ((lGoesOut() && rGoesOut())))
    {
        return 2;
    }
    else if (lptLFound() && ((rGoesOut() && lGoesOut())))
    {
        return 3;
    }
    return FALSE;
}
inline uint8 rampDetect()
{
    return lostStatus == 0 && lptsN > 10 && rptsN > 10 && (!lptJFound()) && (!lptLFound()) && (!lptVFound()) && (!rptJFound()) && (!rptLFound()) && (!rptVFound()) && (borderRPts[0][lptsN - 2] - borderLPts[0][lptsN - 2]) > RAMP_THRES;
}
inline uint8 rampDetectDown()
{
    return lostStatus == 0 && (!lptJFound()) && (!lptLFound()) && (!lptVFound()) && (!rptJFound()) && (!rptLFound()) && (!rptVFound()) && (borderRPts[0][lptsN - 2] - borderLPts[0][lptsN - 2]) > RAMP_THRES_DOWN;
}

// 这些东西应该放到头文件里去……，不过为了调试逻辑方便放在这里也未尝不可，用到啥就用啥
uint16 cirReady = 0;   // 圆环准备状态
uint8 rampFlag = 0;   // 坡道标志
float CURVE_DEFAULT = 6.0f;   // 曲线默认速度
float STRAIGHT_DEFAULT = 9.0f;  // 直线默认速度
float CIR_RUN_DEFAULT = 7.0f;   // 圆环运行默认速度（备用值，实际代码中未直接使用）
#define CROSS_SPEED (5.0f)   // 十字路口外速度
#define CROSS_MID_SPEED (3.0f)  // 十字路口中间速度
#define UP_RAMP_SPEED (8.0f)  // 上坡速度
#define RAMP_SPEED (6.0f)   // 坡上速度
#define DOWN_RAMP_SPEED (4.0f)   // 下坡速度
#define CIR_SPEED (7.0f)    // 圆环速度
#define CURVE_LATER (0.5f) // 曲线预瞄系数
#define STRAIGHT_LATER (1.0f) // 直线预瞄系数
#define CIRCLE_LATER (0.8f)   // 圆环预瞄系数

void updState()
{
    getTrackState();
    // 这部分代码是灵魂！如何切换状态，请想清楚！！
    switch (curState)
    {
    case StopCar: // stopCar 由camera.c中设置
        system_delay_ms(200);
        targetV = 0;
        break;
    case GoStraight:
        targetV = STRAIGHT_DEFAULT; // 直线加速
        (void)0;
        break;
    case GoCurve:
        targetV = CURVE_DEFAULT; // 曲线减速
        (void)0;
        break;
    case GoCross: // 这里到GoCrossMid的状态切换在camera.c中完成
        targetV = CROSS_SPEED;
        (void)0;
        break;
    case GoCrossMid:
        targetV = CROSS_MID_SPEED;
        (void)0;
        break;
    case GoCrossAfter:
        targetV = CROSS_MID_SPEED;
        (void)0;
        break;
    case GoUpRamp:
        targetV = UP_RAMP_SPEED; // 上坡加速
        (void)0;
        break;
    case GoRamp:
        targetV = RAMP_SPEED;
        (void)0;
        break;
    case GoDownRamp:
        targetV = DOWN_RAMP_SPEED; // 下坡减速
        (void)0;
        break;
    case GoCircle:
        targetV = CIR_SPEED; // 圆环中速
        switch (circleState)
        {
        case CircleFound:
            (void)0;
            break;
        case CirclePre:
            (void)0;
            break;
        case CircleIn:
            (void)0;
            break;
        case CircleRunPre:
            (void)0;
            break;
        case CircleRun:
            (void)0;
            break;
        case CircleOutFound:
            (void)0;
            break;
        case CircleOutPre:
            (void)0;
            break;
        case CircleOut:
            (void)0;
            break;
        case CircleEnd:
            (void)0;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
