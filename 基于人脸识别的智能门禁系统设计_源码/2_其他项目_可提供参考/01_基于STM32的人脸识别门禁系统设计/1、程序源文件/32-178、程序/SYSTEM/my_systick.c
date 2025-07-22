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
#include "my_systick.h"

#define VERSION     11

static u32 sysTicks=0;//时间片

void My_SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / TICK_PER_SECOND);
}
u32 My_SysTick_GetTicks(void)
{
    return sysTicks;
}

#if FLAG_SEND_SYSTICK>0
bool mySendFlag_tick = false;//发送数据标志
#endif
#if FLAG_READ_SYSTICK>0
bool myReadFlag_tick = false;//读取数据标志
#endif
///////////////////////////////////////////
#if !defined (USE_HAL_DRIVER)
void SysTick_Handler(void)   //SysTick中断
{
#else
uint32_t HAL_GetTick(void)
{
  return sysTicks*uwTickFreq;
}
void HAL_IncTick(void)//SysTick中断回调函数
{
    uwTick += (uint32_t)uwTickFreq;//在stm32f1xx_hal.c中的HAL_IncTick若定义函数中
#endif
    sysTicks++;
#if FLAG_READ_SYSTICK>0
    if(sysTicks%((int)(20/My_SysTick_GetPeriod()))==0)
    {
        myReadFlag_tick = true;
    }
#endif
#if FLAG_SEND_SYSTICK>0
    if(sysTicks%((int)(500/My_SysTick_GetPeriod()))==1)
    {
        mySendFlag_tick = true;
    }
#endif
}
u16 My_SysTick_GetVersion(void)
{
    return VERSION;
}
