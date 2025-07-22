/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ 51������
//ͨ���Ƽ�@TWKJ
//���ߣ�xdd
//�汾��V1.0
//�޸�����:2020/09/08
//�����ܣ���ȡ�Իس����н����Ĵ�������
//�汾��2020/09/08     V1.0    ��ɻ�������
***********************************************/
#include "my_face.h"

static _FaceData_msg_obj recMsg;
static u8 index_dat=0;

static u16 faceId = 0;//����ʶ��ID��
static u8 backResult = 0;//���ؽ�� 0û�з��� 1���سɹ�

u16 crcTest=0;//У��ֵ

u8 inputFaceCmd[]   = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0xE5}; //¼������ ����ʱ
u8 shibieFaceCmd[]  = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x02,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0xC2}; //ʶ�� ����ʱ
u8 delAllFaceCmd[]  = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x0D,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x1E}; //ɾ������
u8 delFaceIdCmd[]   = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x04,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x5F,0xD1}; ////ɾ��IDΪ9��������Ϣ
u8 cancelCmd[]      = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x13,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD4,0xB7}; //ȡ����ǰ����


void sendInputFaceCmd(void)
{
    faceId = 0;//����ʶ��ID��
    backResult = 0;// 0û�з��� 1���سɹ�
    USARTSendBytes(FACE_USART_MESSAGE,inputFaceCmd,sizeof(inputFaceCmd));	 //����	¼������ ����ʱ
}
void shiBieFaceCmd(void)
{
    faceId = 0;//����ʶ��ID��
    backResult = 0;// 0û�з��� 1���سɹ�
    USARTSendBytes(FACE_USART_MESSAGE,shibieFaceCmd,sizeof(shibieFaceCmd));	 //����	ʶ������
}
void deleteAllFaceCmd(void)
{
    faceId = 0;//����ʶ��ID��
    backResult = 0;// 0û�з��� 1���سɹ�
    USARTSendBytes(FACE_USART_MESSAGE,delAllFaceCmd,sizeof(delAllFaceCmd));	 //���� ɾ������	
}
void deleteFaceIdCmd(u16 midId )//ȡ����ǰ����
{
    faceId = 0;//����ʶ��ID��
    backResult = 0;// 0û�з��� 1���سɹ�
    delFaceIdCmd[14] = midId/256;//�������
    delFaceIdCmd[15] = midId%256;
    crcTest = xbx_crc_calculate(delFaceIdCmd,sizeof(delFaceIdCmd)-2);//����crcУ��
    delFaceIdCmd[16] = crcTest/256;//�������
    delFaceIdCmd[17] = crcTest%256;    
    USARTSendBytes(FACE_USART_MESSAGE,delFaceIdCmd,sizeof(delFaceIdCmd));	 //����	ȡ����ǰ����
}

void cancelCurFaceCmd(void)//ȡ����ǰ����
{
    faceId = 0;//����ʶ��ID��
    backResult = 0;// 0û�з��� 1���سɹ�
    USARTSendBytes(FACE_USART_MESSAGE,cancelCmd,sizeof(cancelCmd));	 //����	ȡ����ǰ����
}
    
uint16_t xbx_crc_calculate(uint8_t *addr ,uint8_t datalen)//CRCУ�� �ο� ��У��λ������ȫ���ֽ�
{
    uint8_t i,j;
    uint16_t crc; //����crc����
    crc = 0xffff;//��ʼ��
    for(i = 0; i < datalen; i++)
    {
        crc ^= *addr++;//����
        for(j=0;j<8;j++)
        {
            if(crc & 0x01)
            {
                crc = (crc >> 1) ^ 0x8408; //�����㳧���ṩ 
            }
            else {crc >>= 0x01;}
        }
    }    
    return ~crc ;//���ؽ��
}

/****************************************************************
�������ܣ����ݽ���״̬�����ڴ����жϵ���
msgByte�����ڽ��յ����ֽ�����
****************************************************************/
void My_FaceData_StateMachine(u8 msgByte)
{
    static u8 index_msg_read=0;
    static u8 msgByte_last=0; 
//    u16 dataLength=0;//���ݽ��ճ���
    u16 recCmd=0;//��������
    if (msgByte_last ==0x5A &&msgByte == 0x2C)//���յ�������־
    {
        recMsg.payload[0]=msgByte_last;
        recMsg.length = 0;//�������ݳ��ȳ�ʼ��
        index_dat=1;
    }     
    if(index_dat>0 &&  index_dat<=LEN_MSG_FACE_MAX)
    {
        recMsg.payload[index_dat] = msgByte;
        index_dat++;
    }
    
    if(index_dat==8) //���յ��������
    {
        recMsg.length = recMsg.payload[4]*256 + recMsg.payload[5];//��ȡ���ݳ���       
    }
    else if(index_dat>=(8+(recMsg.length - 10)+2) && recMsg.length != 0) //���ս��� 8��ʾ���յ������������ led-10���ݳ��� 2��ʾУ��2�ֽ�
    {
        crcTest =  xbx_crc_calculate(recMsg.payload,8+(recMsg.length - 10));//��ȡУ��
        if(crcTest == (recMsg.payload[8+(recMsg.length - 10)]*256+recMsg.payload[8+(recMsg.length - 10)+1]) )//У��ɹ�
        {
            recCmd = recMsg.payload[6]*256+recMsg.payload[7];//��ȡ��������
            switch (recCmd)//�ж�����
            {
                case 0x030A://¼������ �޳�ʱ Ӧ��
                    backResult = 1;// 0û�з��� 1���سɹ�
                    break;
                case 0x030D://¼�뷵�ؽ��
                    backResult = 1;// 0û�з��� 1���سɹ�
                    if(recMsg.payload[18] == 0x00) //¼��ɹ�
                    {
                        faceId = recMsg.payload[16]*256 + recMsg.payload[17];
                    }
                    else faceId = 0;//����¼��ʧ�� ����0
                    break;            
                case 0x020A://�޳�ʱ ���� ʶ��Ӧ��
                    backResult = 1;// 0û�з��� 1���سɹ�
                    break;            
                case 0x020D://ʶ�𷵻ؽ��
                    backResult = 1;// 0û�з��� 1���سɹ�
                    if(recMsg.payload[19] == 0x00) //¼��ɹ�
                    {
                        faceId = recMsg.payload[17]*256 + recMsg.payload[18];
                    }
                    else faceId = 0;//����¼��ʧ�� ����0
                    break;            
                case 0x040A://ɾ������ID
                    backResult = 1;// 0û�з��� 1���سɹ�
                    break;            
                case 0x0D0A://ɾ������ID
                    backResult = 1;// 0û�з��� 1���سɹ�
                    break;   
                default : break;
            }
        
        }
        recMsg.length = 0;//�������ݳ��ȳ�ʼ��
        index_dat = 0;
    }    

    msgByte_last=msgByte;//���½��յ�һ������    
}

u16 getFaceId(void)//��ȡ��ǰ����ID
{
    return faceId;
}
u16 getFaceCmdkRes(void)//�Ƿ���յ��˷������� ���Ҳ������ʧ�ܵ� ���ݾ������
{
    return backResult;
}

