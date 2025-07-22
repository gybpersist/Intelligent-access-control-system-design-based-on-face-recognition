/****************************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V3.2
//�޸�����:2020/09/20
//�����ܣ���װ�ͼ�ͨ�õĲ���
//�汾��V1.0   ��ɻ�������
//�汾��V2.0 2019/12/09 �޸���ʼ�ֽں�У��ͼ��㷽ʽ
//�汾��V2.1 2019/12/12 ���ӵ�ַ�ֽ�����ѡ��
//�汾��V2.2 2019/12/14 1�������Զ�У��ѡ��
                        2�������Զ��ظ�ѡ��
                        3���̶����ںţ��궨�壩
//�汾��V2.3 2019/12/17 �޸�һ��bug
//�汾��V3.0 2020/07/12 1���޸ĺ�������
                        2�����ӻ���������
                        3���޸���Ϣ�ص�����
                        4���޸�Ϊ��������Ϣ�����ֽ���
//�汾��V3.1 2020/09/08 1���������ݴ�������ղ���
//�汾��V3.2 2020/09/20 1�����ӽ���Ϣ����ת��Ϊ�ֽ�����ĺ���
                        2���޸����ܴ��ڵ�bug
****************************************************************/
#include "my_serial_msg.h"

static _serial_msg_obj serialMsg[NUM_MSG_SERIAL_MAX];//�ṹ��

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
�������ܣ�ͨ�����ڷ�����Ϣ
USARTx��������Ϣ�Ĵ��ں�
msgPtr�����������ݵ�ָ��
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
�������ܣ�ͨ�����ڽ����ݷ��͸�ָ���豸
USARTx��������Ϣ�Ĵ��ں�
addr_rec��������Ϣ���豸��ַ
msgPtr�����������ݵ�ָ��
****************************************************************/
void My_SerialMsg_SendTo(USART_TypeDef* USARTx, u32 addr_rec,const _serial_msg_obj *msgPtr)
{
    msgDat->addr_rec.value = addr_rec;
    msgDat->addr_send.value = ADDR_MYSELF;
    My_SerialMsg_Send(USARTx,msgDat);
}
/****************************************************************
�������ܣ�ͨ�����ڽ�ָ��͸�ָ���豸
USARTx��������Ϣ�Ĵ��ں�
addr_rec��������Ϣ���豸��ַ
cmd�������͵�ָ��
****************************************************************/
void My_SerialMsg_SendCmd(USART_TypeDef* USARTx, u32 addr_rec, u8 cmd)
{
    _serial_msg_obj msgDat;
    msgDat.addr_rec.value = addr_rec;
    msgDat.addr_send.value = ADDR_MYSELF;
#else
/****************************************************************
�������ܣ�ͨ�����ڽ�ָ��͸�ָ���豸
USARTx��������Ϣ�Ĵ��ں�
cmd�������͵�ָ��
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
�������ܣ�������Ϣ��У����
msgPtr����У������ݵ�ָ��
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
�������ܣ����������Ƿ���ȷ
msgPtr����У������ݵ�ָ��
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
�������ܣ���Ϣ����״̬�����ڴ����жϵ���
msgByte�����ڽ��յ��ֽ�����
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
        case STATE_LENGTH_0://��ȡ���ݳ��ȸ�λ
            serialMsg[index_msg_read].length = msgByte;//
#if BYTES_LEN_PAYLOAD==2
            state_msg = STATE_LENGTH_1;
            break;
        case STATE_LENGTH_1://��ȡ���ݳ��ȵ�λ
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
            serialMsg[index_msg_read].cmd = msgByte;//��ȡ����������
            if (serialMsg[index_msg_read].length == 0)//������ݳ���Ϊ0
            {
                state_msg = STATE_CHECK;//����У����
            }
            else
            {
                index_dat = 0;
                state_msg = STATE_DATA;//��������
            }
            break;
        case STATE_DATA:
            serialMsg[index_msg_read].payload[index_dat] = msgByte;//��������
            index_dat++;
            if (index_dat == serialMsg[index_msg_read].length)
            {
                state_msg = STATE_CHECK;//���ݽ�����ɣ���һ���ֽ���У���
            }
            break;
        case STATE_CHECK:
            serialMsg[index_msg_read].checkSum = msgByte;
            state_msg = STATE_NONE;//
            serialMsg[index_msg_read].enable = true;//������������Ϊ��Ч
            index_msg_read++;//ָ����һ����Ϣ
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
�������ܣ�������յ����ݣ����������е���
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
    if(serialMsg[index_msg_process].enable == true)//������յ���һ������������
    {   
    #if AUTO_CHECK>0
        if(My_SerialMsg_IsCorrect(&serialMsg[index_msg_process]))//������յ�����Ϣ����ȷ��
    #endif
        {
        #if ENABLE_ADDR>0
            if(serialMsg[index_msg_process].addr_rec.value == ADDR_MYSELF || serialMsg[index_msg_process].addr_rec.value == ADDR_BROADCAST)
        #endif
            {
            #if AUTO_RESPONSE>0
                //�ظ���Ϣ
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
        //ָ����һ����Ϣ
        index_msg_process++;
        if(index_msg_process==NUM_MSG_SERIAL_MAX)
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
void __weak OnGetSerialMessage(const _serial_msg_obj *recMsgPtr)
{
    
}

/****************************************************************
�������ܣ�����Ϣ����ת��Ϊ�ֽ�����
recMsgPtr����Ϣ�����ָ��
buff�������ֽ����ݵ�����ָ��
���أ���Ϣ��������ֽ���
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
�������ܣ���ȡ��Ϣ��������ֽ���
recMsgPtr����Ϣ�����ָ��
���أ���Ϣ��������ֽ���
****************************************************************/
u16 My_SerialMsg_GetTotalLength(const _serial_msg_obj *recMsgPtr)
{
    return recMsgPtr->length+LEN_SERIAL_NO_PAYLOAD;
}

