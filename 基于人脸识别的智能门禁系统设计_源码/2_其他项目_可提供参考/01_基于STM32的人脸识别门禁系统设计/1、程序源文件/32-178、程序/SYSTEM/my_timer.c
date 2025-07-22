/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//YXDZ STM32开发板
//永鑫电子工作室
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/06/06
//程序功能：封装定时器操作
//V1.0 完成基本功能
//V1.1 精简部分功能
************************************************/
 #include "my_timer.h"

/***********************************************
函数功能：初始化定时器
TIMx：定时器选择，TIM1、TIM2、TIM3、TIM4
arr：自动重载值
psc：时钟分频系数
itEnable：是否使能中断，true为使能，false为不使能
************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc,bool itEnable)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    if(TIMx == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
    }
    else if(TIMx == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    }

    TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    if(itEnable)
    {
        TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);//使能TIM中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    }

    TIM_Cmd(TIMx, ENABLE);  //使能TIMx外设
}

void TIM3_IRQHandler(void)   //TIM3中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
    }
}

