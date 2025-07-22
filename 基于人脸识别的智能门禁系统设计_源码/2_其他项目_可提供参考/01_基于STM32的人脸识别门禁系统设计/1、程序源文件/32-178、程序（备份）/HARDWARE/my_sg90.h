/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V2.0
//�޸�����:2020/07/03
//�����ܣ�SG90������Ƴ��򣬴˳���������PWM����
            PWM��ʼ��Ϊ50Hz�����ռ�ձ�3~12.4Ϊ�Ƕȿ���0~180
            Ĭ��ռ�ձ�3%��Ӧ90�ȣ�12.4%��Ӧ-90�ȣ�7.7%��Ӧ0��
//V1.0  2020/06/05  ��ɻ�������
//V2.0  2020/07/03  �޸�Ϊ֧�ֶ�������PWM����Ĭ��Ϊ20����
************************************************/
#ifndef __MY_SG90_H
#define __MY_SG90_H
#include "my_include.h"

#define MAX_NUM_SG90        4//������������

bool My_SG90_Init(u8 sg90Num,TIM_TypeDef* TIMx,TIM_Channel ch);
bool My_SG90_SetAngle(u8 sg90Num,float angle);
float My_SG90_GetAngle(u8 sg90Num);

#endif
