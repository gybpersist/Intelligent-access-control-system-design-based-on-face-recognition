/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������-M3
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2020/10/18
//�����ܣ���װ�ͼ򻯵�·��Ĳ���
//V1.0  2020/10/18  ��ɻ�������
***********************************************/
#ifndef __MY_BOARD_H
#define __MY_BOARD_H

#include "stm32f10x.h"
#ifdef  USE_RT_THREAD
#include "rthw.h"
#include "rtthread.h"
#endif

#define CODE_ENCRYPR        0

#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
//#pragma GCC diagnostic push
//#pragma GCC diagnostic pop
#pragma GCC diagnostic ignored "-Winvalid-source-encoding"//���α�����󾯸棬Ҳ����misc controls�����-Wno-Winvalid-source-encoding
#else
//#pragma push
//#pragma pop
#pragma diag_suppress 177
#pragma diag_suppress 550//����δʹ�õı����ڱ���ʱ����������Ϣ
#endif

/*******************************************************************************************/

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum)           ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)                  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)          MEM_ADDR(BITBAND(addr, bitnum))
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr                  (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr                  (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr                  (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr                  (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr                  (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr                  (GPIOF_BASE+12) //0x40011C0C
#define GPIOG_ODR_Addr                  (GPIOG_BASE+12) //0x4001200C

#define GPIOA_IDR_Addr                  (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr                  (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr                  (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr                  (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr                  (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr                  (GPIOF_BASE+8) //0x40011C08 
#define GPIOG_IDR_Addr                  (GPIOG_BASE+8) //0x40012008 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)                        BIT_ADDR(GPIOA_ODR_Addr,n)  //���
#define PAin(n)                         BIT_ADDR(GPIOA_IDR_Addr,n)  //����

#define PBout(n)                        BIT_ADDR(GPIOB_ODR_Addr,n)  //���
#define PBin(n)                         BIT_ADDR(GPIOB_IDR_Addr,n)  //����

#define PCout(n)                        BIT_ADDR(GPIOC_ODR_Addr,n)  //���
#define PCin(n)                         BIT_ADDR(GPIOC_IDR_Addr,n)  //����

#define PDout(n)                        BIT_ADDR(GPIOD_ODR_Addr,n)  //���
#define PDin(n)                         BIT_ADDR(GPIOD_IDR_Addr,n)  //����

#define PEout(n)                        BIT_ADDR(GPIOE_ODR_Addr,n)  //���
#define PEin(n)                         BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)                        BIT_ADDR(GPIOF_ODR_Addr,n)  //���
#define PFin(n)                         BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)                        BIT_ADDR(GPIOG_ODR_Addr,n)  //���
#define PGin(n)                         BIT_ADDR(GPIOG_IDR_Addr,n)  //����

/************************����Pin�����������Ƶ�ƽ************************************/
#define LED_ON                          0
#define LED_OFF                         1
#define RLY_ON                          1
#define RLY_OFF                         0
#define BEEP_ON                         1
#define BEEP_OFF                        0
#define LOCK_ON                         1
#define LOCK_OFF                        0
#define PUMP_ON                         1
#define PUMP_OFF                        0
#define FAN_ON                          1
#define FAN_OFF                         0

void NVIC_Configuration(void);
#endif
