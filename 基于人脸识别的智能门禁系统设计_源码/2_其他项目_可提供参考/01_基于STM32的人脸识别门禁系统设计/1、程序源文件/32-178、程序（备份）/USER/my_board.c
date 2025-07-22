/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 */
#include "my_include.h"

#ifndef  __RT_THREAD_H__
extern int $Super$$main(void);
int $Sub$$main(void)
{
#if !defined (USE_HAL_DRIVER)
    NVIC_Configuration();
    My_SysTick_Init();
#else
    HAL_Init();//��ʼ��HAL��
    Stm32_Clock_Init(RCC_PLL_MUL9);//����ʱ��,72M
#endif
    delay_init();//��ʱ������ʼ��
#if CODE_ENCRYPR>0
    MyCodeProtect();
#endif
    $Super$$main();
    return 0;
}
void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}
#else
// rtthread tick configuration
// 1. include header files
// 2. configure rtos tick and interrupt
// 3. add tick interrupt handler

// rtthread tick configuration
// 1. include some header file as need

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE        1*1024
/* ���ڲ�SRAM�������һ���־�̬�ڴ�����Ϊrtt�Ķѿռ䣬��������Ϊ4KB */
static uint32_t rt_heap[RT_HEAP_SIZE];
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#define HEAP_BEGIN          rt_heap_begin_get()
#define HEAP_END            rt_heap_end_get()
#endif

#define  DEBUG_USARTx     USART1
bool DEBUG_USARTx_Open = false;

/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{    
    // rtthread tick configuration
    // 2. Configure rtos tick and interrupt
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    delay_init();
    USARTx_Init(DEBUG_USARTx,115200);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init((void*)HEAP_BEGIN, (void*)HEAP_END);
#endif
}

// rtthread tick configuration
// 3. add tick interrupt handler 
  void SysTick_Handler(void)
 {
    /* enter interrupt */
    rt_interrupt_enter();
 
    rt_tick_increase();
 
    /* leave interrupt */
    rt_interrupt_leave();
 }
/**
 * This function will delay for some us.
 *
 * @param us the delay time of us
 */
void rt_hw_us_delay(rt_uint32_t us)
{
    u32 tCnt, tDelayCnt;
    u32 tStart;
    if(us==0)
    {
        return;
    }
    tStart = DWT->CYCCNT;/* �ս���ʱ�ļ�����ֵ */
    tCnt = 0;
    tDelayCnt = us * (SystemCoreClock / 1000000)-32;/* ��Ҫ�Ľ����� -32��ʵ��ֵ��ӽ�*/

    while(tCnt < tDelayCnt)
    {
        tCnt = DWT->CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */
    }
}
/**
  * @brief  ��ӳ�䴮��DEBUG_USARTx��rt_kprintf()����
  *   Note��DEBUG_USARTx����bsp_usart.h�ж���ĺ꣬Ĭ��ʹ�ô���1
  * @param  str��Ҫ��������ڵ��ַ���
  * @retval ��
  *
  * @attention
  *
  */
void rt_hw_console_output(const char *str)
{
    /* �����ٽ�� */
    rt_enter_critical();

    /* ֱ���ַ������� */
    while (DEBUG_USARTx_Open==true && *str!='\0')
    {
        /* ���� */
        if (*str=='\n')
        {
            USARTSendByte(DEBUG_USARTx, '\r');
        }

        USARTSendByte(DEBUG_USARTx, *str++);
    }

    /* �˳��ٽ�� */
    rt_exit_critical();
}
#endif
