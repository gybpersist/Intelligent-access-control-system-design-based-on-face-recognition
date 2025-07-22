/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.2
//修改日期:2020/10/19
//程序功能：封装和简化输出管脚的操作
//V1.0 2019/10/22 完成基本功能
//V1.1 2019/12/05 删除按键个数宏定义
//V1.2 2020/10/19 1、增加初始化后的默认输出电平
************************************************/
#ifndef __MY_LED_H
#define __MY_LED_H
#include "my_include.h"

#define buzzer  PinOut(Pins_LED[0])

#define PIN_STATE_DEF       0x00000000//定义每个管脚初始化后的默认输出电平，最低位代表Pins_LED[0]
const static MyPinDef Pins_LED[] = {PA5};

void My_LED_Init(void);//初始化
 
#endif
