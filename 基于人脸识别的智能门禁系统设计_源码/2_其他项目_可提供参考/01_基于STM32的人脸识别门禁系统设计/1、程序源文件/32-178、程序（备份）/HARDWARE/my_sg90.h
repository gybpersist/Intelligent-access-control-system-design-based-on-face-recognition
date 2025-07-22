/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V2.0
//修改日期:2020/07/03
//程序功能：SG90舵机控制程序，此程序依赖于PWM程序
            PWM初始化为50Hz，输出占空比3~12.4为角度控制0~180
            默认占空比3%对应90度，12.4%对应-90度，7.7%对应0度
//V1.0  2020/06/05  完成基本功能
//V2.0  2020/07/03  修改为支持多个舵机，PWM周期默认为20毫秒
************************************************/
#ifndef __MY_SG90_H
#define __MY_SG90_H
#include "my_include.h"

#define MAX_NUM_SG90        4//舵机的最大数量

bool My_SG90_Init(u8 sg90Num,TIM_TypeDef* TIMx,TIM_Channel ch);
bool My_SG90_SetAngle(u8 sg90Num,float angle);
float My_SG90_GetAngle(u8 sg90Num);

#endif
