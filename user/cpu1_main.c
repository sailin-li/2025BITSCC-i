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
 * 文件名称          cpu1_main
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          ADS v1.9.20
 * 适用平台          TC264D
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期              作者                备注
 * 2022-09-15       pudding            first version
 * 2024-11          peri               adapt for competing use
 ********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中
#include "buzzer.h"
#include "camera.h"
#include "status.h"
#include "control.h"
#include "motor.h"
// **************************** 代码区域 ****************************

void core1_main(void)
{
    disable_Watchdog();         // 关闭看门狗
    interrupt_global_enable(0); // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等
    initBuzzer();
    initBoard();  // 初始化板卡LED灯，按钮与开关
    initCamera(); // 初始化摄像头，通过LED1灯汇报进度，因此必须放在initBoard()后

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready(); // 等待所有核心初始化完毕
    // 斑马线后开车，这时可以看到斑马线，不能刚开始就触发停车对吧
    // 初始化一些状态，避免程序启动时因为能看到斑马线而立刻进入 StopCar
    THRES = getThres();      // 初始化阈值
    BWCount = 0;             // 黑白跳变计数清零
    clearInflectPoint();     // 清除角点信息，默认认为为长直道
    curState = GoCrossAfter; // 认为刚通过斑马线之后开始行驶，避免启动时误判为需要立即停车
    finishFlag = TRUE;       // 标记处理完成，使 CPU0 能在首次周期更新舵机/其它动作
    uint8 COUNTER = 0;
    while (TRUE)
    {
        if (mt9v03x_finish_flag)
        { // 摄像头采集完成
            // 定时大津法获取阈值，也许你有更好的方法
            if ((++COUNTER) == 50)
                updateThres(), COUNTER = 0;
            // 图像处理主函数
            imageProcess(); // in camera.c
            // 更新控制状态
            updState(); // in control.c
            // 获取跟踪路径与预瞄点
            getTrackMethod(); // in camera.c
            // 更新控制值
            updCtrlVal(); // in control.c

            // === 在屏幕上显示标记左右边线的图像（调试用）===
            uint8 *out = genOutput();
            ips114_displayimage03x((const uint8 *)out, MAXX, MAXY);

            finishFlag = TRUE;
            mt9v03x_finish_flag = 0;
        }
    }
}
#pragma section all restore
// **************************** 代码区域 ****************************
