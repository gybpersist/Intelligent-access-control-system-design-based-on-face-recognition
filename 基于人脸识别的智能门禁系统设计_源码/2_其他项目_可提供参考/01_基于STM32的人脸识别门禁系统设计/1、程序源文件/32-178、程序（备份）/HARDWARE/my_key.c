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
#include "my_key.h"

#define VERSION     24

static KeyFlagType keyFlag_state = 0;
static KeyFlagType keyFlag_event = 0;
#if LONG_PRESS>0
static u16 keyPressTime[ArrayCount(Pins_Key)];
#endif
#if defined (USE_RT_THREAD) && defined (USE_KEY_MESSAGE_QUEUE)
rt_mq_t mq_key = RT_NULL;
#endif
/**********************************************
按键初始化，根据Pins_Key数组自动进行初始化
***********************************************/
void My_KEY_Init(void) //IO初始化
{
    u8 index;
    for(index=0;index<ArrayCount(Pins_Key);index++)
    {
        if(KEY_STATE_PRESS&(0x01<<index))
        {
#if !defined (USE_HAL_DRIVER)
            GPIO_Pin_Init(Pins_Key[index],GPIO_Mode_IPD);
#else
            GPIO_Pin_Init(Pins_Key[index],GPIO_MODE_INPUT,GPIO_PULLDOWN);
#endif
        }
        else
        {
#if !defined (USE_HAL_DRIVER)
            GPIO_Pin_Init(Pins_Key[index],GPIO_Mode_IPU);
#else
            GPIO_Pin_Init(Pins_Key[index],GPIO_MODE_INPUT,GPIO_PULLUP);
#endif
        }
    }
#if defined (USE_RT_THREAD) && defined (USE_KEY_MESSAGE_QUEUE)
    mq_key = rt_mq_create("mq_key",/* 消息队列名字 */
                            sizeof(My_KeyEvent), /* 消息的最大长度 */
                            LEN_MQ_KEY, /* 消息队列的最大容量 */
                            RT_IPC_FLAG_FIFO);/* 队列模式 FIFO(0x00)*/
#endif
}
/****************************************************
读取所有按键的状态，KeyFlagType根据按键个数自动调整
****************************************************/
KeyFlagType My_KeyReadState(void)
{
    KeyFlagType key_state=0;
    u8 i;
    //读取所有定义的按键状态
    for(i=0;i<ArrayCount(Pins_Key);i++)
    {
        if(PinRead(Pins_Key[i])==((KEY_STATE_PRESS>>i)&0x01))
        {
            key_state |= (0x01<<i);
        }
    }
    return key_state;
}
/****************************************************
扫描所有按键并标记按键的状态和事件
*****************************************************/
void My_KeyScan(void)
{
    KeyFlagType key_state=0;
    key_state = My_KeyReadState();
    if(key_state!=keyFlag_state)//如果有按键状态发生变化就延时重新读取
    {
        delay_ms(8);//消除抖动
        key_state = My_KeyReadState();
    }

    keyFlag_event = keyFlag_state^key_state;
#if LONG_PRESS>0
    {
        u8 i;
        for(i=0;i<ArrayCount(Pins_Key);i++)
        {
            if(keyFlag_event&(0x01<<i))//如果按键状态与上一次的状态不一样
            {
                keyPressTime[i] = My_SysTick_GetTicks() & 0x7fff;//只要按键状态发生变化就更新时间戳
            }
            else if(key_state&(0x01<<i))//如果按键一直是按下的
            {
                if(keyPressTime[i]<0x8000)//最高位用于标记长按状态
                {
                    if(((My_SysTick_GetTicks()-keyPressTime[i])&0x7fff)*My_SysTick_GetPeriod()>=TIME_LONG_PRESS)
                    {
                        keyPressTime[i] |= 0x8000;
                        keyFlag_event |= (0x01<<i);
                    }
                }
                else
                {
                    keyFlag_event &= ~(0x01<<i);
                }
            }
        }
    }
#endif
    keyFlag_state = key_state;
#if defined (USE_RT_THREAD) && defined (USE_KEY_MESSAGE_QUEUE)
    {
        u8 i;
        for(i=0;i<ArrayCount(Pins_Key);i++)
        {
            if(keyFlag_event&(0x01<<i))//如果按键状态与上一次的状态不一样
            {
                My_KeyEvent event;
                event.eventTime = My_SysTick_GetTicks();
                event.keyType = KEY_TYPE_SINGLE;
                event.keyCode = i;
                event.keyEvent = (key_state&(0x01<<i))?KEY_EVENT_PRESS:KEY_EVENT_RELEASE;
    #if LONG_PRESS>0
                if(keyPressTime[i] & 0x8000)
                {
                    event.keyEvent = KEY_EVENT_LONG_PRESS;
                }
    #endif
                rt_mq_send(mq_key, &event, sizeof(My_KeyEvent));
            }
        }
    }
#endif
}

