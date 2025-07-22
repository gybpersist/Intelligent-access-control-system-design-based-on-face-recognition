/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.2
//修改日期:2020/12/30
//程序功能：封装和简化软件模拟IIC的操作
//V1.0 2020/06/06 完成基本功能
//V1.1 2020/10/22 1、删除部分函数，只保留的基本功能
                  2、修改IIC_Wait_Ack函数的返回值
                  3、IIC_SendAck参数使用枚举类型
//V1.2 2020/12/30 1、重新加入数据读写函数（某些场合需要使用）
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

//IIC所有操作函数
void IIC_Init(void);//初始化IIC的IO口
void IIC_Start(void);//发送IIC开始信号
void IIC_Stop(void);//发送IIC停止信号
//bool IIC_Wait_Ack(void);
u8 IIC_Send_Byte(u8 sendDat);//IIC发送一个字节
u8 IIC_Recv_Byte(Bit_IICACK ack);//IIC接收一个字节
//写寄存器
//SlaveAddress:器件地址
//REG_Address:寄存器地址
//REG_data:要写入寄存器的值
//返回值:无
void IIC_WriteOneByte(u8 SlaveAddress, u8 REG_Address,u8 REG_data);
//*********************************************************
void IIC_WriteBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len);
void IIC_WriteString(u8 SlaveAddress, u8 REG_Address,char *buf);
//读寄存器
//SlaveAddress:器件地址
//REG_Address:寄存器地址
//返回值:读到的值
u8 IIC_ReadOneByte(u8 SlaveAddress, u8 REG_Address);
//*********************************************************
void IIC_ReadBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len);
#endif


