#ifndef __MY_IWDG_H
#define __MY_IWDG_H
#include "my_include.h"
#include "stm32f10x_iwdg.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//���Ź� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);

 
#endif
