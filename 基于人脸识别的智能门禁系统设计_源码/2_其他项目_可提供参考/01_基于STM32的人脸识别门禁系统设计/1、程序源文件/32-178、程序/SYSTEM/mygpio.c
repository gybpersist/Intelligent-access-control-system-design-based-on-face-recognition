/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.3
//修改日期:2020/11/26
//程序功能：封装和简化GPIO的读写操作
//V1.0 完成基本功能
//V1.1  2019/12/03  精简部分功能
//V1.2  2020/07/03  将乘除法运算改为移位运算，提高程序执行效率
//V1.3  2020/11/26  1、修复PC13、PC14初始化的bug
************************************************/
#include "mygpio.h"

#define VERSION     13

void GPIO_Pin_Init(MyPinDef pin,GPIOMode_TypeDef Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;//声明结构体
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA<<(pin>>4), ENABLE);//打开端口时钟 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
    if(pin==PB3 || pin==PB4 || pin==PA15)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开GPIO口时钟，先打开复用才能修改复用功能
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//要先开时钟，再重映射；这句表示关闭jtag，使能swd。
    }
    else if(pin==PA13 || pin==PA14)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开GPIO口时钟，先打开复用才能修改复用功能
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//要先开时钟，再重映射
    }
    else if(pin==PC13)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        PWR_BackupAccessCmd(ENABLE);/* 允许修改RTC和后备寄存器*/
        BKP_TamperPinCmd(DISABLE);  /* PC13可以用作普通IO*/
        PWR_BackupAccessCmd(DISABLE);/* 禁止修改RTC和后备寄存器*/
        //BKP_ITConfig(DISABLE);/* 禁止TAMPER 中断*/
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2M时钟速度
    }
    else if(pin==PD0 || pin==PD1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_PD01,ENABLE);
    }
    GPIO_InitStructure.GPIO_Pin  = GET_PIN_GPIO(pin); //管脚号
    GPIO_InitStructure.GPIO_Mode = Mode;//模式
    GPIO_Init(GET_PORT_GPIO(pin), &GPIO_InitStructure);//初始化端口
}
//BSRR  对寄存器高 16bit 写1 对应管脚为低电平，对寄存器低16bit写1对应管脚为高电平。写 0 ,无动作
//对GPIO高8位写入数据
void GPIO_WriteHigh(GPIO_TypeDef* GPIOx,u8 dat)
{
    GPIOx->BRR = 0xff00;
    GPIOx->BSRR = dat<<8;
}
//对GPIO低8位写入数据
void GPIO_WriteLow(GPIO_TypeDef* GPIOx,u8 dat)
{
    GPIOx->BRR = 0x00ff;
    GPIOx->BSRR = dat;
}
u16 My_GPIO_GetVersion(void)
{
    return VERSION;
}
