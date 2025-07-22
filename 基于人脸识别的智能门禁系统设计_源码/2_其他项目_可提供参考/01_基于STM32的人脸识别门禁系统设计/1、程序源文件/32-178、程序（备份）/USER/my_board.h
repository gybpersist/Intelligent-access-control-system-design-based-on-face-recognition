/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板-M3
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/10/18
//程序功能：封装和简化电路板的操作
//V1.0  2020/10/18  完成基本功能
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
#pragma GCC diagnostic ignored "-Winvalid-source-encoding"//屏蔽编码错误警告，也可在misc controls中添加-Wno-Winvalid-source-encoding
#else
//#pragma push
//#pragma pop
#pragma diag_suppress 177
#pragma diag_suppress 550//避免未使用的变量在编译时产生警告信息
#endif

/*******************************************************************************************/

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum)           ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)                  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)          MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)                        BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PAin(n)                         BIT_ADDR(GPIOA_IDR_Addr,n)  //输入

#define PBout(n)                        BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
#define PBin(n)                         BIT_ADDR(GPIOB_IDR_Addr,n)  //输入

#define PCout(n)                        BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
#define PCin(n)                         BIT_ADDR(GPIOC_IDR_Addr,n)  //输入

#define PDout(n)                        BIT_ADDR(GPIOD_ODR_Addr,n)  //输出
#define PDin(n)                         BIT_ADDR(GPIOD_IDR_Addr,n)  //输入

#define PEout(n)                        BIT_ADDR(GPIOE_ODR_Addr,n)  //输出
#define PEin(n)                         BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)                        BIT_ADDR(GPIOF_ODR_Addr,n)  //输出
#define PFin(n)                         BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)                        BIT_ADDR(GPIOG_ODR_Addr,n)  //输出
#define PGin(n)                         BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/************************定义Pin控制器件控制电平************************************/
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
