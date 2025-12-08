/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.4
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
* 2024-11          peri               adapt for competing usage
********************************************************************************************************************/

/* 这是控制cpu0的代码，在此我赋予cpu0的使命是：根据当前状态，控制马达和舵机响应来自上位机的信号，与上位机进行信息的交互
 * 马达和舵机的控制将通过中断完成
 ****************************************************************/

#define USE_IPS114_SCREEN

#include "zf_common_headfile.h"
#include "buzzer.h"
#include "encoder.h"
#include "servo.h"
#include "board.h"
#include "motor.h"
#include "camera.h"
#include "zf_device_ips114.h"
#include "wireless.h"
#include "control.h"
#include "status.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// **************************** 代码区域 ****************************
#include "utils.h"

#define DEBUGGING_FLAG (FALSE)
#define SAMPLE_PERIOD (0.05)        // 主定时器采样周期,单位毫秒
UTimeStamp clock=0;                 // 基于主定时器的时间戳

uint8 curAltMot=0;
SpeedValue curSpeed={0,0};          // 当前速度，二元浮点对

// helper: convert 8-bit RGB to RGB565
static inline uint16_t rgb565_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

// helper: convert 8-bit gray to RGB565
static inline uint16_t rgb565_from_gray(uint8_t g)
{
    return rgb565_from_rgb(g, g, g);
}

// clip helper (macro used in camera code)
#ifndef clip
#define clip(x, minn, maxn) ((x) < (minn) ? (minn) : ((x) > (maxn) ? (maxn) : (x)))
#endif

