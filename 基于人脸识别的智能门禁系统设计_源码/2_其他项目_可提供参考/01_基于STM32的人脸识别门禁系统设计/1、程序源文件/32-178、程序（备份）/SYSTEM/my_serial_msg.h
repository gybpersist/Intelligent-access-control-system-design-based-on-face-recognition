/****************************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V3.2
//�޸�����:2020/09/20
//�����ܣ���װ�ͼ�ͨ�õĲ���
//�汾��V1.0   ��ɻ�������
//�汾��V2.0 2019/12/09 �޸���ʼ�ֽں�У��ͼ��㷽ʽ
//�汾��V2.1 2019/12/12 ���ӵ�ַ�ֽ�����ѡ��
//�汾��V2.2 2019/12/14 1�������Զ�У��ѡ��
                        2�������Զ��ظ�ѡ��
                        3���̶����ںţ��궨�壩
//�汾��V2.3 2019/12/17 �޸�һ��bug
//�汾��V3.0 2020/07/12 1���޸ĺ�������
                        2�����ӻ���������
                        3���޸���Ϣ�ص�����
                        4���޸�Ϊ��������Ϣ�����ֽ���
//�汾��V3.1 2020/09/08 1���������ݴ�������ղ���
//�汾��V3.2 2020/09/20 1�����ӽ���Ϣ����ת��Ϊ�ֽ�����ĺ���
                        2���޸����ܴ��ڵ�bug
****************************************************************/
#ifndef __SERIAL_MSG_H
#define __SERIAL_MSG_H
#include "my_include.h"

/************************************
�Զ���Э�飺
����      ��ʼ�ֽ�        ���ݳ���        ����      ��������        У���
��ʶ      smsb slsb       length          cmd       data  data      check
ʾ��      0x2a 0xd5       0x02            0x04      0x50 0x78       0x31
**************************************
Ϊ��ȷ����Ϣ����ʼ���������������ʼ�ֽںͽ����ֽڣ��ڳ����⵽��ʼ�ֽ�ʱ����Ϊ����Ϣ�Ŀ�ʼ��
�ڴ�֮ǰ���յ��������������������Ϣ�򽻸���������������ǲ������Ļ�������������������䶪����
Ϊ��ʹ��Ϣ�����ϴ󡢴���Ч�ʽϸߣ���Э��ʹ�����ݵľ�����ֵ��ʾ���ݴ�������ݣ���ʹ�������������ASCII�롣
������0x31��ʾ��������49��������ASCII���ַ�1��
�������������������У�0x00-0xff�Ƕ��п��ܳ��ֵģ��������ǽ���ʼ�ֽںͽ����ֽ�����Ϊ�̶���2���ֽڣ�
2���ֽڵĴ�����Ϊ0xff�������ͻ����Ͽ��Ա�������Ϣ�м������ʼ�ֽڻ�����ֽڶ�������Ϊ�����ݵĿ�ʼ���������������Ȼ�п��ܳ��֣������ʷǳ��ͣ���
��ʼ�ֽڣ�2���ֽڣ�0x2a��0xd5��0x2a��ASCII�ַ�Ϊ'*'��0x2a+0xd5=0xff
���ݳ��ȣ�1���ֽڣ���ʾ�������ݵĳ��ȣ��������ݵĳ����ǿɱ�ģ�
�����ֽڣ�1���ֽڣ���ʾ��ǰ��Ϣ����һ����Ϣ
�������ݣ�0~255���ֽڣ������ǿɱ�ģ����255���ֽ�
У��ͣ�1���ֽڣ�����У����յ�����Ϣ�Ƿ���ȷ�����㷽����
check = 0xff - (length+cmd+data[0]+data[1]+...+data[n])&0xff
���ϱ��У�0x31 = 0xff - (0x02+0x04+0x50+0x78)&0xff

***************************************/
#define NUM_MSG_SERIAL_MAX                      4//
#define LEN_MSG_SERIAL_MAX                      24//��Ϣ����,��ʹ��RTOS����ע���߳�ջ�ռ��С
#define MSG_START_MSB                           0x2a//���ڷ���������ʼ���ֽ�
#define MSG_START_LSB                           0xd5//���ڷ���������ʼ���ֽ�
#if LEN_MSG_SERIAL_MAX>255
#define LEN_SERIAL_NO_PAYLOAD                   6//��Ϣ�����г�payload��������ݳ���
#else
#define LEN_SERIAL_NO_PAYLOAD                   5//��Ϣ�����г�payload��������ݳ���
#endif
#define AUTO_CHECK                              0//�Ƿ�ʹ���Զ�У������
#define AUTO_RESPONSE                           0//�Ƿ�ʹ���Զ��ظ�
#define ENABLE_ADDR                             0//�Ƿ�ʹ�ܵ�ַѡ��
#define USE_BUF_SERIAL_MESSAGE                  0//�Ƿ�ʹ���Զ��崮�ڻ�����
#define USART_SERIAL_MESSAGE                    USART1//������Ϣ�Ĵ���
/*****************************************************************************************/
#if LEN_MSG_SERIAL_MAX>65535
#error  LEN_MSG_SERIAL_MAX value can not be bigger than 65535 !
#endif

