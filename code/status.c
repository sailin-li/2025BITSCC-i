/*
 * status.c
 *
 *  Created on: 2024年11月11日
 *      Author: peril
 */
#include "status.h"


CurState curState=GoStraight;
CircleDir circleDir=NoCircle;
CircleState circleState=CircleNone;
uint8 STOP_FLAG=FALSE;
uint8 inflectPoint=0;
// 01234567:  左尖角角点，左直角角点，左弧线，长直道，右尖角角点，右直角角点，右弧线，NC
uint8 lostStatus=0;
// 01234567: 左丢线，右丢线，NC*6
uint16 VId[2]; // 第一个锐角角点的编号
uint16 LId[2]; // 第一个直角角点的编号
float targetV=9;        // 目标速度
uint16 BWCount;
double curDir=0;                  // 当前方向，由图像得出
uint8 finishFlag=TRUE;

uint8 lptVFound(){       // 发现左尖角
    return inflectPoint&1;
}
uint8 lptLFound(){       // 发现左直角
    return (inflectPoint>>1)&1;
}
uint8 lptJFound(){       // 发现左弧线
    return (inflectPoint>>2)&1;
}
uint8 longRoadFound(){   // 发现长直道
    return (inflectPoint>>3)&1;
}
uint8 rptVFound(){       // 发现右尖角
    return (inflectPoint>>4)&1;
}
uint8 rptLFound(){       // 发现右直角
    return (inflectPoint>>5)&1;
}
uint8 rptJFound(){       // 发现右弧线
    return (inflectPoint>>6)&1;
}
uint8 clearInflectPoint(){   // 清除所有角点
    return inflectPoint=1<<3; // 默认只有长直道
}
uint8 rstInflectPoint(uint8 Vf,uint8 Lf,uint8 Jf,uint8 lrType){ // 设置一边角点
    if(Jf==TRUE||Lf==TRUE||Vf==TRUE) inflectPoint&=~(1<<3); // 长直道标志清除
    inflectPoint|=(Vf<<(0+4*lrType))|(Lf<<(1+4*lrType))|(Jf<<(2+4*lrType));
    return inflectPoint;
}
uint8 resetInflectPoint(uint8 lVf,uint8 lLf,uint8 lJf,uint8 longRd,uint8 rVf,uint8 rLf,uint8 rJf){
    return inflectPoint=(lVf<<0)|(lLf<<1)|(lJf<<2)|(longRd<<3)|(rVf<<4)|(rLf<<5)|(rJf<<6);
}