int core0_main(void){
    // 这两行初始化在任何项目中请都保留
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    initBuzzer();                   // 初始化蜂鸣器（调试用）
    initBoard();                    // 初始化板卡LED灯，按钮与开关、
    initEncoder();                  // 初始化编码器（获取速度）
    initMotor(clock);               // 初始化马达
    initServo(clock);               // 初始化舵机
//    initWireless();                 // 初始化无线模块，通过LED2灯汇报进度，实际比赛时禁止使用！
    pit_us_init(CCU60_CH0, SAMPLE_PERIOD*1000); // 使用中断初始化定时器 0.1s
#ifdef USE_IPS114_SCREEN
    ips114_set_dir(IPS114_PORTAIT);
    ips114_set_color(RGB565_WHITE, RGB565_BLACK);
    ips114_init();                  // 初始化ips114屏幕
#endif
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

    // 初始化屏幕与摄像头

    ips114_show_string(0, 0, "camera init.");

    // 初始化摄像头驱动（底层驱动可能会在内部进行重试）
    while (1)
    {
        if (mt9v03x_init())
            ips114_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);
    }
    ips114_show_string(0, 16, "init success.");

    // 初始化摄像头的高级处理模块
    initCamera();



    // 初始化部分结束，开始主循环（加入图像显示）
    while(TRUE){
        if (mt9v03x_finish_flag)
        {
            uint16_t dis_w = ips114_width_max;
            uint16_t half_h = ips114_height_max / 2;

#if DISPLAY_MODE == 0
            // 原图全屏显示
            ips114_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, dis_w, ips114_height_max, 0);
            mt9v03x_finish_flag = 0;

#elif DISPLAY_MODE == 1
            // 仅显示处理后全图
            imageProcess(); // 图像处理/二值化/巡线
            uint8_t *proc = genOutput();
            static uint16_t rgbBuf[MT9V03X_W * MT9V03X_H];
            for (int y = 0; y < MT9V03X_H; y++)
            {
                for (int x = 0; x < MT9V03X_W; x++)
                {
                    uint8_t g = proc[y * MT9V03X_W + x];
                    rgbBuf[y * MT9V03X_W + x] = rgb565_from_gray(g);
                }
            }
            // 标记左右边界与轨迹
            for (int i = 0; i < (int)lptsN; i++)
            {
                int px = (int)clip((int)borderLPts[0][i], 0, MT9V03X_W - 1);
                int py = (int)clip((int)borderLPts[1][i], 0, MT9V03X_H - 1);
                rgbBuf[py * MT9V03X_W + px] = rgb565_from_rgb(0, 255, 0);
            }
            for (int i = 0; i < (int)rptsN; i++)
            {
                int px = (int)clip((int)borderRPts[0][i], 0, MT9V03X_W - 1);
                int py = (int)clip((int)borderRPts[1][i], 0, MT9V03X_H - 1);
                rgbBuf[py * MT9V03X_W + px] = rgb565_from_rgb(0, 0, 255);
            }
            for (int i = 0; i < (int)tptsN; i++)
            {
                int px = (int)clip((int)trackPts[0][i], 0, MT9V03X_W - 1);
                int py = (int)clip((int)trackPts[1][i], 0, MT9V03X_H - 1);
                rgbBuf[py * MT9V03X_W + px] = rgb565_from_rgb(255, 0, 0);
            }
            ips114_show_rgb565_image(0, 0, (const uint16 *)rgbBuf, MT9V03X_W, MT9V03X_H, dis_w, ips114_height_max, 1);
            mt9v03x_finish_flag = 0;

#else
            // 默认：上半屏显示原图，下半屏显示处理结果（带标记）
            ips114_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, dis_w, half_h, 0);

            imageProcess(); // 图像处理/二值化/巡线
            uint8_t *proc = genOutput();
            static uint16_t rgbBuf[MT9V03X_W * MT9V03X_H];
            for (int y = 0; y < MT9V03X_H; y++)
            {
                for (int x = 0; x < MT9V03X_W; x++)
                {
                    uint8_t g = proc[y * MT9V03X_W + x];
                    rgbBuf[y * MT9V03X_W + x] = rgb565_from_gray(g);
                }
            }
            for (int i = 0; i < (int)lptsN; i++)
            {
                int px = (int)clip((int)borderLPts[0][i], 0, MT9V03X_W - 1);
                int py = (int)clip((int)borderLPts[1][i], 0, MT9V03X_H - 1);
                rgbBuf[py * MT9V03X_W + px] = rgb565_from_rgb(0, 255, 0);
            }
            for (int i = 0; i < (int)rptsN; i++)
            {
                int px = (int)clip((int)borderRPts[0][i], 0, MT9V03X_W - 1);
                int py = (int)clip((int)borderRPts[1][i], 0, MT9V03X_H - 1);
                rgbBuf[py * MT9V03X_W + px] = rgb565_from_rgb(0, 0, 255);
            }
            for (int i = 0; i < (int)tptsN; i++)
            {
                int px = (int)clip((int)trackPts[0][i], 0, MT9V03X_W - 1);
                int py = (int)clip((int)trackPts[1][i], 0, MT9V03X_H - 1);
                rgbBuf[py * MT9V03X_W + px] = rgb565_from_rgb(255, 0, 0);
            }
            ips114_show_rgb565_image(0, half_h, (const uint16 *)rgbBuf, MT9V03X_W, MT9V03X_H, dis_w, half_h, 1);

            mt9v03x_finish_flag = 0;
#endif
        }

        // 其余控制逻辑继续由中断处理，主循环可保持短延时
        system_delay_ms(1);
    }
}
// 中断，在此处理编码器
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY){
    interrupt_global_enable(0);                      // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);
    clock+=SAMPLE_PERIOD;                            // 这里利用中断特性更新时间戳，你也可以用 system_getval 获取以 10ns 为单位的时间（不过请注意溢出问题）
    if(STOP_FLAG==FALSE){
        EncoderReturnValue val=encoderRes();
        curSpeed.l =  val.l/1.024/ENCODER_INT_WIDTH/SAMPLE_PERIOD ;
        curSpeed.r =-(val.r/1.024/ENCODER_INT_WIDTH/SAMPLE_PERIOD);
        updateMotorL(curSpeed.l,clock);
        updateMotorR(curSpeed.r,clock);
        if(finishFlag==TRUE){
            updateServo(curDir,clock);
            finishFlag=FALSE;
        }
    }
}
#pragma section all restore
// **************************** 代码区域 ****************************