#if LONG_PRESS>0
bool KeyIsLongPress(My_KeyDef key)
{
    return (bool)((keyFlag_event&(0x01<<key))&&(keyFlag_state&(0x01<<key))&&(keyPressTime[key]>0x8000));
}
bool KeyIsLongPressed(My_KeyDef key)
{
    return (bool)(!(keyFlag_event&(0x01<<key))&&(keyFlag_state&(0x01<<key))&&(keyPressTime[key]>0x8000));
}
bool KeyIsPress(My_KeyDef key)
{
    return (bool)((keyFlag_event&(0x01<<key))&&(keyFlag_state&(0x01<<key))&&(keyPressTime[key]<0x8000));
}
#else
bool KeyIsPress(My_KeyDef key)
{
    return (bool)((keyFlag_event&(0x01<<key))&&(keyFlag_state&(0x01<<key)));
}
#endif
bool KeyIsPressed(My_KeyDef key)
{
    return (bool)((!(keyFlag_event&(0x01<<key)))&&(keyFlag_state&(0x01<<key)));
}
bool KeyIsRelease(My_KeyDef key)
{
    return (bool)((keyFlag_event&(0x01<<key))&&!(keyFlag_state&(0x01<<key)));
}
bool KeyIsReleased(My_KeyDef key)
{
    return (bool)((!(keyFlag_event&(0x01<<key)))&&(!(keyFlag_state&(0x01<<key))));
}

bool My_Key_HasEvent(KeyEvent_Def event)
{
    KeyFlagType keyPressFlag = keyFlag_event&keyFlag_state;
    
    if((event&KEY_EVENT_RELEASE) && (keyFlag_event&(~keyFlag_state)))
    {
        return true;
    }
#if LONG_PRESS>0
    if(event&KEY_EVENT_PRESS)
    {
        u8 i;
        for(i=0;i<ArrayCount(Pins_Key);i++)
        {
            if((keyPressFlag&(1<<i)) && keyPressTime[i]<0x8000)
            {
                return true;
            }
        }
    }
    if(event&KEY_EVENT_LONG_PRESS)
    {
        u8 i;
        for(i=0;i<ArrayCount(Pins_Key);i++)
        {
            if((keyPressFlag&(1<<i)) && keyPressTime[i]>0x8000)
            {
                return true;
            }
        }
    }
#else
    if((event&KEY_EVENT_PRESS) && keyPressFlag)
    {
        return true;
    }
#endif
    return false;
}

#if LONG_PRESS>0
void My_Key_PerformLongPress(My_KeyDef key)
{
    keyFlag_event|=(0x01<<key);
    keyFlag_state|=(0x01<<key);
    keyPressTime[key] |= 0x8000;
}
void My_Key_PerformLongPressed(My_KeyDef key)
{
    keyFlag_event&=~(0x01<<key);
    keyFlag_state|=(0x01<<key);
    keyPressTime[key] |= 0x8000;
}
void My_Key_PerformPress(My_KeyDef key)
{
    keyFlag_event|=(0x01<<key);
    keyFlag_state|=(0x01<<key);
    keyPressTime[key]=0;
}
void My_Key_PerformPressed(My_KeyDef key)
{
    keyFlag_event&=~(0x01<<key);
    keyFlag_state|=(0x01<<key);
    keyPressTime[key]=0;
}
#else
void My_Key_PerformPress(My_KeyDef key)
{
    keyFlag_event|=(0x01<<key);
    keyFlag_state|=(0x01<<key);
}
void My_Key_PerformPressed(My_KeyDef key)
{
    keyFlag_event&=~(0x01<<key);
    keyFlag_state|=(0x01<<key);
}
#endif
void My_Key_PerformRelease(My_KeyDef key)
{
    keyFlag_event|=(0x01<<key);
    keyFlag_state&=~(0x01<<key);
}
u16 My_Key_GetVersion(void)
{
    return VERSION;
}
