/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V4.1
//�޸�����:2021/01/14
//�����ܣ���װ�ͼ�ͨ�õĲ���
//V2.0  2020/04/08  ����ϵͳ�����λ����
//V2.1  2020/07/03  ���ӳ���ID���ܹ���
//V3.2  2020/07/16  1���޸��ַ���ָ������Ϊconst�����������뾯��
                    2���޸�GetIntegerFromString����������ʽ���޸���bug
//V3.3  2020/09/21   1�����Ӷ���ַ����жϵ�״̬������
                    2���������ַ���״̬�����������޸�
//V3.4  2020/09/27  1��ͬʱ֧�������rt-thread
//V3.5  2020/10/18  1�����벿�ִ��뵽my_board��
//V3.6  2020/11/13  1�����Ӷ����Ʋ�������ɸ���ʵ����Ҫ�޸�
//V3.7  2020/12/11  1�����¼��ܳ���
//V3.8  2020/12/18  1���Ż��ַ���ʶ��״̬��
//V3.9  2020/12/30  1������SWO��ITM�����Դ�ӡ������DAPLink��ʱ��֧�֣�
//V4.0  2021/01/10  1����AlarmModeö�ٱ�����ΪAlertMode��Alarm����Ϊ���ӣ�
//V4.1  2021/01/14  1����������ܴ���ɾ��
************************************************/
#ifndef __MY_SYSDEF_H
#define __MY_SYSDEF_H

#include "my_board.h"
#include <string.h> 
#include <stdio.h> 
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

#define DEBUG_PRINTF        1//�Ƿ�ʹ�ܵ��Դ�ӡ����

#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
////////////////////////////////////////////
#ifndef __weak
    #define __weak          __attribute__((weak))
#endif /* __weak */
////////////////////////////////////////////
#ifndef __packed
    #define __packed        __attribute__((__packed__))
#endif /* __packed */
#ifndef _packed
    #define _packed         __packed
#endif /* __packed */
////////////////////////////////////////////
#define Addr_at(n)      __attribute__((section(".ARM.__at_"#n"")))
#define Section_at(n)   __attribute__((section(""#n"")))
#ifndef ALIGN
    #define ALIGN(n)                    __attribute__((aligned(n)))
#endif
#else
    #define Addr_at(n)      __attribute__((at(n)))
#ifndef __packed
    #define __packed        _packed
#endif /* __packed */
#ifndef ALIGN
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
#endif
#endif

#ifndef byte
typedef signed char  byte;
#endif
#ifndef int8
typedef signed char  int8;
#endif
#ifndef int16
typedef signed short int int16;
#endif
#ifndef int32
typedef signed long int int32;
#endif
#ifndef int64
typedef signed long long int64;
#endif
#ifndef int_8
typedef signed char  int_8;
#endif
#ifndef int_16
typedef signed short int int_16;
#endif
#ifndef int_32
typedef signed long int int_32;
#endif
#ifndef int_64
typedef signed long long int_64;
#endif
//��stm32f10x.h���д����
#if !defined __STM32F10x_H
#ifndef u8
typedef unsigned char  u8;
#endif
#endif
#ifndef uint8
typedef unsigned char  uint8;
#endif
#ifndef uint_8
typedef unsigned char  uint_8;
#endif
#ifndef uint8_t
typedef unsigned char  uint8_t;
#endif
//��stm32f10x.h���д����
#if !defined __STM32F10x_H
#ifndef u16
typedef unsigned short int  u16;
#endif
#endif
#ifndef uint16
typedef unsigned short int  uint16;
#endif
#ifndef uint_16
typedef unsigned short int  uint_16;
#endif
#ifndef uint16_t
typedef unsigned short int  uint16_t;
#endif
//��stm32f10x.h���д����
#if !defined __STM32F10x_H
#ifndef u32
typedef unsigned int  u32;
#endif
#endif
#ifndef uint32
typedef unsigned int  uint32;
#endif
#ifndef uint_32
typedef unsigned int  uint_32;
#endif
//��stm32f10x.h���д����
#if !defined __STM32F10x_H
#ifndef uint32_t
typedef unsigned int  uint32_t;
#endif
#endif
#ifndef u64
typedef unsigned long long  u64;
#endif
#ifndef uint64
typedef unsigned long long  uint64;
#endif
#ifndef uint_64
typedef unsigned long long  uint_64;
#endif
#ifndef uint64_t
typedef unsigned long long  uint64_t;
#endif

