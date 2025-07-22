/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/06/06
//程序功能：使用DWT做精准延时
//V1.0 完成基本功能
************************************************/
#ifndef __DELAY_H
#define __DELAY_H
#include "my_include.h" 

#define delay_init  DWT_Init

void DWT_Init(void);
void delay_us(u32 usCount);
#if !defined(USE_RT_THREAD)
void delay_ms(u16 msCount);
#endif
void delay_sec(u8 secCount);

#endif





























