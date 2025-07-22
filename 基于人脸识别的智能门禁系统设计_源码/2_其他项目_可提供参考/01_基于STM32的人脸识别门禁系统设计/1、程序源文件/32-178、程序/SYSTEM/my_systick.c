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
#include "my_systick.h"

#define VERSION     11

static u32 sysTicks=0;//ʱ��Ƭ

void My_SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / TICK_PER_SECOND);
}
u32 My_SysTick_GetTicks(void)
{
    return sysTicks;
}

#if FLAG_SEND_SYSTICK>0
bool mySendFlag_tick = false;//�������ݱ�־
#endif
#if FLAG_READ_SYSTICK>0
bool myReadFlag_tick = false;//��ȡ���ݱ�־
#endif
///////////////////////////////////////////
#if !defined (USE_HAL_DRIVER)
void SysTick_Handler(void)   //SysTick�ж�
{
#else
uint32_t HAL_GetTick(void)
{
  return sysTicks*uwTickFreq;
}
void HAL_IncTick(void)//SysTick�жϻص�����
{
    uwTick += (uint32_t)uwTickFreq;//��stm32f1xx_hal.c�е�HAL_IncTick�����庯����
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
