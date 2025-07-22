/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.3
//修改日期:2020/11/26
//程序功能：封装和简化GPIO的读写操作
//V1.0 完成基本功能
//V1.1  2019/12/03  精简部分功能
//V1.2  2020/07/03  将乘除法运算改为移位运算，提高程序执行效率
//V1.3  2020/11/26  1、修复PC13、PC14初始化的bug
************************************************/
#ifndef __MYGPIO_H
#define __MYGPIO_H

typedef enum
{ 
PA0=0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15, 
  PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15, 
  PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15, 
  PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15, 
  PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15, 
  PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15
}MyPinDef;

#include "my_include.h"

#ifndef BITBAND
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#endif
#ifndef MEM_ADDR
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#endif
#ifndef BIT_ADDR
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#endif 
#ifndef GPIOA_ODR_Addr
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#endif 
#ifndef GPIOA_IDR_Addr
#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#endif 

#define GET_PORT_GPIO(n)        (GPIO_TypeDef *)(GPIOA_BASE+0x0400UL*((n)>>4))
#define GET_PIN_GPIO(n)         (GPIO_Pin_0<<((n)&0x0f))
//IO快速操作(STM32F103在72M时约82ns)，使用灵活度较低
#define ReadPin(m,n)            P##m##in(n)
#define WritePin(m,n)           P##m##out(n)
#define SetPin(m,n)             WritePin(m,n)=1
#define ResetPin(m,n)           WritePin(m,n)=0
#define TogglePin(m,n)          WritePin(m,n)=!WritePin(m,n)
//IO操作速度较慢（STM32F103在72M时约0.85us,表达是中有数据运算），使用比较灵活
#define PinRead(n)              BIT_ADDR(GPIOA_IDR_Addr+0x400*((n)>>4),((n)&0x0f))
#define PinOut(n)               BIT_ADDR(GPIOA_ODR_Addr+0x400*((n)>>4),((n)&0x0f))
#define PinWrite                PinOut
#define PinSet(n)               PinOut(n)=1
#define PinReset(n)             PinOut(n)=0
#define PinToggle(n)            PinOut(n)=!Pin_Out(n)

void GPIO_Pin_Init(MyPinDef pin,GPIOMode_TypeDef Mode);
void GPIO_WriteHigh(GPIO_TypeDef* GPIOx,u8 dat);
void GPIO_WriteLow(GPIO_TypeDef* GPIOx,u8 dat);
u16 My_GPIO_GetVersion(void);

#endif
