/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/06/06
//程序功能：封装和简化外部中断的初始化操作
************************************************/
#include "my_exti.h"

//外部中断初始化函数
void EXTIx_Init(MyPinDef pin, EXTITrigger_TypeDef trigger, u8 priority)
{
    IRQn_Type irq;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
    GPIO_Pin_Init(pin,GPIO_Mode_IPU);

    //GPIOA.0     中断线以及中断初始化配置
    GPIO_EXTILineConfig(pin>>4,pin&0x0f);

    EXTI_InitStructure.EXTI_Line=(EXTI_Line0<<(pin&0x0f));
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

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
    NVIC_InitStructure.NVIC_IRQChannel = irq;//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = priority;//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4);//清除EXTI0线路挂起位
        
    }
}
//void EXTI9_5_IRQHandler(void)
//{
//    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
//    {
//        EXTI_ClearITPendingBit(EXTI_Line8);    //清除LINE8上的中断标志位 
//    } 
//}
//void EXTI15_10_IRQHandler(void)
//{
//    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
//    {
//        EXTI_ClearITPendingBit(EXTI_Line10);    //清除LINE5上的中断标志位  
//    }
//}


