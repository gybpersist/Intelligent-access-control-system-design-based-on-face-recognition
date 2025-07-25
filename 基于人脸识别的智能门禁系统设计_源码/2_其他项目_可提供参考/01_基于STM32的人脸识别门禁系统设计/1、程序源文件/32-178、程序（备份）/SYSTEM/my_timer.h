/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//YXDZ STM32开发板
//永鑫电子工作室
//作者：tianqingyong
//版本：V1.2
//修改日期:2020/12/14
//程序功能：封装定时器操作，适用于标准库
//V1.0 完成基本功能
//V1.1  2020/06/06  1、精简部分功能
//V1.2  2020/12/14  1、更新初始化函数，可选择要不要开启中断
************************************************/
 #ifndef __MY_TIMER_H
#define __MY_TIMER_H
#include "my_include.h"

/***********************************************
函数功能：初始化定时器
TIMx：定时器选择，TIM1、TIM2、TIM3、TIM4
arr：自动重载值
psc：时钟分频系数
itEnable：是否使能中断，true为使能，false为不使能
************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc,bool itEnable); 
 
#endif
