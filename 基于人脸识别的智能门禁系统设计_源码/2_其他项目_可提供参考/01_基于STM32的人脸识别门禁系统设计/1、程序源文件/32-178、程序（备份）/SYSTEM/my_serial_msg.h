/****************************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V3.2
//修改日期:2020/09/20
//程序功能：封装和简化通用的操作
//版本：V1.0   完成基本功能
//版本：V2.0 2019/12/09 修改起始字节和校验和计算方式
//版本：V2.1 2019/12/12 增加地址字节配置选项
//版本：V2.2 2019/12/14 1、增加自动校验选项
                        2、增加自动回复选项
                        3、固定串口号（宏定义）
//版本：V2.3 2019/12/17 修复一个bug
//版本：V3.0 2020/07/12 1、修改函数名称
                        2、增加缓冲区功能
                        3、修改消息回调函数
                        4、修改为可配置消息长度字节数
//版本：V3.1 2020/09/08 1、增加数据处理后的清空操作
//版本：V3.2 2020/09/20 1、增加将消息对象转换为字节数组的函数
                        2、修复可能存在的bug
****************************************************************/
#ifndef __SERIAL_MSG_H
#define __SERIAL_MSG_H
#include "my_include.h"

/************************************
自定义协议：
定义      起始字节        数据长度        命令      数据内容        校验和
标识      smsb slsb       length          cmd       data  data      check
示例      0x2a 0xd5       0x02            0x04      0x50 0x78       0x31
**************************************
为了确定信息的起始与结束，定义了起始字节和结束字节，在程序检测到起始字节时则认为是信息的开始，
在此之前接收到的数据如果是完整的信息则交给其他程序处理，如果是不完整的或者是无意义的数据则将其丢弃。
为了使信息容量较大、传输效率较高，本协议使用数据的具体数值表示数据传输的内容，不使用数据所代表的ASCII码。
如数据0x31表示的是数字49，而不是ASCII码字符1。
理论上在完整的数据中，0x00-0xff是都有可能出现的，所以我们将起始字节和结束字节设置为固定的2个字节，
2个字节的代数和为0xff，这样就基本上可以避免在消息中间出现起始字节或结束字节而被误认为是数据的开始或结束（理论上仍然有可能出现，但概率非常低）。
起始字节：2个字节，0x2a、0xd5，0x2a的ASCII字符为'*'，0x2a+0xd5=0xff
数据长度：1个字节，表示数据内容的长度（数据内容的长度是可变的）
命令字节：1个字节，表示当前信息是哪一种信息
数据内容：0~255个字节，长度是可变的，最大255个字节
校验和：1个字节，用于校验接收到的信息是否正确，计算方法是
check = 0xff - (length+cmd+data[0]+data[1]+...+data[n])&0xff
如上表中：0x31 = 0xff - (0x02+0x04+0x50+0x78)&0xff

***************************************/
#define NUM_MSG_SERIAL_MAX                      4//
#define LEN_MSG_SERIAL_MAX                      24//信息容量,若使用RTOS，须注意线程栈空间大小
#define MSG_START_MSB                           0x2a//串口发送数据起始高字节
#define MSG_START_LSB                           0xd5//串口发送数据起始低字节
#if LEN_MSG_SERIAL_MAX>255
#define LEN_SERIAL_NO_PAYLOAD                   6//消息对象中除payload意外的数据长度
#else
#define LEN_SERIAL_NO_PAYLOAD                   5//消息对象中除payload意外的数据长度
#endif
#define AUTO_CHECK                              0//是否使能自动校验数据
#define AUTO_RESPONSE                           0//是否使能自动回复
#define ENABLE_ADDR                             0//是否使能地址选项
#define USE_BUF_SERIAL_MESSAGE                  0//是否使用自定义串口缓冲区
#define USART_SERIAL_MESSAGE                    USART1//接收信息的串口
/*****************************************************************************************/
#if LEN_MSG_SERIAL_MAX>65535
#error  LEN_MSG_SERIAL_MAX value can not be bigger than 65535 !
#endif