#if !defined (__STM32F10x_H)
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */
#endif
/********************************************************************/
//�Զ��������壬�������ݲ�����byte_0Ĭ��Ϊ����ֽڣ����С��ģʽ�йأ�
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
}Bits_8;
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
    u8 bit_8:1;
    u8 bit_9:1;
    u8 bit_10:1;
    u8 bit_11:1;
    u8 bit_12:1;
    u8 bit_13:1;
    u8 bit_14:1;
    u8 bit_15:1;
}Bits_16;
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
    u8 bit_8:1;
    u8 bit_9:1;
    u8 bit_10:1;
    u8 bit_11:1;
    u8 bit_12:1;
    u8 bit_13:1;
    u8 bit_14:1;
    u8 bit_15:1;
    u8 bit_16:1;
    u8 bit_17:1;
    u8 bit_18:1;
    u8 bit_19:1;
    u8 bit_20:1;
    u8 bit_21:1;
    u8 bit_22:1;
    u8 bit_23:1;
    u8 bit_24:1;
    u8 bit_25:1;
    u8 bit_26:1;
    u8 bit_27:1;
    u8 bit_28:1;
    u8 bit_29:1;
    u8 bit_30:1;
    u8 bit_31:1;
}Bits_32;
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
    u8 bit_8:1;
    u8 bit_9:1;
    u8 bit_10:1;
    u8 bit_11:1;
    u8 bit_12:1;
    u8 bit_13:1;
    u8 bit_14:1;
    u8 bit_15:1;
    u8 bit_16:1;
    u8 bit_17:1;
    u8 bit_18:1;
    u8 bit_19:1;
    u8 bit_20:1;
    u8 bit_21:1;
    u8 bit_22:1;
    u8 bit_23:1;
    u8 bit_24:1;
    u8 bit_25:1;
    u8 bit_26:1;
    u8 bit_27:1;
    u8 bit_28:1;
    u8 bit_29:1;
    u8 bit_30:1;
    u8 bit_31:1;
    u8 bit_32:1;
    u8 bit_33:1;
    u8 bit_34:1;
    u8 bit_35:1;
    u8 bit_36:1;
    u8 bit_37:1;
    u8 bit_38:1;
    u8 bit_39:1;
    u8 bit_40:1;
    u8 bit_41:1;
    u8 bit_42:1;
    u8 bit_43:1;
    u8 bit_44:1;
    u8 bit_45:1;
    u8 bit_46:1;
    u8 bit_47:1;
    u8 bit_48:1;
    u8 bit_49:1;
    u8 bit_50:1;
    u8 bit_51:1;
    u8 bit_52:1;
    u8 bit_53:1;
    u8 bit_54:1;
    u8 bit_55:1;
    u8 bit_56:1;
    u8 bit_57:1;
    u8 bit_58:1;
    u8 bit_59:1;
    u8 bit_60:1;
    u8 bit_61:1;
    u8 bit_62:1;
    u8 bit_63:1;
}Bits_64;
/****************************************************************/
typedef union{
    Bits_8 Bits;
    u8 value;
}union_u8;
typedef union{
    Bits_16 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
    }Bytes;
    u16 value;
}union_u16;
typedef union{
    Bits_16 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
    }Bytes;
    int16 value;
}union_int16;
typedef union{
    Bits_32 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
    }Uint16;
    u32 value;
}union_u32;
typedef union{
    Bits_32 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
    }Uint16;
    int32 value;
}union_int32;
typedef union{
    Bits_64 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
        u8 byte_4:8;
        u8 byte_5:8;
        u8 byte_6:8;
        u8 byte_7:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
        u16 uint16_2:16;
        u16 uint16_3:16;
    }Uint16;
    struct
    {
        u32 uint32_0:32;
        u32 uint32_1:32;
    }Uint32;
    u64  value;
}union_u64;
typedef union{
    Bits_64 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
        u8 byte_4:8;
        u8 byte_5:8;
        u8 byte_6:8;
        u8 byte_7:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
        u16 uint16_2:16;
        u16 uint16_3:16;
    }Uint16;
    struct
    {
        u32 uint32_0:32;
        u32 uint32_1:32;
    }Uint32;
    int64  value;
}union_int64;
typedef union{
    Bits_32 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
    }Uint16;
    float value;
}union_float;
typedef union{
    Bits_64 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
        u8 byte_4:8;
        u8 byte_5:8;
        u8 byte_6:8;
        u8 byte_7:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
        u16 uint16_2:16;
        u16 uint16_3:16;
    }Uint16;
    struct
    {
        u32 uint32_0:32;
        u32 uint32_1:32;
    }Uint32;
    double  value;
}union_double;
/*******************************************************************************************/

