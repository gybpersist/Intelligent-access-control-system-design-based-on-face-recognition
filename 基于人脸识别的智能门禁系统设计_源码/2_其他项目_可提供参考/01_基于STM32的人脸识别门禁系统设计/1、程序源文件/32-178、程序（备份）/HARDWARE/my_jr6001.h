/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2020/11/17
//�����ܣ���װ�ͼ�JR6001����ģ��Ĳ���
//V1.0  2020/11/17  1����ɻ�������
************************************************/
/***********************************************
*++++++++++++++++++++++++++++++++++++++++++++++*
*ע�⣺��ģ��ָ���β�������ַ���������ָ��֮�����
        ����ʱ(�Ӿ�����Զ���)���ϵ��ʼ��Ҳ��Ҫ��ʱ
        ���⣬�����̱����Ż��ȼ�Ϊ0��ʵ��ģ������Ӧ
*++++++++++++++++++++++++++++++++++++++++++++++*
************************************************/
#ifndef __MY_JR6001_H
#define __MY_JR6001_H
#include "my_include.h"

#define UART_JR6001                 USART3
#define PIN_JR6001_BUSY             PB12

#define CMD_JR6001_PLAY             "A2"//����
#define CMD_JR6001_PAUSE            "A3"//��ͣ
#define CMD_JR6001_STOP             "A4"//ֹͣ
#define CMD_JR6001_PREV             "A5"//��һ��
#define CMD_JR6001_NEXT             "A6"//��һ��
#define CMD_JR6001_SELECT           "A7:"//ָ���ļ�������
#define CMD_JR6001_SELECT_FLASH     "A8:02"//ָ��FLASH��·�����ļ�������
#define CMD_JR6001_SELECT_TF        "A8:01"//ָ��TF����·�����ļ�������
#define CMD_JR6001_SELECT_UDISK     "A8:00"//ָ��U����·�����ļ�������
#define CMD_JR6001_VOLUME_SET       "AF:"//���������ȼ�
#define CMD_JR6001_VOLUME_INC       "B0"//������
#define CMD_JR6001_VOLUME_DEC       "B1"//������
#define CMD_JR6001_MODE_SET         "B4:"//���ò���ģʽ
#define CMD_JR6001_PLAY_GROUP       "B7:"//��ϲ��� ZH �ļ����ڵ��ļ�
#define CMD_JR6001_STOP_GROUP       "B8"//��ϲ��� ZH �ļ����ڵ��ļ�

typedef enum
{
    JR6001_CYCLE_SINGEL=1,//����ѭ��
    JR6001_ONCE_SINGEL,//��������һ��
    JR6001_RANDOM_ALL,//ȫ�����
    JR6001_CYCLE_DIR,//Ŀ¼��ѭ��
    JR6001_RANDOM_DIR,//Ŀ¼�����
    JsR6001_ORDER_ALL,//ȫ��˳�򲥷�
    JsR6001_ORDER_DIR//Ŀ¼��˳�򲥷�
}PlayMode_JR6001;//����ģʽ


#define JR6001_SEND_CMD(n)          My_USART_SendString(UART_JR6001,n)
#define My_JR6001_Play()            JR6001_SEND_CMD(CMD_JR6001_PLAY)
#define My_JR6001_Stop()            JR6001_SEND_CMD(CMD_JR6001_STOP)
#define My_JR6001_Pause()           JR6001_SEND_CMD(CMD_JR6001_PAUSE)
#define My_JR6001_Previous()        JR6001_SEND_CMD(CMD_JR6001_PREV)
#define My_JR6001_Next()            JR6001_SEND_CMD(CMD_JR6001_NEXT)
#define My_JR6001_VolumeUp()        JR6001_SEND_CMD(CMD_JR6001_VOLUME_INC)
#define My_JR6001_VolumeDown()      JR6001_SEND_CMD(CMD_JR6001_VOLUME_DEC)
#define My_JR6001_StopGroup()       JR6001_SEND_CMD(CMD_JR6001_STOP_GROUP)

void My_JR6001_Init(u8 volume);//��ʼ��
void My_JR6001_PlayByName(const char *name);
void My_JR6001_SetVolume(u8 level);
bool My_JR6001_IsBusy(void);
void My_JR6001_SetPlayMode(PlayMode_JR6001 mode);
void My_JR6001_PlayGroup(char *ptr);

//void My_JR6001_PlayGroup(int count, ...);

#endif
