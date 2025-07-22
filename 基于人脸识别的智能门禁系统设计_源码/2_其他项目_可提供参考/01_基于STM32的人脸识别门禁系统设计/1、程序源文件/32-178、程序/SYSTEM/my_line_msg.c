/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ 51开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/09/08
//程序功能：获取以回车换行结束的串口数据
//版本：2020/09/08     V1.0    完成基本功能
***********************************************/
#include "my_line_msg.h"

static _line_msg_obj recMsg[COUNT_MSG_LINE_MAX];

#if USE_BUF_LINE_MESSAGE>0
static u8 recBuffer[MY_LINE_BUF_LEN];
static u8 index_save_buf = 0;
static u8 index_read_buf = 0;
void My_LineMessage_SaveToBuffer(u8 dat)
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
函数功能：数据接收状态机，在串口中断调用
msgByte：串口接收到的字节数据
****************************************************************/
void My_LineMessage_StateMachine(u8 msgByte)
{
    static u8 index_msg_read=0;
    static u8 index_dat=0;
    static u8 msgByte_last=0;
    if(msgByte_last=='\r' && msgByte=='\n')
    {
        recMsg[index_msg_read].payload[index_dat-1] = 0;
        recMsg[index_msg_read].length = index_dat-1;
        recMsg[index_msg_read].enable = true;
        index_dat = 0;
        index_msg_read++;//指向下一条信息
        index_msg_read %= COUNT_MSG_LINE_MAX;//如果是最后一条则跳转到第一条，防止数组越界
    }
    else if(index_dat<=LEN_MSG_LINE_MAX)
    {
        recMsg[index_msg_read].payload[index_dat] = msgByte;
        index_dat++;
    }
    msgByte_last=msgByte;
}
/****************************************************************
函数功能：处理接收的数据，在主程序中调用
****************************************************************/
void My_LineMessage_Process(void)
{
    static u8 index_msg_process=0;
    u8 i;

#if USE_BUF_LINE_MESSAGE>0
    while(index_read_buf!=index_save_buf)
    {
        My_LineMessage_StateMachine(recBuffer[index_read_buf]);
        index_read_buf++;
        if(index_read_buf==ArrayCount(recBuffer))
        {
            index_read_buf=0;
        }
    }
#endif
    if(recMsg[index_msg_process].enable == true)//如果接收到了一个完整的数据
    {
        OnGetLineMessage(&recMsg[index_msg_process]);
        recMsg[index_msg_process].enable = false;
        memset(recMsg[index_msg_process].payload,0,recMsg[index_msg_process].length);
        //指向下一条信息
        index_msg_process++;
        if(index_msg_process==COUNT_MSG_LINE_MAX)
        {
            index_msg_process=0;
        }
    }
}
/****************************************************************
函数功能：信息接收处理回调函数
此处为弱定义空函数，开发者需自行重定义实现
lineMsgRec：接收到的信息的指针
****************************************************************/
void __weak OnGetLineMessage(const _line_msg_obj *lineMsgRec)
{

}

