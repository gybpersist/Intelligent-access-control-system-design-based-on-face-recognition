/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ 51开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.3
//修改日期:2020/09/08
//程序功能：封装和简化通用的操作
//版本：2020/02/17     V1.0    完成基本功能
//版本：2020/02/24     V1.1    优化部分功能
//版本：2020/07/12     V1.2    1、增加缓冲区功能
                               2、修改消息回调函数定义
//版本：2020/09/08     V1.3    1、程序优化
***********************************************/
#include "my_uart_msg.h"

_uart_msg_obj uartMsg[COUNT_MSG_UART_MAX];//结构体 

#if USE_BUF_UART_MESSAGE>0
static u8 recBuffer[MY_UART_BUF_LEN];
static u8 index_save_buf = 0;
static u8 index_read_buf = 0;
void My_UartMessage_SaveToBuffer(u8 dat)
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
函数功能：发送信息
USARTx：发送信息的串口
msgPtr：待发送数据的指针
****************************************************************/
void My_UartMessage_Send(USART_TypeDef* USARTx, _uart_msg_obj *msgPtr)
{
    u8 i = 0;
    USARTSendByte(USARTx, MSG_START);
    for(i=0;i<msgPtr->length;i++)
    {
        USARTSendByte(USARTx, msgPtr->payload[i]);
    }
    USARTSendByte(USARTx, MSG_END);
}
/****************************************************************
函数功能：数据接收状态机，在串口中断调用
msgByte：串口接收到的字节数据
****************************************************************/
void My_UartMessage_StateMachine(u8 msgByte)
{
    static u8 index_dat=0;
    static u8 index_msg_read=0;
    static u8 startRecFlag = 0;    
    
    if (msgByte == '*')
    {
        startRecFlag = 1;//启动接收
        index_dat = 0;
        uartMsg[index_msg_read].payload[index_dat++] = msgByte;   //存储接收到的数据
    }
    else if(startRecFlag == 1)
    {    
        uartMsg[index_msg_read].payload[index_dat++] = msgByte; //存储接收到的数据
        if(msgByte  == '#')    
        {
            uartMsg[index_msg_read].enable = true;//标志数据可以交给应用程序处理
            index_msg_read++;//指向下一个消息体
            if(index_msg_read>=COUNT_MSG_UART_MAX) //接收过多组数据 重新计数
            {
                index_msg_read=0;
            }
            index_dat = 0;//清空数据重新接收
            startRecFlag =0;
        } 
        else if(index_dat>=LEN_MSG_UART_MAX)//单独数据不能超过定义长度   
        {
            startRecFlag = 0;//清空数据重新接收
            index_dat = 0;        
        }        
    }
}
/****************************************************************
函数功能：处理接收的数据，在主程序中调用
****************************************************************/
void My_UartMessage_Process(void)
{
    static u8 index_msg_process=0;
    u8 i;

#if USE_BUF_UART_MESSAGE>0
    while(index_read_buf!=index_save_buf)
    {
        My_UartMessage_StateMachine(recBuffer[index_read_buf]);
        index_read_buf++;
        if(index_read_buf==ArrayCount(recBuffer))
        {
            index_read_buf=0;
        }
    }
#endif
    if(uartMsg[index_msg_process].enable == true)//如果接收到了一个完整的数据
    {
        OnGetUartMessage(&uartMsg[index_msg_process]);
        uartMsg[index_msg_process].enable = false;
        memset(uartMsg[index_msg_process].payload,0,uartMsg[index_msg_process].length);
        //指向下一条信息
        index_msg_process++;
        if(index_msg_process==COUNT_MSG_UART_MAX)
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
void __weak OnGetUartMessage(const _uart_msg_obj *uartMsgRec)
{

}

