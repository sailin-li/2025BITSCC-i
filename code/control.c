/*
 * controler.c
 *
 *  Created on: 2024年12月1日
 *      Author: peril
 */

#include "control.h"
FPos aimDis=40;
void getTrackState(){
    if(lostStatus==0){
        if(lptsN>rptsN) trackState=TrackLeft;
        else if(lptsN<rptsN) trackState=TrackRight;
    }
}
float CIRCLE_AIM=43;
void updCtrlVal(){

    // 根据速度与状态，调整预瞄点距离
    aimDis = <代码挖空自己补>;

    if(tptsN<2){tptsN=5;}
    int l=0,r=tptsN-1,mid;
    double x,y;         // 预瞄点坐标
    // 获取预瞄点坐标
    <代码挖空自己补>
    // 计算舵机打角与两轮差速
    curDir=atan((FRONTWHEEL_X-x)/(FRONTWHEEL_Y-y));
    if(FRONTWHEEL_X-x>1e-10){
        double radius=((FRONTWHEEL_X-x)*(FRONTWHEEL_X-x)+(FRONTWHEEL_Y-y)*(FRONTWHEEL_Y-y))/2/(FRONTWHEEL_X-x);  // 转弯半径，以左转为正
        setTargetSpeedL(targetV*sqrt((radius-CAR_HALF_WIDTH)*(radius-CAR_HALF_WIDTH)+(CAR_HALF_LENGTH)*(CAR_HALF_LENGTH))/fabs(radius));
        setTargetSpeedR(targetV*sqrt((radius+CAR_HALF_WIDTH)*(radius+CAR_HALF_WIDTH)+(CAR_HALF_LENGTH)*(CAR_HALF_LENGTH))/fabs(radius));
    }else{
        setTargetSpeedL(targetV);
        setTargetSpeedR(targetV);
    }
}

inline uint8 lCircleFound(){
    return (lptVFound()&&VId[0]<37) && (lostStatus&1)==0 && (lostStatus&2)==0 && (!rptVFound()) && (!rptLFound());
}
inline uint8 rCircleFound(){
    return (rptVFound()&&VId[1]<37) && (lostStatus&1)==0 && (lostStatus&2)==0 && (!lptVFound()) && (!lptLFound());
}
inline uint8 crossFound(){
    if((rptLFound()&&lptLFound())){
        return 1;
    }else if(rptLFound()&&((lGoesOut()&&rGoesOut()))){
        return 2;
    }else if(lptLFound()&&((rGoesOut()&&lGoesOut()))){
        return 3;
    }
    return FALSE;
}
inline uint8 rampDetect(){
    return lostStatus==0 && lptsN>10 && rptsN>10 && (!lptJFound()) && (!lptLFound()) && (!lptVFound()) && (!rptJFound())
            && (!rptLFound()) &&(!rptVFound()) && (borderRPts[0][lptsN-2]-borderLPts[0][lptsN-2])>RAMP_THRES;
}
inline uint8 rampDetectDown(){
    return lostStatus==0 && (!lptJFound()) && (!lptLFound()) && (!lptVFound()) && (!rptJFound())
            && (!rptLFound()) &&(!rptVFound()) && (borderRPts[0][lptsN-2]-borderLPts[0][lptsN-2])>RAMP_THRES_DOWN;
}

// 这些东西应该放到头文件里去……，不过为了调试逻辑方便放在这里也未尝不可，用到啥就用啥
uint16 cirReady=<代码挖空自己补>;
uint8 rampFlag=<代码挖空自己补>;
float CURVE_DEFAULT=<代码挖空自己补>;
float STRAIGHT_DEFAULT=<代码挖空自己补>;
float CIR_RUN_DEFAULT=<代码挖空自己补>;
#define CROSS_SPEED (<代码挖空自己补>)
#define CROSS_MID_SPEED (<代码挖空自己补>)
#define UP_RAMP_SPEED (<代码挖空自己补>)
#define RAMP_SPEED (<代码挖空自己补>)
#define DOWN_RAMP_SPEED (<代码挖空自己补>)
#define CIR_SPEED (<代码挖空自己补>)
#define CURVE_LATER (<代码挖空自己补>)
#define STRAIGHT_LATER (<代码挖空自己补>)
#define CIRCLE_LATER (<代码挖空自己补>)

void updState(){
    getTrackState();
    // 这部分代码是灵魂！如何切换状态，请想清楚！！
    switch(curState){
        case StopCar: // stopCar 由camera.c中设置
            system_delay_ms(<代码挖空自己补>);
            targetV=0;
            break;
        case GoStraight:
            targetV=STRAIGHT_DEFAULT; // 直线加速
            <代码挖空自己补>
            break;
        case GoCurve:
            targetV=CURVE_DEFAULT; // 曲线减速
            <代码挖空自己补>
            break;
        case GoCross:   // 这里到GoCrossMid的状态切换在camera.c中完成
            targetV=CROSS_SPEED;
            <代码挖空自己补>
            break;
        case GoCrossMid:
            targetV=CROSS_MID_SPEED;
            <代码挖空自己补>
            break;
        case GoCrossAfter:
            targetV=CROSS_MID_SPEED;
            <代码挖空自己补>
            break;
        case GoUpRamp:
            targetV=UP_RAMP_SPEED; // 上坡加速
            <代码挖空自己补>
            break;
        case GoRamp:
            targetV=RAMP_SPEED;
            <代码挖空自己补>
            break;
        case GoDownRamp:
            targetV=DOWN_RAMP_SPEED; // 下坡减速
            <代码挖空自己补>
            break;
        case GoCircle:
            targetV=CIR_SPEED; // 圆环中速
            switch(circleState){
                case CircleFound:
                    <代码挖空自己补>
                    break;
                case CirclePre:
                    <代码挖空自己补>
                    break;
                case CircleIn:
                    <代码挖空自己补>
                    break;
                case CircleRunPre:
                    <代码挖空自己补>
                    break;
                case CircleRun:
                    <代码挖空自己补>
                    break;
                case CircleOutFound:
                    <代码挖空自己补>
                    break;
                case CircleOutPre:
                    <代码挖空自己补>
                    break;
                case CircleOut:
                    <代码挖空自己补>
                    break;
                case CircleEnd:
                    <代码挖空自己补>
                    break;
                default: break;
            }
            break;
        default: break;
    }
}
