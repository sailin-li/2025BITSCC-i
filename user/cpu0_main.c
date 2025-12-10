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
#include "wireless.h"
#include "control.h"
#include "status.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// **************************** 代码区域 ****************************
#include "utils.h"

#define DEBUGGING_FLAG (FALSE)
#define SAMPLE_PERIOD (0.05) // 主定时器采样周期,单位毫秒
UTimeStamp clock = 0;        // 基于主定时器的时间戳

uint8 curAltMot = 0;
SpeedValue curSpeed = {0, 0}; // 当前速度，二元浮点对

int core0_main(void)
{
    // 这两行初始化在任何项目中请都保留
    clock_init(); // 获取时钟频率<务必保留>
    debug_init(); // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    initBuzzer();     // 初始化蜂鸣器（调试用）
    initBoard();      // 初始化板卡LED灯，按钮与开关、
    initEncoder();    // 初始化编码器（获取速度）
    initMotor(clock); // 初始化马达
    initServo(clock); // 初始化舵机
    // initWireless();                               // 初始化无线模块，通过LED2灯汇报进度，实际比赛时禁止使用！
    pit_us_init(CCU60_CH0, SAMPLE_PERIOD * 1000); // 使用中断初始化定时器 0.1s
#ifdef USE_IPS114_SCREEN
    ips114_set_dir(IPS114_PORTAIT);
    ips114_set_color(RGB565_WHITE, RGB565_BLACK);
    ips114_init(); // 初始化ips114屏幕
#endif
    cpu_wait_event_ready(); // 等待所有核心初始化完毕

    // 初始化部分结束，开始主循环
    while (TRUE)
    {
        // 本代码核 0 放硬件控制，核 1 放图像处理，所以这里是空的很正常
        // 你也可以在这里放针对特定情况改 kp,ki,kd 的代码，随你
        (void)0; // 空循环，可在此处放自定义周期性处理或调参逻辑
        system_delay_ms(1);
    }
}
// 中断，在此处理编码器
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0); // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);
    clock += SAMPLE_PERIOD; // 这里利用中断特性更新时间戳，你也可以用 system_getval 获取以 10ns 为单位的时间（不过请注意溢出问题）
    if (STOP_FLAG == FALSE)
    {
        EncoderReturnValue val = encoderRes();
        curSpeed.l = val.l / 1.024 / ENCODER_INT_WIDTH / SAMPLE_PERIOD;
        curSpeed.r = -(val.r / 1.024 / ENCODER_INT_WIDTH / SAMPLE_PERIOD);
        updateMotorL(curSpeed.l, clock);
        updateMotorR(curSpeed.r, clock);
        if (finishFlag == TRUE)
        {
            updateServo(curDir, clock);
            finishFlag = FALSE;
        }
    }
}
#pragma section all restore
// **************************** 代码区域 ****************************
