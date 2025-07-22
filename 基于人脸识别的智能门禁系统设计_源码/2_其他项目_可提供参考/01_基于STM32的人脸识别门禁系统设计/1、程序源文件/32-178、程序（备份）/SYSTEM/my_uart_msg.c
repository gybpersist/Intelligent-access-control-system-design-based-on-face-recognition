/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ 51������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.3
//�޸�����:2020/09/08
//�����ܣ���װ�ͼ�ͨ�õĲ���
//�汾��2020/02/17     V1.0    ��ɻ�������
//�汾��2020/02/24     V1.1    �Ż����ֹ���
//�汾��2020/07/12     V1.2    1�����ӻ���������
                               2���޸���Ϣ�ص���������
//�汾��2020/09/08     V1.3    1�������Ż�
***********************************************/
#include "my_uart_msg.h"

_uart_msg_obj uartMsg[COUNT_MSG_UART_MAX];//�ṹ�� 

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
�������ܣ�������Ϣ
USARTx��������Ϣ�Ĵ���
msgPtr�����������ݵ�ָ��
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
�������ܣ����ݽ���״̬�����ڴ����жϵ���
msgByte�����ڽ��յ����ֽ�����
****************************************************************/
void My_UartMessage_StateMachine(u8 msgByte)
{
    static u8 index_dat=0;
    static u8 index_msg_read=0;
    static u8 startRecFlag = 0;    
    
    if (msgByte == '*')
    {
        startRecFlag = 1;//��������
        index_dat = 0;
        uartMsg[index_msg_read].payload[index_dat++] = msgByte;   //�洢���յ�������
    }
    else if(startRecFlag == 1)
    {    
        uartMsg[index_msg_read].payload[index_dat++] = msgByte; //�洢���յ�������
        if(msgByte  == '#')    
        {
            uartMsg[index_msg_read].enable = true;//��־���ݿ��Խ���Ӧ�ó�����
            index_msg_read++;//ָ����һ����Ϣ��
            if(index_msg_read>=COUNT_MSG_UART_MAX) //���չ��������� ���¼���
            {
                index_msg_read=0;
            }
            index_dat = 0;//����������½���
            startRecFlag =0;
        } 
        else if(index_dat>=LEN_MSG_UART_MAX)//�������ݲ��ܳ������峤��   
        {
            startRecFlag = 0;//����������½���
            index_dat = 0;        
        }        
    }
}
/****************************************************************
�������ܣ�������յ����ݣ����������е���
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
    if(uartMsg[index_msg_process].enable == true)//������յ���һ������������
    {
        OnGetUartMessage(&uartMsg[index_msg_process]);
        uartMsg[index_msg_process].enable = false;
        memset(uartMsg[index_msg_process].payload,0,uartMsg[index_msg_process].length);
        //ָ����һ����Ϣ
        index_msg_process++;
        if(index_msg_process==COUNT_MSG_UART_MAX)
        {
            index_msg_process=0;
        }
    }
}
/****************************************************************
�������ܣ���Ϣ���մ���ص�����
�˴�Ϊ������պ������������������ض���ʵ��
recMsgPtr�����յ�����Ϣ��ָ��
****************************************************************/
void __weak OnGetUartMessage(const _uart_msg_obj *uartMsgRec)
{

}

