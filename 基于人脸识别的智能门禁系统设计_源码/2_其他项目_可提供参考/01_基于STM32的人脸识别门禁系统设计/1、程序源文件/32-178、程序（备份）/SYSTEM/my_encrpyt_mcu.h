/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2021/01/14
//�����ܣ���װ��Ƭ��������ܵĲ���
//V1.0  2021/01/14  1����ɻ������ܣ���my_sysdef�и��Ƴ�����
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
//�������ܣ���ÿһ���ͺŵ�STM32��Ƭ�����г������
//type����Ƭ�������ͺ�
//printLogo���Ƿ��ӡ��Ȩlogo��Ϊtrue���ӡ
*********************************************************/
void MyCodeProtect(McuTypeDef_Encrpyt type,bool printLogo);
u16 My_Encrpyt_GetVersion(void);
