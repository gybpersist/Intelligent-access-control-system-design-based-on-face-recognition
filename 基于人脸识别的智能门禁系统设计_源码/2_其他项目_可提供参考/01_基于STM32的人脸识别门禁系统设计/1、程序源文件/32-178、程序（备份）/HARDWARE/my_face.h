/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ 51������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2020/09/08
//�����ܣ���ȡ�Իس����н����Ĵ�������
//�汾��2020/09/08     V1.0    ��ɻ�������
***********************************************/
#ifndef __MY_FACE_MSG_H
#define __MY_FACE_MSG_H
#include "my_include.h"


#define FACE_USART_MESSAGE            USART3


#define COUNT_MSG_FACE_MAX                      4//
#define LEN_MSG_FACE_MAX                        32//��Ϣ����
/****************************************************************************************/

#define My_FaceData_Receive                  My_FaceData_StateMachine

typedef struct 
{
    bool enable;
    u16 length;
    u8 payload[LEN_MSG_FACE_MAX];
}_FaceData_msg_obj;

uint16_t xbx_crc_calculate(uint8_t *addr ,uint8_t datalen);


void My_FaceData_StateMachine(u8 msgByte);
void My_FaceData_Process(void);

void sendInputFaceCmd(void);//¼������ �޳�ʱ
void shiBieFaceCmd(void); //ʶ������ �޳�ʱ
void deleteAllFaceCmd(void);//ɾ������	
void deleteFaceIdCmd(u16 midId ); //ɾ��ĳID������Ϣ
void cancelCurFaceCmd(void);//ȡ����ǰ����

u16 getFaceId(void);//��ȡ��ǰ����ID
u16 getFaceCmdkRes(void);//�Ƿ���յ��˷������� ���Ҳ������ʧ�ܵ� ���ݾ������

#endif
