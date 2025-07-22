/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//YXDZ STM32开发板
//永鑫电子工作室
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/10/22
//程序功能：系统时钟滴答计数
//V1.0 2020/06/07   完成基本功能
//V1.1 2020/10/22   1、增加HAL_Delay函数的支持
************************************************/
#ifndef __MY_SYSTICK_H
#define __MY_SYSTICK_H
#include "my_include.h"

#if defined (__RT_THREAD_H__)
#define TICK_PER_SECOND             RT_TICK_PER_SECOND//1秒钟的滴答次数
#define My_SysTick_GetPeriod()      ((float)1000/TICK_PER_SECOND)

#define My_SysTick_GetTicks()       rt_tick_get()
#define My_SysTick_GetCounter()     (SysTick->LOAD + SysTick->VAL +1)
#else
#define FLAG_READ_SYSTICK           1
#define FLAG_SEND_SYSTICK           1

#if !defined (USE_HAL_DRIVER)
#define TICK_PER_SECOND             1000//1秒钟的滴答次数
#define My_SysTick_GetPeriod()      ((float)1000/TICK_PER_SECOND)
#else
#define TICK_PER_SECOND             (1000U / uwTickFreq)//1秒钟的滴答次数
#define My_SysTick_GetPeriod()      uwTickFreq
#endif

void My_SysTick_Init(void);
u32 My_SysTick_GetTicks(void);
#endif

#define My_SysTick_GetCounter()     (SysTick->LOAD + SysTick->VAL +1)
#define My_SysTick_GetSeconds()     (My_SysTick_GetTicks()/TICK_PER_SECOND)
#define SysTickOfSeconds(n)         (n*TICK_PER_SECOND)//n秒内的tick中断次数
#define SysTickSecPassed(m,n)       ((My_SysTick_GetTicks()-m)>=SysTickOfSeconds(n))//n秒内的timer中断次数,m为主程序中自定义的TimerTicks,n为要判断的秒数
#define SysTickSecArrive(m,n)       ((My_SysTick_GetTicks()-m)==SysTickOfSeconds(n))//n秒内的timer中断次数,m为主程序中自定义的TimerTicks,n为要判断的秒数

#if FLAG_READ_SYSTICK>0
extern bool myReadFlag_tick;//读取数据标志
#endif
#if	FLAG_SEND_SYSTICK>0
extern bool mySendFlag_tick;//发送数据标志
#endif

u16 My_SysTick_GetVersion(void);
#endif
