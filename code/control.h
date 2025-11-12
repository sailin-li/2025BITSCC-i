/*
 * controler.h
 *
 *  Created on: 2024年12月1日
 *      Author: peril
 * Function:
 *  根据摄像头获取到的信息，作为中间件，传递给控制模块
 *  在此有状态机
 */
#ifndef CODE_CONTROLER_H_
#define CODE_CONTROLER_H_

#include "camera.h"
#include "status.h"
#include "motor.h"

#define RAMP_THRES (60)      // 判定为上坡道时路宽阈值（像素），可根据摄像头视角调整
#define RAMP_THRES_DOWN (60) // 判定为下坡道时路宽阈值（像素）

void getTrackState(void); // 获取巡线状态
void updCtrlVal(void);    // 更新控制值
void updState(void);      // 主要方法，更新状态

extern FPos aimDis;     // 预瞄点距离
extern uint16 cirReady; // 走圆环元素状态
#endif
