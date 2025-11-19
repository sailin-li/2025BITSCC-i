/*
 * status.h
 *
 *  Created on: 2024年11月11日
 *      Author: peril
 */

#ifndef CODE_STATUS_H_
#define CODE_STATUS_H_
#include "zf_common_headfile.h"

#define CAR_HALF_WIDTH (<参数自己调>)       // 半车宽
#define CAR_HALF_LENGTH (<参数自己调>)      // 半车长
#define BACKWHEEL_X ((MAXX)/2)             // 后轮中心横坐标
#define BACKWHEEL_Y (MAXY+<参数自己调>)     // 后轮中心纵坐标
#define FRONTWHEEL_X ((MAXX)/2)            // 前轮中心横坐标
#define FRONTWHEEL_Y (BACKWHEEL_Y-2*CAR_HALF_LENGTH)    // 前轮中心纵坐标

typedef enum{
    StopCar,            // 斑马线停车
    GoStraight,         // 直线行驶
    GoCurve,            // 弯道
    GoCross,            // 经过十字路口
    GoCrossMid,         // 经过十字路口
    GoUpRamp,           // 上坡
    GoRamp,             // 坡上
    GoDownRamp,         // 下坡
    GoCircle,           // 环路
    GoCrossAfter
}CurState;
typedef enum{
    NoCircle,           // 无圆环
    LeftCircle,         // 左圆环（圆环在直道左侧）
    RightCircle         // 右圆环（圆环在直道右侧）
}CircleDir;
typedef enum{
    CircleNone,         // 非圆环状态
    CircleFound,        // 看到角点
    CirclePre,          // 角点消失，角点侧丢线
    CircleIn,           // 角点侧不丢线，非角点侧未开始丢线
    CircleRunPre,       // 角点侧不丢线，非角点侧丢线
    CircleRun,          // 不丢线
    CircleOutFound,     // 发现新角点
    CircleOutPre,       // 角点侧丢线
    CircleOut,          // 角点侧不丢线，非角点侧不丢线
    CircleEnd           // 非角点侧丢线
}CircleState;

typedef struct{
   float l,r;
} SpeedValue; // unit: r/s

extern CurState curState;
extern CircleDir circleDir;
extern CircleState circleState;
extern uint8 STOP_FLAG;      // 强制停止标志（调试用，用于在失控情况下立刻暂停）
extern uint8 lostStatus;     // 丢线状态
extern uint16 VId[2];        // 左右锐角点 id
extern uint16 LId[2];        // 左右直角点 id
extern float curDir;         // 当前方向，由图像得出
extern float targetV;        // 目标速度
extern uint16 BWCount;       // 黑白跳变个数
extern uint8 finishFlag;

uint8 lptVFound(void);
uint8 lptLFound(void);
uint8 lptJFound(void);
uint8 rptVFound(void);
uint8 rptLFound(void);
uint8 rptJFound(void);
uint8 longRoadFound(void);
uint8 resetInflectPoint(uint8 lVf,uint8 lLf,uint8 lJf,uint8 longRd,uint8 rVf,uint8 rLf,uint8 rJf);
uint8 clearInflectPoint(void);
uint8 rstInflectPoint(uint8 Vf,uint8 Lf,uint8 Jf,uint8 lrType);

#endif /* CODE_STATUS_H_ */
