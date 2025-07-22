/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/12/07
//程序功能：封装和简化PWM的初始化等操作，适用于标准库
//V1.0  2020/06/05  1、完成基本功能
//V1.1  2020/12/07  1、增加TIM1和TIM8编译器选择警告
************************************************/ 
#ifndef __MY_PWM_H
#define __MY_PWM_H

#include "my_include.h"

void My_PWM_Init(TIM_TypeDef* TIMx,TIM_Channel ch, u16 period);
void My_PWM_SetDuty(TIM_TypeDef* TIMx,TIM_Channel channel,float percent);
u16 My_PWM_GetVersion(void);

#endif
