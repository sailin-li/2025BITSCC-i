/*
 * camera.c
 *
 *  Created on: 2024年11月5日
 *      Author: peril
 */
// X in range 188
// Y in range 120
#include "camera.h"
uint8 cameraProcessFlag=FALSE;
FPos tmp1[MAXX+MAXY],tmp2[MAXX+MAXY];
UPix THRES=0;                                           // 小于等于记为0，大于记为1
FPos borderLPts[2][MAXX+MAXY],borderRPts[2][MAXX+MAXY];
FPos trackPts[2][MAXX+MAXY];
FAng borderLAng[MAXX+MAXY],borderRAng[MAXX+MAXY];
UDPos lptsN,rptsN,tptsN;
IFX_ALIGN(4) UPix outImg[MAXY][MAXX];
FAng genAngle=0;
TrackState trackState=TrackLeft;
void initCamera(){
    while(1){
        if(mt9v03x_init())
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        else break;
        system_delay_ms(100);                                                  // 闪灯表示异常
    }
    while(!mt9v03x_finish_flag);                        // 等待读取到图像
    THRES=getThres();
    gpio_set_level(LED1,0);                             // 关闭LED1提示摄像头初始化完成
}
inline UPix getPixelOrigin(UPos x,UPos y){              // 从未逆透视变换图上，获取横坐标为 x ,纵坐标为 y 点处的图像，即获取 y 行第 x 个位置上的像素值
    return mt9v03x_image[y][x];
}
inline UPix getPixel(UPos x,UPos y){                    // 从逆透视变换图上，获取横坐标为 x ,纵坐标为 y 点处的图像，即获取 y 行第 x 个位置上的像素值
    UPos u=INV_PLOT[y][x][0],v=INV_PLOT[y][x][1];
    return getPixelOrigin(u,v);
}
inline UPix getPixelWithCheck(UPos x,UPos y){           // 带边界检查的 getPixel
    if(x>250) x=0; else if(x>=MAXX) x=MAXX-1;
    if(y>250) y=0; else if(y>=MAXY) y=MAXY-1;
    UPos u=INV_PLOT[y][x][0],v=INV_PLOT[y][x][1];
    return getPixelOrigin(u,v);
}
inline uint8 pointIsValid(SPos x,SPos y){
    if(x>=MAXX||x<0||y>=MAXY||y<0) return FALSE;     // 本身超范围，直接返回假
    if(INV_PLOT[y][x][0]<MAXX-1&&INV_PLOT[y][x][0]>0&&INV_PLOT[y][x][1]<MAXY-1&&INV_PLOT[y][x][1]>0) return TRUE;
    return FALSE;
}
inline uint8 rectIsValid(SPos cx,SPos cy,SPos kernel){
    return pointIsValid(cx-kernel,cy-kernel)&&pointIsValid(cx+kernel,cy+kernel);
}
UPix getThres(){
    UDPos HistoGram[256]={0};                           // 灰度分布直方图
    // 这里你需要使用 ostu 法获取图像二值化阈值
    <代码挖空自己补>
    return thres;
}
inline UPix updateThres(){return THRES=getThres();}
static const SPos dirFront[4][2]={{0,-1},{1,0},{0,1},{-1,0}};           // {x,y}
static const SPos dirFrontLeft[4][2]={{-1,-1},{1,-1},{1,1},{-1,1}};     // {x,y}
static const SPos dirFrontRight[4][2]={{1,-1},{1,1},{-1,1},{-1,-1}};    // {x,y}
#define clip(x,minn,maxn) ((x)<(minn)?(minn):((x)>(maxn)?(maxn):(x)))
void findBorderLineL(UPos x,UPos y){ // 寻左边
    #define setNewPoint borderLPts[0][lptsN]=x;borderLPts[1][lptsN]=y;lptsN++
    #define UDir uint8
    UDir nxt=0,turn=0; lptsN=0;  setNewPoint;
    while(lptsN<MAXX+MAXY&&turn<4&&pointIsValid(x+3,y)&&pointIsValid(x-3,y-3)){
        // 获得局部二值化阈值
        <代码挖空自己补>
        // 迷宫法巡线
        <代码挖空自己补>
    }
    #undef UDir
    #undef setNewPoint
}
void findBorderLineR(UPos x,UPos y){ // 寻右边
    #define setNewPoint borderRPts[0][rptsN]=x;borderRPts[1][rptsN]=y;rptsN++
    #define UDir uint8
    UDir nxt=0,turn=0; rptsN=0;  setNewPoint;
    while(rptsN<MAXX+MAXY&&turn<4&&pointIsValid(x+3,y)&&pointIsValid(x-3,y-3)){
        // 获得局部二值化阈值
        <代码挖空自己补>
        // 迷宫法巡线
        <代码挖空自己补>
    }
    #undef UDir
    #undef setNewPoint
}
void triFiltering(UDPos ptsN,FPos *borderPts){   // 对巡线点三角滤波
    // 可以用 TRI_HALF_KERNEL 定义半卷积核大小
    <代码挖空自己补>
}
void resamplePoints(FPos *borderPts,UDPos *num){ // 在折线上根据 RESAMPLE_DIS 等距重采样
    <代码挖空自己补>
}
void getTurningAngles(FPos *borderPts,FAng *borderAng,UDPos num){ // 获得转角
    <代码挖空自己补>
}
inline void findCorner(FAng *borderAng,UDPos num,uint8 dir){
    // 寻找角点
    uint8 LFound=FALSE,VFound=FALSE,isCurve=FALSE;
    // 遍历所有点，判断角点类型
    <代码挖空自己补>
    // 记录角点类型
    rstInflectPoint(VFound,LFound,isCurve,dir);
}
void imageProcess(){
//    while(cameraProcessFlag==TRUE);
//    cameraProcessFlag=TRUE;
    // 从近处左右寻黑白跳变点
    UPos xl=MAXX/2-BEGIN_X,xr=MAXX/2+BEGIN_X,y0=BEGIN_Y;
    for(;xl>0;xl--)         if(getPixel(xl,y0)<=THRES) break;
    for(;xr<MAXX-1;xr++)    if(getPixel(xr,y0)<=THRES) break;
    lostStatus=0;
    if(INV_PLOT[y0][xl][0]<LOSS_THRES) lostStatus|=1;
    if(INV_PLOT[y0][xr][0]>MAXX-1-LOSS_THRES) lostStatus|=2;
    if(lostStatus!=3&&curState==GoCrossMid){
        curState=GoCrossAfter;
    }
    switch(lostStatus){
        case 1: // 左边丢失
            trackState=TrackRight; break;
        case 2: // 右边丢失
            trackState=TrackLeft; break;
        case 3: // 左右丢失
            if(curState==GoCross){
                curState=GoCrossMid;
            }
            break;
        default: break;
    }
    if(curState==GoCrossMid||(curState==GoCross&&genAngle>1e-8)){ // 巡远线
        int xMid; y0 = BEGIN_FAR_Y;
        if(curState==GoCross&&(genAngle<3*PI/5 && genAngle>2*PI/5)){
            xMid=(MAXX/2)+(BEGIN_FAR_Y-BEGIN_Y)/tan(genAngle);
        }else{
            xMid=MAXX/2;
        }
        if(getPixel((UPos)xMid,y0)<=THRES){
            for(int i=1;xMid-i-1>=0 && xMid+i+1<MAXX;i+=2){
                if(getPixel((UPos)xMid-i,y0)>THRES){
                    xMid=xMid-i-1; break;
                }else if(getPixel((UPos)xMid+i,y0)>THRES){
                    xMid=xMid+i+1; break;
                }
            }
        }
        xl=xMid-BEGIN_X; xr=xMid+BEGIN_X;
        for(;xl>0;xl--)     if(getPixel(xl,y0)<=THRES) break;
        for(;xr<MAXX-1;xr++)if(getPixel(xr,y0)<=THRES) break;
        if(((INV_PLOT[y0][xl][0]<LOSS_THRES&&INV_PLOT[y0][xr][0]>MAXX-1-LOSS_THRES) || ((xr-xl)>48 && (genAngle<2*PI/5 || genAngle>3*PI/5)) ) && curState==GoCross){
            xl=0;xr=MAXX; lptsN=rptsN=tptsN=0;
            rstInflectPoint(FALSE,FALSE,FALSE,0);
            rstInflectPoint(FALSE,FALSE,FALSE,1);
            return;
        }
    }
    // 迷宫法自适应寻边线
    findBorderLineL(xl+2,y0);
    findBorderLineR(xr-2,y0);
    // 三角滤波1
    triFiltering(lptsN,borderLPts[0]);
    triFiltering(lptsN,borderLPts[1]);
    triFiltering(rptsN,borderRPts[0]);
    triFiltering(rptsN,borderRPts[1]);
    // 边线等距采样
    resamplePoints(borderLPts[0],&lptsN);
    resamplePoints(borderRPts[0],&rptsN);
    // 边线获取转角
    getTurningAngles(borderLPts[0],borderLAng,lptsN);
    getTurningAngles(borderRPts[0],borderRAng,rptsN);
    // 检查角点并写入
    clearInflectPoint();
    findCorner(borderLAng,lptsN,0);
    findCorner(borderRAng,rptsN,1);
//    cameraProcessFlag=FALSE;
    if(curState==GoCrossAfter){
        if(lptsN<3&&rptsN<3) curState=GoCrossMid;
    }
    // 检查斑马线
    <代码挖空自己补>
    if(<代码挖空自己补>){
        curState=StopCar;
    }
}

