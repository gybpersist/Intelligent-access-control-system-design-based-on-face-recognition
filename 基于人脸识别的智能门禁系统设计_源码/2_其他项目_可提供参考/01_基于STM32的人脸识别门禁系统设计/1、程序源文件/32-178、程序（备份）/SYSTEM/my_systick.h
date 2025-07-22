/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//YXDZ STM32������
//���ε��ӹ�����
//���ߣ�tianqingyong
//�汾��V1.1
//�޸�����:2020/10/22
//�����ܣ�ϵͳʱ�ӵδ����
//V1.0 2020/06/07   ��ɻ�������
//V1.1 2020/10/22   1������HAL_Delay������֧��
************************************************/
#ifndef __MY_SYSTICK_H
#define __MY_SYSTICK_H
#include "my_include.h"

#if defined (__RT_THREAD_H__)
#define TICK_PER_SECOND             RT_TICK_PER_SECOND//1���ӵĵδ����
#define My_SysTick_GetPeriod()      ((float)1000/TICK_PER_SECOND)

#define My_SysTick_GetTicks()       rt_tick_get()
#define My_SysTick_GetCounter()     (SysTick->LOAD + SysTick->VAL +1)
#else
#define FLAG_READ_SYSTICK           1
#define FLAG_SEND_SYSTICK           1

#if !defined (USE_HAL_DRIVER)
#define TICK_PER_SECOND             1000//1���ӵĵδ����
#define My_SysTick_GetPeriod()      ((float)1000/TICK_PER_SECOND)
#else
#define TICK_PER_SECOND             (1000U / uwTickFreq)//1���ӵĵδ����
#define My_SysTick_GetPeriod()      uwTickFreq
#endif

void My_SysTick_Init(void);
u32 My_SysTick_GetTicks(void);
#endif

#define My_SysTick_GetCounter()     (SysTick->LOAD + SysTick->VAL +1)
#define My_SysTick_GetSeconds()     (My_SysTick_GetTicks()/TICK_PER_SECOND)
#define SysTickOfSeconds(n)         (n*TICK_PER_SECOND)//n���ڵ�tick�жϴ���
#define SysTickSecPassed(m,n)       ((My_SysTick_GetTicks()-m)>=SysTickOfSeconds(n))//n���ڵ�timer�жϴ���,mΪ���������Զ����TimerTicks,nΪҪ�жϵ�����
#define SysTickSecArrive(m,n)       ((My_SysTick_GetTicks()-m)==SysTickOfSeconds(n))//n���ڵ�timer�жϴ���,mΪ���������Զ����TimerTicks,nΪҪ�жϵ�����

#if FLAG_READ_SYSTICK>0
extern bool myReadFlag_tick;//��ȡ���ݱ�־
#endif
#if	FLAG_SEND_SYSTICK>0
extern bool mySendFlag_tick;//�������ݱ�־
#endif

u16 My_SysTick_GetVersion(void);
#endif
