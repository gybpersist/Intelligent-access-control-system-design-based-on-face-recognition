/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V2.4
//�޸�����:2020/11/26
//�����ܣ���װ�ͼ򻯰����Ĳ���,C�ļ��������κ��޸�
//V1.0 ��ɻ�������
//V1.1 �޸�Ϊ�ɶ���ÿ���������µ�״̬
//V1.2 2019/11/03 ɾ���ڶ�ʱ���жϵ��õĹ���
//V1.3 2019/12/05 ɾ�����������궨��
//V1.4 2019/12/09 ���� KeyIsReleased �궨��
//V1.5 2019/12/18 �޸�һ��bug
                    ���жϺ궨���Ϊ�ӳ������
//V1.6 2019/12/25 ���ӳ���״̬���ж�
//V1.7 2020/01/06 �����˲��ְ���״̬�жϵĺ���
                    IO��ʼ�����ݰ��µ�״̬���г�ʼ��
                    �����˰����¼��жϺ���
//V1.8 2020/02/26 �Ż��˰���״̬�жϳ���
//V2.0 2020/04/08 ������ģ�ⰴ�����µĹ��ܺ���
                    �����˰���ö������
//V2.1 2020/06/28 �����������жϸ�Ϊ����ϵͳʱ�ӵ�ʱ���ж�
//V2.2 2020/09/27 1��֧��rt-thread���Ͱ�����Ϣ����
//V2.3 2020/11/13 1��My_Key_HasEvent����¼������ж�
                  2���Ż����ֳ���
//V2.4 2020/11/26 1������rt-thread��Ϣ���г�ʼ��
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
������ʼ��������Pins_Key�����Զ����г�ʼ��
***********************************************/
void My_KEY_Init(void) //IO��ʼ��
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
    mq_key = rt_mq_create("mq_key",/* ��Ϣ�������� */
                            sizeof(My_KeyEvent), /* ��Ϣ����󳤶� */
                            LEN_MQ_KEY, /* ��Ϣ���е�������� */
                            RT_IPC_FLAG_FIFO);/* ����ģʽ FIFO(0x00)*/
#endif
}
/****************************************************
��ȡ���а�����״̬��KeyFlagType���ݰ��������Զ�����
****************************************************/
KeyFlagType My_KeyReadState(void)
{
    KeyFlagType key_state=0;
    u8 i;
    //��ȡ���ж���İ���״̬
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
ɨ�����а�������ǰ�����״̬���¼�
*****************************************************/
void My_KeyScan(void)
{
    KeyFlagType key_state=0;
    key_state = My_KeyReadState();
    if(key_state!=keyFlag_state)//����а���״̬�����仯����ʱ���¶�ȡ
    {
        delay_ms(8);//��������
        key_state = My_KeyReadState();
    }

    keyFlag_event = keyFlag_state^key_state;
#if LONG_PRESS>0
    {
        u8 i;
        for(i=0;i<ArrayCount(Pins_Key);i++)
        {
            if(keyFlag_event&(0x01<<i))//�������״̬����һ�ε�״̬��һ��
            {
                keyPressTime[i] = My_SysTick_GetTicks() & 0x7fff;//ֻҪ����״̬�����仯�͸���ʱ���
            }
            else if(key_state&(0x01<<i))//�������һֱ�ǰ��µ�
            {
                if(keyPressTime[i]<0x8000)//���λ���ڱ�ǳ���״̬
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
            if(keyFlag_event&(0x01<<i))//�������״̬����һ�ε�״̬��һ��
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