void getTrackMethod(<参数也自己填吧>){
    // 这里你可能用到 getTrackLineViaPointAndAngle 函数或者 getTrackLine 函数，思考一下使用场景呗
    if(curState==GoCross&&(<代码挖空自己补>)){ // 大概什么严重挫折/qwq
        <代码挖空自己补> // 算一下 genAngle 吧
        if(<代码挖空自己补>)
            getTrackLineViaPointAndAngle(genAngle,0,0);
        else getTrackLine();
    }else if(curState==GoCircle&&(circleState==CircleRunPre)){
        if(circleDir==RightCircle)
            getTrackLineViaPointAndAngle(<代码挖空自己补>,0,0); // 参数自己定
        else getTrackLineViaPointAndAngle(<代码挖空自己补>,0,0); // 参数自己定
    }else if(curState==GoCircle&&(circleState==CircleOutPre||(circleState==CircleOutFound&&(
            (circleDir==RightCircle&&(VId[0]<10||(!lptVFound())))||
            (circleDir==LeftCircle&&(VId[1]<10||(!rptVFound())))
        )))){
        if(circleDir==RightCircle) getTrackLineViaPointAndAngle(参数自己定,0,0);
        else getTrackLineViaPointAndAngle(参数自己定,0,0);
    }else{
        getTrackLine();
    }
}
void getTrackLine(){
    UDPos num=(trackState==TrackLeft?lptsN:rptsN);
    FPos *borderPts=trackState==TrackLeft?borderLPts[0]:borderRPts[0]; tptsN=num;
    // 偏移边线到中线去吧
    <代码挖空自己补>
}
void getTrackLineViaPointAndAngle(FPos angle,FPos x0,FPos y0){ // angle 弧度输入， .__为0; ! 为 pi/2 ; __. 为pi
    if(x0<1e-10||y0<1e-10){ // x0,y0置0时，自动获取巡线起点
        if((trackState==TrackLeft||(lostStatus&2)!=0)&&(lostStatus&1)==0){
            x0=borderLPts[0][0]+TRACK_DIS;
            y0=borderLPts[1][0];
        }else if((lostStatus&2)==0){
            x0=borderRPts[0][0]-TRACK_DIS;
            y0=borderRPts[1][0];
        }else{
            x0=MAXX/2;
            y0=MAXY;
        }
    }
    tptsN=(MAXX+MAXY)/RESAMPLE_DIS;
    for(UDPos i=0;i<tptsN;i++){
        trackPts[0][i]=x0+i*cos(angle)*RESAMPLE_DIS;
        trackPts[1][i]=y0-i*sin(angle)*RESAMPLE_DIS;
    }
}
uint8* genOutput(){ // 调试的时候，可以用它生成一幅标记左右点的图像
//    while(cameraProcessFlag==TRUE);
//    cameraProcessFlag=TRUE;
    for(UPos y=0;y<MAXY;y++) for(UPos x=0;x<MAXX;x++){
        outImg[y][x]=getPixel(x,y);
    }
    for(int i=0;i<lptsN;i++){
        outImg[(int)clip(borderLPts[1][i]  ,0,MAXY-1)][(int)clip(borderLPts[0][i]  ,0,MAXX-1)]=255;
        outImg[(int)clip(borderLPts[1][i]-1,0,MAXY-1)][(int)clip(borderLPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(borderLPts[1][i]+1,0,MAXY-1)][(int)clip(borderLPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(borderLPts[1][i]  ,0,MAXY-1)][(int)clip(borderLPts[0][i]-1,0,MAXX-1)]=0;
        outImg[(int)clip(borderLPts[1][i]  ,0,MAXY-1)][(int)clip(borderLPts[0][i]+1,0,MAXX-1)]=0;
    }
    for(int i=0;i<rptsN;i++){
        outImg[(int)clip(borderRPts[1][i]  ,0,MAXY-1)][(int)clip(borderRPts[0][i]  ,0,MAXX-1)]=255;
        outImg[(int)clip(borderRPts[1][i]-1,0,MAXY-1)][(int)clip(borderRPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(borderRPts[1][i]+1,0,MAXY-1)][(int)clip(borderRPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(borderRPts[1][i]  ,0,MAXY-1)][(int)clip(borderRPts[0][i]-1,0,MAXX-1)]=0;
        outImg[(int)clip(borderRPts[1][i]  ,0,MAXY-1)][(int)clip(borderRPts[0][i]+1,0,MAXX-1)]=0;
    }
    for(int i=0;i<tptsN;i++){
        outImg[(int)clip(trackPts[1][i]  ,0,MAXY-1)][(int)clip(trackPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(trackPts[1][i]-1,0,MAXY-1)][(int)clip(trackPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(trackPts[1][i]+1,0,MAXY-1)][(int)clip(trackPts[0][i]  ,0,MAXX-1)]=0;
        outImg[(int)clip(trackPts[1][i]  ,0,MAXY-1)][(int)clip(trackPts[0][i]-1,0,MAXX-1)]=0;
        outImg[(int)clip(trackPts[1][i]  ,0,MAXY-1)][(int)clip(trackPts[0][i]+1,0,MAXX-1)]=0;
    }
    for(int i=0;i<lptsN;i++) printf("%lf ",borderLAng[i]*180/PI); printf("END\n");
    for(int i=0;i<rptsN;i++) printf("%lf ",borderRAng[i]*180/PI); printf("END\n");
    cameraProcessFlag=FALSE;
    return (uint8*)outImg;
}
uint8 lGoesOut(){   // 判断左侧线走着走着就出界了
    if((lostStatus&1)!=0) return TRUE;
    if(INV_PLOT[(int)borderLPts[1][lptsN-1]][(int)borderLPts[0][lptsN-1]][0]<3) return TRUE;
    if(borderLPts[0][lptsN-1]<=1.0*MAXX/4) return TRUE;
    return FALSE;
}
uint8 rGoesOut(){   // 同理
    <代码挖空自己补>
}
// 逆透视变换表，自己去测算
const UPos INV_PLOT[MT9V03X_H][MT9V03X_W][2]={
{{24,9},{24,9},...},
{{23,9},{24,9},...},
...
};
