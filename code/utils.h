// 不明所以的一个模块，当时为了调试显示左右车道线用过的，可以去掉
#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#include "buzzer.h"
#include "camera.h"
#include "status.h"
#include "control.h"
#include "motor.h"
#include "zf_common_headfile.h"

void color(uint16 x,uint16 y,const uint16 color); // 给图像上某个点染色

#endif