/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/12/07
//程序功能：封装和简化PWM的初始化等操作，适用于标准库
//V1.0  2020/06/05  1、完成基本功能
//V1.1  2020/12/07  1、增加TIM1和TIM8编译器选择警告
************************************************/ 
#include "my_pwm.h"

#define VERSION         11

u16 Pwm_Per[4];
/************************************************************
函数功能；PWM输出初始化
TIMx：PWM使用的定时器，如TIM2
ch：定时器输出通道，参考TIM_Channel定义
period：PWM输出周期（单位：微秒）
************************************************************/
#if defined (__CC_ARM) /* GNU Compiler */
#warning Please select complier version 6 if use TIM1 or TIM8 for PWM
#endif
void My_PWM_PreInit(TIM_TypeDef* TIMx,TIM_Channel ch, u16 period)
{
/* Set TIM3 and TIM4 for PWM mode */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    switch((u32)TIMx)
    {
        case (u32)TIM1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
            if(ch&TIM_CH_1)
            {
                GPIO_Pin_Init(PA8,GPIO_Mode_AF_PP);
            }
            if(ch&TIM_CH_2)GPIO_Pin_Init(PA9,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_3)GPIO_Pin_Init(PA10,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_4)GPIO_Pin_Init(PA11,GPIO_Mode_AF_PP);
            Pwm_Per[0] = period;
            break;
        case (u32)TIM2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            if(ch&TIM_CH_1)GPIO_Pin_Init(PA0,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_2)GPIO_Pin_Init(PA1,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_3)GPIO_Pin_Init(PA2,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_4)GPIO_Pin_Init(PA3,GPIO_Mode_AF_PP);
            Pwm_Per[1] = period;
            break;
        case (u32)TIM3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            if(ch&TIM_CH_1)GPIO_Pin_Init(PA6,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_2)GPIO_Pin_Init(PA7,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_3)GPIO_Pin_Init(PB0,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_4)GPIO_Pin_Init(PB1,GPIO_Mode_AF_PP);
            Pwm_Per[2] = period;
            break;
        case (u32)TIM4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            if(ch&TIM_CH_1)GPIO_Pin_Init(PB6,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_2)GPIO_Pin_Init(PB7,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_3)GPIO_Pin_Init(PB8,GPIO_Mode_AF_PP);
            if(ch&TIM_CH_4)GPIO_Pin_Init(PB9,GPIO_Mode_AF_PP);
            Pwm_Per[3] = period;
            break;
        default:break;
    }

    TIM_TimeBaseStructure.TIM_Period = period-1;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =71;//设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = period; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
    if(ch&TIM_CH_1)TIM_OC1Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    if(ch&TIM_CH_2)TIM_OC2Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    if(ch&TIM_CH_3)TIM_OC3Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    if(ch&TIM_CH_4)TIM_OC4Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

    if(ch&TIM_CH_1)TIM_SetCompare1(TIMx, 0);
    if(ch&TIM_CH_2)TIM_SetCompare2(TIMx, 0);
    if(ch&TIM_CH_3)TIM_SetCompare3(TIMx, 0);
    if(ch&TIM_CH_4)TIM_SetCompare4(TIMx, 0);

    TIM_CtrlPWMOutputs(TIMx,ENABLE);//MOE 主输出使能

    if(ch&TIM_CH_1)TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //CH1预装载使能
    if(ch&TIM_CH_2)TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //CH2预装载使能
    if(ch&TIM_CH_3)TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);  //CH3预装载使能
    if(ch&TIM_CH_4)TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);  //CH4预装载使能

    TIM_ARRPreloadConfig(TIMx, ENABLE); //使能TIMx在ARR上的预装载寄存器

    TIM_Cmd(TIMx, ENABLE);  //使能TIMx
}
void My_PWM_Init(TIM_TypeDef* TIMx,TIM_Channel ch, u16 period)
{
    delay_us(2000);
    My_PWM_PreInit( TIMx, ch, period);
}
/************************************************************
函数功能；PWM输出占空比的设置
TIMx：PWM使用的定时器，如TIM2
ch：定时器输出通道，参考TIM_Channel定义
percent：PWM输出占空比
************************************************************/
void My_PWM_SetDuty(TIM_TypeDef* TIMx,TIM_Channel ch,float percent)
{
    u16 per;
    if(percent>100){
        percent = 100;
    }
    switch((u32)TIMx)
    {
        case (u32)TIM1:per = Pwm_Per[0];break;
        case (u32)TIM2:per = Pwm_Per[1];break;
        case (u32)TIM3:per = Pwm_Per[2];break;
        case (u32)TIM4:per = Pwm_Per[3];break;
        default:break;
    }
    if(ch&TIM_CH_1)TIM_SetCompare1(TIMx, per*percent/100);
    if(ch&TIM_CH_2)TIM_SetCompare2(TIMx, per*percent/100);
    if(ch&TIM_CH_3)TIM_SetCompare3(TIMx, per*percent/100);
    if(ch&TIM_CH_4)TIM_SetCompare4(TIMx, per*percent/100);
}
u16 My_PWM_GetVersion(void)
{
    return VERSION;
}

