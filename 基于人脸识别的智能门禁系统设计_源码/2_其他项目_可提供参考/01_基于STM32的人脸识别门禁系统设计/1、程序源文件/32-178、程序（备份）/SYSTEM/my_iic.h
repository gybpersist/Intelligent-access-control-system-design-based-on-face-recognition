/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.2
//�޸�����:2020/12/30
//�����ܣ���װ�ͼ����ģ��IIC�Ĳ���
//V1.0 2020/06/06 ��ɻ�������
//V1.1 2020/10/22 1��ɾ�����ֺ�����ֻ�����Ļ�������
                  2���޸�IIC_Wait_Ack�����ķ���ֵ
                  3��IIC_SendAck����ʹ��ö������
//V1.2 2020/12/30 1�����¼������ݶ�д������ĳЩ������Ҫʹ�ã�
************************************************/
#ifndef __MY_IIC_H
#define __MY_IIC_H
#include "my_include.h"

typedef enum
{
    IIC_BIT_ACK = 0,
    IIC_BIT_NOACK = 1
} Bit_IICACK;

#define PIN_IIC_SCL     PC13
#define PIN_IIC_SDA     PC14

//IIC���в�������
void IIC_Init(void);//��ʼ��IIC��IO��
void IIC_Start(void);//����IIC��ʼ�ź�
void IIC_Stop(void);//����IICֹͣ�ź�
//bool IIC_Wait_Ack(void);
u8 IIC_Send_Byte(u8 sendDat);//IIC����һ���ֽ�
u8 IIC_Recv_Byte(Bit_IICACK ack);//IIC����һ���ֽ�
//д�Ĵ���
//SlaveAddress:������ַ
//REG_Address:�Ĵ�����ַ
//REG_data:Ҫд��Ĵ�����ֵ
//����ֵ:��
void IIC_WriteOneByte(u8 SlaveAddress, u8 REG_Address,u8 REG_data);
//*********************************************************
void IIC_WriteBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len);
void IIC_WriteString(u8 SlaveAddress, u8 REG_Address,char *buf);
//���Ĵ���
//SlaveAddress:������ַ
//REG_Address:�Ĵ�����ַ
//����ֵ:������ֵ
u8 IIC_ReadOneByte(u8 SlaveAddress, u8 REG_Address);
//*********************************************************
void IIC_ReadBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len);
#endif


