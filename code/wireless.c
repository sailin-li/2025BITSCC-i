/*
 * wireless.c
 *
 *  Created on: 2024年11月10日
 *      Author: peril
 */
#include "wireless.h"

uint8 WlDataBuffer[32];
uint8 WlDataLen;
void initWireless(){
    while(1){
        if(wireless_uart_init())
            gpio_toggle_level(LED2);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        else break;
        system_delay_ms(100);                                                  // 闪灯表示异常
    }
    wireless_uart_send_byte('\r');
    wireless_uart_send_byte('\n');
    wireless_uart_send_string("wireless module start.\r\n");              // 初始化正常 输出测试信息
    gpio_set_level(LED2,0);                             // 关闭LED2提示摄像头初始化完成
}