#if USE_BUF_SERIAL_MESSAGE>0
#define My_SerialMsg_Receive                    My_SerialMsg_SaveToBuffer
#define MY_SERIAL_BUF_LEN                       128
#else
#define My_SerialMsg_Receive                    My_SerialMsg_StateMachine
#endif

typedef enum
{
    STATE_NONE=0x00,
    STATE_LENGTH_0,
    STATE_LENGTH_1,
#if ENABLE_ADDR>0
    STATE_ADDR_REC_1,
    STATE_ADDR_REC_2,
    STATE_ADDR_REC_3,
    STATE_ADDR_REC_4,
    STATE_ADDR_SEND_1,
    STATE_ADDR_SEND_2,
    STATE_ADDR_SEND_3,
    STATE_ADDR_SEND_4,
#endif
    STATE_CMD,
    STATE_DATA,
    STATE_CHECK,
}SerialMessageState;
/****************************************************************************************/
#define ADDR_MYSELF                     0x12345678
#define ADDR_BROADCAST                  0xFFFFFFFF
//
#define CMD_RESPONSE                    0x80//Ӧ��ָ�����յ������������󷵻ظ���Ϣ������
#define CMD_CAR_DEBUG                   0x00//�����ַ���
#define CMD_CAR_READY                   0x01//׼������
#define CMD_CAR_STATUS                  0x02//С��״̬
#define CMD_CAR_SENSOR                  0x04//С����������Ԥ����
#define CMD_CAR_BATTERY                 0x05//���״̬��Ԥ����
#define CMD_CAR_DIRECTION               0x06//С������

#define CMD_APP_DEBUG                   0x00//�����ַ���
#define CMD_APP_READY                   0x01//׼������
#define CMD_APP_GET_CAR_STATUS          0x02//��ȡС��״̬
#define CMD_APP_GET_CAR_SPEED           0x03//��ȡС���ٶ�
#define CMD_APP_GET_CAR_DIR             0x04//��ȡС������
#define CMD_APP_GET_CAR_SENSOR          0x05//��ȡС��������״̬��Ԥ����
#define CMD_APP_GET_CAR_BATTERY         0x06//��ȡС�����״̬��Ԥ����
#define CMD_APP_SET_CAR_STOP            0x10//ͣ��
#define CMD_APP_SET_CAR_START           0x11//����
#define CMD_APP_SET_CAR_STATUS          0x12//����С��״̬
#define CMD_APP_SET_CAR_SPEED           0x13//����С���ٶ�
#define CMD_APP_SET_CAR_DIR             0x14//����С������
#define CMD_APP_SET_CAR_SENSOR          0x15//����С��������״̬��Ԥ����

typedef struct
{
#if LEN_MSG_SERIAL_MAX>255
    u16 length;
#else
    u8 length;
#endif
#if ENABLE_ADDR>0
    union_u32 addr_rec;
    union_u32 addr_send;
#endif
    u8 cmd;
    u8 payload[LEN_MSG_SERIAL_MAX];
    u8 checkSum;
    bool enable;
}_serial_msg_obj;

#if USE_BUF_SERIAL_MESSAGE>0
void My_SerialMsg_SaveToBuffer(u8 dat);
#endif
void My_SerialMsg_Send(USART_TypeDef* USARTx, const _serial_msg_obj *msgPtr);
#if ENABLE_ADDR>0
void My_SerialMsg_SendTo(USART_TypeDef* USARTx, u32 addr_rec, _serial_msg_obj *msgPtr);
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u32 addr_rec, u8 cmd);
#else
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u8 cmd);
#endif
bool My_SerialMsg_IsCorrect(const _serial_msg_obj *msgPtr);
u8 My_SerialMsg_CalCheck(const _serial_msg_obj *msgPtr);
void My_SerialMsg_StateMachine(u8 msgByte);
void My_SerialMsg_Process(void);
//����ԭ����Ҫ�������Լ�����
void OnGetSerialMessage(const _serial_msg_obj *recMsgPtr);
u16 My_SerialMsg_ToBytes(const _serial_msg_obj *recMsgPtr,u8 *buff);
u16 My_SerialMsg_GetTotalLength(const _serial_msg_obj *recMsgPtr);
#endif
