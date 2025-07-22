/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V2.4
//修改日期:2020/11/26
//程序功能：封装和简化按键的操作,C文件无须做任何修改
//V1.0 完成基本功能
//V1.1 修改为可定义每个按键按下的状态
//V1.2 2019/11/03 删除在定时器中断调用的功能
//V1.3 2019/12/05 删除按键个数宏定义
//V1.4 2019/12/09 增加 KeyIsReleased 宏定义
//V1.5 2019/12/18 修复一个bug
                    将判断宏定义改为子程序调用
//V1.6 2019/12/25 增加长按状态的判断
//V1.7 2020/01/06 修正了部分按键状态判断的函数
                    IO初始化根据按下的状态进行初始化
                    增加了按键事件判断函数
//V1.8 2020/02/26 优化了按键状态判断程序
//V2.0 2020/04/08 增加了模拟按键按下的功能函数
                    增加了按键枚举类型
//V2.1 2020/06/28 将按键长按判断改为基于系统时钟的时间判断
//V2.2 2020/09/27 1、支持rt-thread发送按键消息队列
//V2.3 2020/11/13 1、My_Key_HasEvent添加事件类型判断
                  2、优化部分程序
//V2.4 2020/11/26 1、增加rt-thread消息队列初始化
************************************************/
#ifndef __MY_KEY_H
#define __MY_KEY_H
#include "my_include.h"

typedef enum
{
    KEY_1=0,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5
}My_KeyDef;

//按键定义
const static MyPinDef Pins_Key[] = {PB6,PB7,PB8,PB9,PA0};

#define KeyFlagType             u8
#define KEY_STATE_PRESS         0x00000000//按键按下时IO口的值，32个位对应32个按键的状态
#define TIME_LONG_PRESS         1000//按键长按判断时间（单位：毫秒）
#define LONG_PRESS              0//按键长按使能定义，为0时长按功能无效
#define USE_KEY_MESSAGE_QUEUE   //是否使用按键消息队列
#define LEN_MQ_KEY              10//按键消息队列最大长度（能存储的按键消息的个数）

#if defined (USE_RT_THREAD) && defined (USE_KEY_MESSAGE_QUEUE)
extern rt_mq_t mq_key;
#endif

void My_KEY_Init(void);//IO初始化
void My_KeyScan(void);
#if LONG_PRESS>0
bool KeyIsLongPress(My_KeyDef key);
bool KeyIsLongPressed(My_KeyDef key);
#endif
bool KeyIsPress(My_KeyDef key);
bool KeyIsPressed(My_KeyDef key);
bool KeyIsRelease(My_KeyDef key);
bool KeyIsReleased(My_KeyDef key);
bool My_Key_HasEvent(KeyEvent_Def event);
#if LONG_PRESS>0
void My_Key_PerformLongPress(My_KeyDef key);
void My_Key_PerformLongPressed(My_KeyDef key);
#endif
void My_Key_PerformPress(My_KeyDef key);
void My_Key_PerformPressed(My_KeyDef key);
void My_Key_PerformRelease(My_KeyDef key);
u16 My_Key_GetVersion(void);

#endif
