/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/06/06
//程序功能：封装和简化外部中断的初始化操作
************************************************/
#ifndef __MY_EXTI_H
#define __MY_EXTI_H
#include "my_include.h" 

void EXTIx_Init(MyPinDef pin, EXTITrigger_TypeDef trigger, u8 priority);//IO初始化

#endif

