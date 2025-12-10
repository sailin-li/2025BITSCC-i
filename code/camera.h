/*
 * camera.h
 * camera and imageProcessing
 *
 *  Created on: 2024年11月5日
 *      Author: peril
 * Function:
 *   本头文件提供了摄像头操作以及图像处理。
 */

#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_
#include "zf_common_headfile.h"
#include "board.h"
#include "buzzer.h"
#include "status.h"

#define UPos uint8   // 坐标值
#define SPos int16   // 处理坐标计算中的溢出
#define FPos float   // 浮点型坐标值
#define FAng float   // 浮点型角度值
#define UDPos uint16 // UPos*UPos，坐标数量
#define UPix uint8   // 灰度值类型

#define _SquareDiff float      // 方差时使用
#define _PixelIntergral uint32 // UDPOS*UPIX
#define _PixelSumInSquare uint16

#define MAXX MT9V03X_W
#define MAXY MT9V03X_H
#define BEGIN_X (20)            // 扫描起始坐标（略大于 0 的数）
#define BEGIN_Y (110)           // 近处水平线纵坐标
#define BEGIN_FAR_Y (60)        // 远处水平线纵坐标
#define HALF_KERNEL (7)         // 自适应阈值半矩形大小 参考值：7
#define TRI_HALF_KERNEL (12)    // 三角滤波半卷积核大小，参考值：12
#define TP_HALF_KERNEL (2)      // 角点参考点范围一半，参考值：2
#define CLIP_VALUE (2)          // 自适应阈值clip参数
#define RESAMPLE_DIS (3.0f)     // 重采样间距，参考：3.0
#define ANG_KERNAL (6)          // 角度获取半卷积核，参考：6
#define LOSS_THRES (3)          // 认为丢线时，左右边点距边距离？？？？
#define TRACK_KERNEL (6)        // 寻找当前点法相值前后参考点范围
#define TRACK_DIS (22)          // 路宽一半（像素）
#define LOW_SAMPLE_ACTIVATE (6) // 某侧可用点数少于此时，激活缺少可用数据点判断方法
// V 对应锐角，L 对应直角，J 对应曲线
#define ANGV_THRES (20.0f * PI / 180.0f)                // 锐角角点最小角度阈值（弧度）
#define ANGL_MIN_THRES (40.0f * PI / 180.0f)            // 直角角点最小角度阈值
#define ANGL_MIN_THRES_LOW_SAMPLE (30.0f * PI / 180.0f) // 当采样点较少时，适当减少阈值
#define ANGL_MAX_THRES (120.0f * PI / 180.0f)           // 直角角点最大角度阈值
#define CURVE_THRES (0.15f)                             // 曲率阈值（经验值）
#define BW_JUMP_THRES (18)                              // 斑马线黑白跳变数量阈值
#define BW_Y (55)                                       // 检查斑马线纵坐标
#define BW_Y2 (70)                                      // 检查斑马线纵坐标2

#define KERNEL_SIZE (((HALF_KERNEL) * 2 + 1) * ((HALF_KERNEL) * 2 + 1))
#define XMAX MAXX // 防止写错，可以与MAXX混用，不推荐使用（
#define YMAX MAXY // 防止写错，可以与MAXY混用，不推荐使用（

// 枚举类型，当前巡左线还是巡右线
typedef enum
{
    TrackLeft,
    TrackRight
} TrackState;

// 全局变量
extern UPix THRES;                                                  // 全局大阈值
extern const UPos INV_PLOT[MT9V03X_H][MT9V03X_W][2];                // 反投影变换打表
extern UPix outImg[MAXY][MAXX];                                     // 输出图像
extern FPos trackPts[2][MAXX + MAXY];                               // 左目标点与右目标点
extern TrackState trackState;                                       // 当前巡线状态
extern UDPos tptsN, lptsN, rptsN;                                   // 目标巡线点数，左右线点数
extern FPos borderLPts[2][MAXX + MAXY], borderRPts[2][MAXX + MAXY]; // 左线上点坐标与右线上点坐标
extern FAng borderLAng[MAXX + MAXY], borderRAng[MAXX + MAXY];       // 左线各处曲率（角度）与右线各处曲率（角度）
extern FAng genAngle;                                               // 经过十字，若无法获取远方边线，设置固定偏差角度
extern uint8 cameraProcessFlag;                                     // 摄像头数据是否处理完成，放置屏幕或串口显示与图像读取冲突

// functions
void initCamera(void);                                              // 初始化摄像头模块
UPix getPixelOrigin(UPos x, UPos y);                                // 获取原图中横坐标为 x ,纵坐标为 y 点处的像素值
UPix getPixel(UPos x, UPos y);                                      // 获取逆透视变换后图像中横坐标为 x ,纵坐标为 y 点处的像素值
UPix getThres(void);                                                // otsu 法获取图像阈值
UPix updateThres(void);                                             // 更新图像阈值
void findBorderLineL(UPos x, UPos y);                               // 寻找左边线
void findBorderLineR(UPos x, UPos y);                               // 寻找右边线
void triFiltering(UDPos ptsN, FPos *borderPts);                     // 三角滤波边线
void resamplePoints(FPos *borderPts, UDPos *num);                   // 重采样边线点
void getTurningAngles(FPos *borderPts, FAng *borderAng, UDPos num); // 获取边线各处曲率（角度）
inline void findCorner(FAng *angs, UDPos len, uint8 dir);           // 寻找边线上拐点
void imageProcess(void);                                            // 图像处理主函数
void getTrackLine(void);                                            // 获取要巡的目标线（左/右边线偏移）
void getTrackLineViaPointAndAngle(FPos angle, FPos x0, FPos y0);    // 通过坐标与角度直接固定要巡的目标线
void getTrackMethod(void);                                          // 根据需要选择合适的获取目标线的方法
uint8 *genOutput(void);                                             // 产生绘制了左右边线关键点的图像，返回图像
uint8 *genBinaryWithMarks(void);                                    // 产生二值化图像并标记左右边线关键点
uint8 lGoesOut(void);                                               // 左边线是否向左侧超出屏幕
uint8 rGoesOut(void);                                               // 右边线是否向右侧超出屏幕

#endif /* CODE_CAMERA_H_ */
