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
#include "my_serial_msg.h"

static _serial_msg_obj serialMsg[NUM_MSG_SERIAL_MAX];//结构体

#if USE_BUF_SERIAL_MESSAGE>0
static u8 recBuffer[MY_SERIAL_BUF_LEN];
static u8 index_save_buf = 0;
static u8 index_read_buf = 0;
void My_SerialMsg_SaveToBuffer(u8 dat)
{
    recBuffer[index_save_buf]=dat;
    index_save_buf++;
    if(index_save_buf==ArrayCount(recBuffer))
    {
        index_save_buf=0;
    }
}
#endif
/****************************************************************
函数功能：通过串口发送信息
USARTx：发送信息的串口号
msgPtr：待发送数据的指针
****************************************************************/
void My_SerialMsg_Send(USART_TypeDef* USARTx, const _serial_msg_obj *msgPtr)
{
    u16 i = 0;
    USARTSendByte(USARTx,MSG_START_MSB);
    USARTSendByte(USARTx,MSG_START_LSB);
#if BYTES_LEN_PAYLOAD==2
    USARTSendByte(USARTx,msgPtr->length>>8);
#endif
    USARTSendByte(USARTx,msgPtr->length);
#if ENABLE_ADDR>0
    USARTSendByte(USARTx,msgPtr->addr_rec.Bytes.byte_3);
    USARTSendByte(USARTx,msgPtr->addr_rec.Bytes.byte_2);
    USARTSendByte(USARTx,msgPtr->addr_rec.Bytes.byte_1);
    USARTSendByte(USARTx,msgPtr->addr_rec.Bytes.byte_0);
    USARTSendByte(USARTx,msgPtr->addr_send.Bytes.byte_3);
    USARTSendByte(USARTx,msgPtr->addr_send.Bytes.byte_2);
    USARTSendByte(USARTx,msgPtr->addr_send.Bytes.byte_1);
    USARTSendByte(USARTx,msgPtr->addr_send.Bytes.byte_0);
#endif
    USARTSendByte(USARTx,msgPtr->cmd);
    for(i=0;i<msgPtr->length;i++)
    {
        USARTSendByte(USARTx,msgPtr->payload[i]);
    }
    USARTSendByte(USARTx,My_SerialMsg_CalCheck(msgPtr));
}
#if ENABLE_ADDR>0
/****************************************************************
函数功能：通过串口将数据发送给指定设备
USARTx：发送信息的串口号
addr_rec：接收信息的设备地址
msgPtr：待发送数据的指针
****************************************************************/
void My_SerialMsg_SendTo(USART_TypeDef* USARTx, u32 addr_rec,const _serial_msg_obj *msgPtr)
{
    msgDat->addr_rec.value = addr_rec;
    msgDat->addr_send.value = ADDR_MYSELF;
    My_SerialMsg_Send(USARTx,msgDat);
}
/****************************************************************
函数功能：通过串口将指令发送给指定设备
USARTx：发送信息的串口号
addr_rec：接收信息的设备地址
cmd：待发送的指令
****************************************************************/
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u32 addr_rec, u8 cmd)
{
    _serial_msg_obj msgDat;
    msgDat.addr_rec.value = addr_rec;
    msgDat.addr_send.value = ADDR_MYSELF;
#else
/****************************************************************
函数功能：通过串口将指令发送给指定设备
USARTx：发送信息的串口号
cmd：待发送的指令
****************************************************************/
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u8 cmd)
{
    _serial_msg_obj msgDat;
#endif
    msgDat.length = 0;
    msgDat.cmd = cmd;
    My_SerialMsg_Send(USARTx,&msgDat);
}
/****************************************************************
函数功能：计算信息的校验码
msgPtr：待校验的数据的指针
****************************************************************/
u8 My_SerialMsg_CalCheck(const _serial_msg_obj *msgPtr)
{
    u8 check = 0;
    u16 i=0;
#if BYTES_LEN_PAYLOAD==2
    check += msgPtr->length>>8;
#endif
    check += msgPtr->length&0xff;
#if ENABLE_ADDR>0
    check += msgPtr->addr_rec.Bytes.byte_3;
    check += msgPtr->addr_rec.Bytes.byte_2;
    check += msgPtr->addr_rec.Bytes.byte_1;
    check += msgPtr->addr_rec.Bytes.byte_0;
    check += msgPtr->addr_send.Bytes.byte_3;
    check += msgPtr->addr_send.Bytes.byte_2;
    check += msgPtr->addr_send.Bytes.byte_1;
    check += msgPtr->addr_send.Bytes.byte_0;
#endif
    check += msgPtr->cmd;
    for(i=0;i<msgPtr->length;i++)
    {
        check += msgPtr->payload[i];
    }
    return 0xff-check;
}
/****************************************************************
函数功能：计算数据是否正确
msgPtr：待校验的数据的指针
****************************************************************/
bool My_SerialMsg_IsCorrect(const _serial_msg_obj *msgPtr)
{
    if(My_SerialMsg_CalCheck(msgPtr) == msgPtr->checkSum)
    {
        return true;
    }
    return false;
}
/****************************************************************
函数功能：信息接收状态机，在串口中断调用
msgByte：串口接收的字节数据
****************************************************************/
void My_SerialMsg_StateMachine(u8 msgByte)
{
    static u8 index_msg_read=0;
    static u8 state_msg=STATE_NONE;
    static u16 index_dat=0;
    static u8 msg_get_last=0;
    switch (state_msg)
    {
        case STATE_NONE:
            if (msg_get_last == MSG_START_MSB && msgByte == MSG_START_LSB)
            {
                msg_get_last = msgByte;
                state_msg = STATE_LENGTH_0;
            }
            break;
        case STATE_LENGTH_0://获取数据长度高位
            serialMsg[index_msg_read].length = msgByte;//
#if BYTES_LEN_PAYLOAD==2
            state_msg = STATE_LENGTH_1;
            break;
        case STATE_LENGTH_1://获取数据长度低位
            serialMsg[index_msg_read].length = serialMsg[index_msg_read].length*256 + msgByte;//
#endif
#if ENABLE_ADDR>0
            state_msg = STATE_ADDR_REC_1;
            break;
        case STATE_ADDR_REC_1:
            serialMsg[index_msg_read].addr_rec.Bytes.byte_3 = msgByte;
            state_msg = STATE_ADDR_REC_2;
            break;
        case STATE_ADDR_REC_2:
            serialMsg[index_msg_read].addr_rec.Bytes.byte_2 = msgByte;
            state_msg = STATE_ADDR_REC_3;
            break;
        case STATE_ADDR_REC_3:
            serialMsg[index_msg_read].addr_rec.Bytes.byte_1 = msgByte;
            state_msg = STATE_ADDR_REC_4;
            break;
        case STATE_ADDR_REC_4:
            serialMsg[index_msg_read].addr_rec.Bytes.byte_0 = msgByte;
            state_msg = STATE_ADDR_SEND_1;
            break;
        case STATE_ADDR_SEND_1:
            serialMsg[index_msg_read].addr_send.Bytes.byte_3 = msgByte;
            state_msg = STATE_ADDR_SEND_2;
            break;
        case STATE_ADDR_SEND_2:
            serialMsg[index_msg_read].addr_send.Bytes.byte_2 = msgByte;
            state_msg = STATE_ADDR_SEND_3;
            break;
        case STATE_ADDR_SEND_3:
            serialMsg[index_msg_read].addr_send.Bytes.byte_1 = msgByte;
            state_msg = STATE_ADDR_SEND_4;
            break;
        case STATE_ADDR_SEND_4:
            serialMsg[index_msg_read].addr_send.Bytes.byte_0 = msgByte;
#endif
            state_msg = STATE_CMD;
            break;
        case STATE_CMD:
            serialMsg[index_msg_read].cmd = msgByte;//获取数据命令字
            if (serialMsg[index_msg_read].length == 0)//如果数据长度为0
            {
                state_msg = STATE_CHECK;//接收校验码
            }
            else
            {
                index_dat = 0;
                state_msg = STATE_DATA;//接收数据
            }
            break;
        case STATE_DATA:
            serialMsg[index_msg_read].payload[index_dat] = msgByte;//保存数据
            index_dat++;
            if (index_dat == serialMsg[index_msg_read].length)
            {
                state_msg = STATE_CHECK;//数据接收完成，下一个字节是校验和
            }
            break;
        case STATE_CHECK:
            serialMsg[index_msg_read].checkSum = msgByte;
            state_msg = STATE_NONE;//
            serialMsg[index_msg_read].enable = true;//将此数据设置为有效
            index_msg_read++;//指向下一条信息
            if(index_msg_read==NUM_MSG_SERIAL_MAX)
            {
                index_msg_read=0;
            }
            break;
            default:break;
    }
    msg_get_last = msgByte;
}
/****************************************************************
函数功能：处理接收的数据，在主程序中调用
****************************************************************/
void My_SerialMsg_Process(void)
{
    static u8 index_msg_process=0;
#if USE_BUF_SERIAL_MESSAGE>0
    while(index_read_buf!=index_save_buf)
    {
        My_SerialMsg_StateMachine(recBuffer[index_read_buf]);
        index_read_buf++;
        if(index_read_buf==ArrayCount(recBuffer))
        {
            index_read_buf=0;
        }
    }
#endif
    if(serialMsg[index_msg_process].enable == true)//如果接收到了一个完整的数据
    {   
    #if AUTO_CHECK>0
        if(My_SerialMsg_IsCorrect(&serialMsg[index_msg_process]))//如果接收到的信息是正确的
    #endif
        {
        #if ENABLE_ADDR>0
            if(serialMsg[index_msg_process].addr_rec.value == ADDR_MYSELF || serialMsg[index_msg_process].addr_rec.value == ADDR_BROADCAST)
        #endif
            {
            #if AUTO_RESPONSE>0
                //回复信息
                _serial_msg_obj serialMsg_send;
                serialMsg_send.length = serialMsg[index_msg_process].length;
            #if ENABLE_ADDR>0
                serialMsg_send.addr_rec = serialMsg[index_msg_process].addr_send;
                serialMsg_send.addr_send.value = ADDR_MYSELF;
            #endif
                serialMsg_send.cmd = CMD_RESPONSE | serialMsg[index_msg_process].cmd;
                memcpy(serialMsg_send.payload,serialMsg[index_msg_process].payload,serialMsg[index_msg_process].length);
//                serialMsg_send.checkSum = CalculateCheckSum(serialMsg_send);
                My_SerialMsg_Send(USART_SERIAL_MESSAGE,&serialMsg_send);
            #endif
                OnGetSerialMessage(&serialMsg[index_msg_process]);
                memset(serialMsg[index_msg_process].payload,0,serialMsg[index_msg_process].length);
            }
        }
        serialMsg[index_msg_process].enable = false;
        //指向下一条信息
        index_msg_process++;
        if(index_msg_process==NUM_MSG_SERIAL_MAX)
        {
            index_msg_process=0;
        }
    }
}
/****************************************************************
函数功能：信息接收处理回调函数
此处为弱定义空函数，开发者需自行重定义实现
recMsgPtr：接收到的信息的指针
****************************************************************/
void __weak OnGetSerialMessage(const _serial_msg_obj *recMsgPtr)
{
    
}

/****************************************************************
函数功能：将消息对象转换为字节数组
recMsgPtr：信息对象的指针
buff：保存字节数据的数组指针
返回：消息对象的总字节数
****************************************************************/
u16 My_SerialMsg_ToBytes(const _serial_msg_obj *recMsgPtr,u8 *buff)
{
    *buff++ = MSG_START_MSB;
    *buff++ = MSG_START_LSB;
#if LEN_MSG_SERIAL_MAX>255
    *buff++ = (recMsgPtr->length)>>8;
#endif
    *buff++ = (recMsgPtr->length);
    *buff++ = (recMsgPtr->cmd);
    My_memcpy(buff,recMsgPtr->payload,recMsgPtr->length);
    buff+=recMsgPtr->length;
    *buff = My_SerialMsg_CalCheck(recMsgPtr);
    return recMsgPtr->length+LEN_SERIAL_NO_PAYLOAD;
}
/****************************************************************
函数功能：获取消息对象的总字节数
recMsgPtr：信息对象的指针
返回：消息对象的总字节数
****************************************************************/
u16 My_SerialMsg_GetTotalLength(const _serial_msg_obj *recMsgPtr)
{
    return recMsgPtr->length+LEN_SERIAL_NO_PAYLOAD;
}