#if USE_BUF_SERIAL_MESSAGE>0
#define My_SerialMsg_Receive                    My_SerialMsg_SaveToBuffer
#define MY_SERIAL_BUF_LEN                       128
#else
#define My_SerialMsg_Receive                    My_SerialMsg_StateMachine
#endif

typedef enum
{
    STATE_NONE=0x00,
    STATE_LENGTH_0,
    STATE_LENGTH_1,
#if ENABLE_ADDR>0
    STATE_ADDR_REC_1,
    STATE_ADDR_REC_2,
    STATE_ADDR_REC_3,
    STATE_ADDR_REC_4,
    STATE_ADDR_SEND_1,
    STATE_ADDR_SEND_2,
    STATE_ADDR_SEND_3,
    STATE_ADDR_SEND_4,
#endif
    STATE_CMD,
    STATE_DATA,
    STATE_CHECK,
}SerialMessageState;
/****************************************************************************************/
#define ADDR_MYSELF                     0x12345678
#define ADDR_BROADCAST                  0xFFFFFFFF
//
#define CMD_RESPONSE                    0x80//应答指令，与接收到的命令或运算后返回给信息发送者
#define CMD_CAR_DEBUG                   0x00//发送字符串
#define CMD_CAR_READY                   0x01//准备就绪
#define CMD_CAR_STATUS                  0x02//小车状态
#define CMD_CAR_SENSOR                  0x04//小车传感器（预留）
#define CMD_CAR_BATTERY                 0x05//电池状态（预留）
#define CMD_CAR_DIRECTION               0x06//小车方向

#define CMD_APP_DEBUG                   0x00//发送字符串
#define CMD_APP_READY                   0x01//准备就绪
#define CMD_APP_GET_CAR_STATUS          0x02//获取小车状态
#define CMD_APP_GET_CAR_SPEED           0x03//获取小车速度
#define CMD_APP_GET_CAR_DIR             0x04//获取小车方向
#define CMD_APP_GET_CAR_SENSOR          0x05//获取小车传感器状态（预留）
#define CMD_APP_GET_CAR_BATTERY         0x06//获取小车电池状态（预留）
#define CMD_APP_SET_CAR_STOP            0x10//停车
#define CMD_APP_SET_CAR_START           0x11//启动
#define CMD_APP_SET_CAR_STATUS          0x12//设置小车状态
#define CMD_APP_SET_CAR_SPEED           0x13//设置小车速度
#define CMD_APP_SET_CAR_DIR             0x14//设置小车方向
#define CMD_APP_SET_CAR_SENSOR          0x15//设置小车传感器状态（预留）

typedef struct
{
#if LEN_MSG_SERIAL_MAX>255
    u16 length;
#else
    u8 length;
#endif
#if ENABLE_ADDR>0
    union_u32 addr_rec;
    union_u32 addr_send;
#endif
    u8 cmd;
    u8 payload[LEN_MSG_SERIAL_MAX];
    u8 checkSum;
    bool enable;
}_serial_msg_obj;

#if USE_BUF_SERIAL_MESSAGE>0
void My_SerialMsg_SaveToBuffer(u8 dat);
#endif
void My_SerialMsg_Send(USART_TypeDef* USARTx, const _serial_msg_obj *msgPtr);
#if ENABLE_ADDR>0
void My_SerialMsg_SendTo(USART_TypeDef* USARTx, u32 addr_rec, _serial_msg_obj *msgPtr);
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u32 addr_rec, u8 cmd);
#else
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u8 cmd);
#endif
bool My_SerialMsg_IsCorrect(const _serial_msg_obj *msgPtr);
u8 My_SerialMsg_CalCheck(const _serial_msg_obj *msgPtr);
void My_SerialMsg_StateMachine(u8 msgByte);
void My_SerialMsg_Process(void);
//函数原型需要开发者自己定义
void OnGetSerialMessage(const _serial_msg_obj *recMsgPtr);
u16 My_SerialMsg_ToBytes(const _serial_msg_obj *recMsgPtr,u8 *buff);
u16 My_SerialMsg_GetTotalLength(const _serial_msg_obj *recMsgPtr);
#endif
