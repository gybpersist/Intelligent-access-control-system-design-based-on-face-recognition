/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2020/06/06
//�����ܣ���װ�ͼ��ⲿ�жϵĳ�ʼ������
************************************************/
#include "my_exti.h"

//�ⲿ�жϳ�ʼ������
void EXTIx_Init(MyPinDef pin, EXTITrigger_TypeDef trigger, u8 priority)
{
    IRQn_Type irq;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
    GPIO_Pin_Init(pin,GPIO_Mode_IPU);

    //GPIOA.0     �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(pin>>4,pin&0x0f);

    EXTI_InitStructure.EXTI_Line=(EXTI_Line0<<(pin&0x0f));
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    switch(pin&0x0f)
    {
        case 0: irq = EXTI0_IRQn; break;
        case 1: irq = EXTI1_IRQn; break;
        case 2: irq = EXTI2_IRQn; break;
        case 3: irq = EXTI3_IRQn; break;
        case 4: irq = EXTI4_IRQn; break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            irq = EXTI9_5_IRQn; 
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            irq = EXTI15_10_IRQn; 
            break;
    }
    NVIC_InitStructure.NVIC_IRQChannel = irq;//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = priority;//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4);//���EXTI0��·����λ
        
    }
}
//void EXTI9_5_IRQHandler(void)
//{
//    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
//    {
//        EXTI_ClearITPendingBit(EXTI_Line8);    //���LINE8�ϵ��жϱ�־λ 
//    } 
//}
//void EXTI15_10_IRQHandler(void)
//{
//    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
//    {
//        EXTI_ClearITPendingBit(EXTI_Line10);    //���LINE5�ϵ��жϱ�־λ  
//    }
//}


