/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V2.0
//修改日期:2020/07/03
//程序功能：SG90舵机控制程序，此程序依赖于PWM程序
            PWM初始化为50Hz，输出占空比3~12.4为角度控制0~180
            默认占空比3%对应90度，12.4%对应-90度，7.7%对应0度
//V1.0  2020/06/05  完成基本功能
//V2.0  2020/07/03  修改为支持多个舵机，PWM周期默认为20毫秒
************************************************/
#include "my_sg90.h"

static TIM_TypeDef* TIM_SG90[MAX_NUM_SG90];//输出控制舵机的PWM的定时器
static TIM_Channel channel[MAX_NUM_SG90];//使用的定时器通道（参考TIM_Channel定义）
static int16 sg90Angle[MAX_NUM_SG90];//舵机当前角度,实际值的100倍（以舵机臂在中间为0度，范围是-90~+90度）
/************************************************************
函数功能；SG90舵机初始化
sg90Num：舵机的编号
TIMx：PWM使用的定时器，如TIM2
ch：定时器输出通道，参考TIM_Channel定义
************************************************************/
bool My_SG90_Init(u8 sg90Num,TIM_TypeDef* TIMx,TIM_Channel ch)
{
    if(sg90Num<MAX_NUM_SG90)//在设定范围内
    {
        TIM_SG90[sg90Num] = TIMx;
        channel[sg90Num] = ch;
        sg90Angle[sg90Num] = 0;//初始角度为0
        My_PWM_Init(TIMx,ch,20000);//PWM周期默认为20毫秒
        return true;
    }
    return false;
}

/************************************************************
函数功能；获取舵机控制是否有效（有没有被初始化）
sg90Num：舵机的编号
************************************************************/
bool My_SG90_IsEnable(u8 sg90Num)
{
    if(TIM_SG90[sg90Num]==NULL || channel[sg90Num]==TIM_CH_NULL)
    {
        return false;
    }
    return true;
}
/************************************************************
函数功能；设置SG90舵机的角度
sg90Num：舵机的编号
angle：要设置的角度值（以舵机臂在中间为0度，范围是-90~+90度）
************************************************************/
bool My_SG90_SetAngle(u8 sg90Num,float angle)
{
    if(My_SG90_IsEnable(sg90Num)==false)
    {
        return false;
    }
    sg90Angle[sg90Num] = angle*100;
    if(angle>90)
    {
        angle = 90;
    }
    else if(angle<-90)
    {
        angle = -90;
    }
    angle += 90;
    My_PWM_SetDuty(TIM_SG90[sg90Num],channel[sg90Num],9.4-(angle/180)*9.4+3);
    return true;
}

/************************************************************
函数功能；获取SG90舵机当前的角度
sg90Num：舵机的编号
************************************************************/
float My_SG90_GetAngle(u8 sg90Num)
{
    return (float)sg90Angle[sg90Num]/100;
}

