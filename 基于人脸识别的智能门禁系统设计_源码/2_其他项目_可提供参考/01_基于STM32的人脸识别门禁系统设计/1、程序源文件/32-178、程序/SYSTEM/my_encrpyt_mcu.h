/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2021/01/14
//程序功能：封装单片机程序加密的操作
//V1.0  2021/01/14  1、完成基本功能（从my_sysdef中复制出来）
************************************************/
#include "my_stmflash.h"
#include "my_sysdef.h"

typedef enum
{ 
    ENC_STM32F0=0,//0x1FFFF7AC
    ENC_STM32F3=0,//0x1FFFF7AC
    ENC_STM32F1=1,//0x1FFFF7E8
    ENC_STM32F2=2,//0x1FFF7A10
    ENC_STM32F4=2,//0x1FFFYA10
    ENC_STM32F7=3,//0x1FF0F420
    ENC_STM32H7=3,//0x1FF0F420
    ENC_STM32L0=4,//0x1FF80050
    ENC_STM32L1=4,//0x1FF80050
    ENC_STM32L4=5,//0x1FFF7590
}McuTypeDef_Encrpyt;
/*********************************************************
//函数功能：对每一种型号的STM32单片机进行程序加密
//type：单片机基本型号
//printLogo：是否打印版权logo，为true则打印
*********************************************************/
void MyCodeProtect(McuTypeDef_Encrpyt type,bool printLogo);
u16 My_Encrpyt_GetVersion(void);
