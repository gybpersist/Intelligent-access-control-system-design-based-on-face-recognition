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

//��������
const static MyPinDef Pins_Key[] = {PB6,PB7,PB8,PB9,PA0};

#define KeyFlagType             u8
#define KEY_STATE_PRESS         0x00000000//��������ʱIO�ڵ�ֵ��32��λ��Ӧ32��������״̬
#define TIME_LONG_PRESS         1000//���������ж�ʱ�䣨��λ�����룩
#define LONG_PRESS              0//��������ʹ�ܶ��壬Ϊ0ʱ����������Ч
#define USE_KEY_MESSAGE_QUEUE   //�Ƿ�ʹ�ð�����Ϣ����
#define LEN_MQ_KEY              10//������Ϣ������󳤶ȣ��ܴ洢�İ�����Ϣ�ĸ�����

#if defined (USE_RT_THREAD) && defined (USE_KEY_MESSAGE_QUEUE)
extern rt_mq_t mq_key;
#endif

void My_KEY_Init(void);//IO��ʼ��
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