/*�����ͱ���*/
typedef enum
{
    false =0,
    true =!false
} bool;
typedef enum
{
    Ctrl_AUTO=0,
    Ctrl_MANUAL=!Ctrl_AUTO
}CtrlMode;
typedef enum
{
    Alert_N=0,
    Alert_Y=!Alert_N
}AlertMode;
//PWMͨ��
typedef enum
{
    TIM_CH_NULL=0x00,TIM_CH_1=0x01,TIM_CH_2=0x02,TIM_CH_12=0x03,
    TIM_CH_3=0x04,TIM_CH_13=0x05,TIM_CH_23=0x06,TIM_CH_123=0x07,
    TIM_CH_4=0x08,TIM_CH_14=0x09,TIM_CH_24=0x0A,TIM_CH_124=0x0B,
    TIM_CH_34=0x0C,TIM_CH_134=0x0D,TIM_CH_234=0x0E,TIM_CH_ALL=0x0F
}TIM_Channel;
//��������
typedef enum
{
    KEY_TYPE_SINGLE,
    KEY_TYPE_MATRIX
}KeyType_Def;
//�����¼�����
typedef enum
{
    KEY_EVENT_RELEASE=0x01,
    KEY_EVENT_PRESS=0x02,
    KEY_EVENT_LONG_PRESS=0x04,
    KEY_EVENT_ALL = KEY_EVENT_RELEASE|KEY_EVENT_PRESS|KEY_EVENT_LONG_PRESS
}KeyEvent_Def;
//�����¼�����
typedef struct
{
    u32 eventTime;
    KeyType_Def keyType;
    u8 keyCode;
    KeyEvent_Def keyEvent;
}My_KeyEvent;
//��������
typedef enum
{
    DATA_SIZE_BYTE,
    DATA_SIZE_HALFWORD,
    DATA_SIZE_WORD
}My_DataSize;
/***************************��������Ժ궨��****************************************/
#define boolean                         bool
#define TRUE                            true
#define FALSE                           false
#define DelayMs                         delay_ms
#define ArrayCount(table)               (sizeof(table)/sizeof(table[0]))
#if defined(USE_RT_THREAD)
#define delay_ms                        rt_thread_mdelay
#define My_memcmp(m,n,t)                (rt_memcmp(m,n,t)==0)
#define My_memcpy                       rt_memcpy
#define My_memset                       rt_memset
#define TICK_PER_SECOND                 RT_TICK_PER_SECOND//1���ӵĵδ����
#define My_SysTick_GetPeriod()          ((float)1000/TICK_PER_SECOND)
#define My_SysTick_GetTicks()           rt_tick_get()
#define My_SysTick_GetCounter()         (SysTick->LOAD + SysTick->VAL +1)
#else//USE_RT_THREAD
#define My_memcmp(m,n,t)                (memcmp(m,n,t)==0)
#define My_memcpy                       memcpy
#define My_memset                       memset
#endif//USE_RT_THREAD
#define DataStr_Complie                 __DATE__//����ʱ�Զ��������ʱ������
#define TimeStr_Complie                 __TIME__//����ʱ�Զ��������ʱ��ʱ��
#ifndef SET_BIT
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#endif
#ifndef CLEAR_BIT
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#endif
#ifndef READ_BIT
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#endif
#ifndef CLEAR_REG
#define CLEAR_REG(REG)        ((REG) = (0x0))
#endif
#ifndef WRITE_REG
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#endif
#ifndef READ_REG
#define READ_REG(REG)         ((REG))
#endif
#ifndef MODIFY_REG
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#endif
/*******************************************************************************************/

void SystemReset_Soft(void);//ϵͳ��λ
//����Ϊ��ຯ��
void WFI_SET(void);//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);//���������ж�
void MSR_MSP(u32 addr);//���ö�ջ��ַ
void printf_debug(const char *format, ...);
#define GetCPU_DEVID        DBGMCU_GetDEVID
bool StateMachine_GetStr(char *str,u8 msgByte,u8 *state);
u8 My_GetStrings(const char **strPtr,u8 *state,u8 strCount,u8 msgByte);
void ByteArrayToHalfWordArray(u8 *byteArray,u16 *halfWordArray,u16 lengthOfByteArray);
void HalfWordArrayToByteArray(u16 *halfWordArray,u8 *byteArray,u16 lengthOfHalfWordArray);
void SortShort(short *arr,u16 len,bool toBig);
int64 ParseInteger(const char *str,u16 len);//ԭGetIntegerFromString����
u16 BinarySearch_Float(float *tab,float value,u16 len);
u16 HexStringToBytes(char *hexStr,u8 *byteArray);
u16 My_Sysdef_GetVersion(void);

#endif
