/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ 51开发板
//通旺科技@TWKJ
//作者：xdd
//版本：V1.0
//修改日期:2020/09/08
//程序功能：获取以回车换行结束的串口数据
//版本：2020/09/08     V1.0    完成基本功能
***********************************************/
#include "my_face.h"

static _FaceData_msg_obj recMsg;
static u8 index_dat=0;

static u16 faceId = 0;//人脸识别ID号
static u8 backResult = 0;//返回结果 0没有返回 1返回成功

u16 crcTest=0;//校验值

u8 inputFaceCmd[]   = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0xE5}; //录入人脸 无延时
u8 shibieFaceCmd[]  = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x02,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0xC2}; //识别 无延时
u8 delAllFaceCmd[]  = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x0D,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x1E}; //删除所有
u8 delFaceIdCmd[]   = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x04,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x5F,0xD1}; ////删除ID为9的人脸信息
u8 cancelCmd[]      = {0x5A,0x2C,0x00,0x00,0x00,0x12,0x13,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD4,0xB7}; //取消当前命令


void sendInputFaceCmd(void)
{
    faceId = 0;//人脸识别ID号
    backResult = 0;// 0没有返回 1返回成功
    USARTSendBytes(FACE_USART_MESSAGE,inputFaceCmd,sizeof(inputFaceCmd));	 //发送	录入人脸 无延时
}
void shiBieFaceCmd(void)
{
    faceId = 0;//人脸识别ID号
    backResult = 0;// 0没有返回 1返回成功
    USARTSendBytes(FACE_USART_MESSAGE,shibieFaceCmd,sizeof(shibieFaceCmd));	 //发送	识别命令
}
void deleteAllFaceCmd(void)
{
    faceId = 0;//人脸识别ID号
    backResult = 0;// 0没有返回 1返回成功
    USARTSendBytes(FACE_USART_MESSAGE,delAllFaceCmd,sizeof(delAllFaceCmd));	 //发送 删除所有	
}
void deleteFaceIdCmd(u16 midId )//取消当前命令
{
    faceId = 0;//人脸识别ID号
    backResult = 0;// 0没有返回 1返回成功
    delFaceIdCmd[14] = midId/256;//填充数据
    delFaceIdCmd[15] = midId%256;
    crcTest = xbx_crc_calculate(delFaceIdCmd,sizeof(delFaceIdCmd)-2);//测试crc校验
    delFaceIdCmd[16] = crcTest/256;//填充数据
    delFaceIdCmd[17] = crcTest%256;    
    USARTSendBytes(FACE_USART_MESSAGE,delFaceIdCmd,sizeof(delFaceIdCmd));	 //发送	取消当前命令
}

void cancelCurFaceCmd(void)//取消当前命令
{
    faceId = 0;//人脸识别ID号
    backResult = 0;// 0没有返回 1返回成功
    USARTSendBytes(FACE_USART_MESSAGE,cancelCmd,sizeof(cancelCmd));	 //发送	取消当前命令
}
    
uint16_t xbx_crc_calculate(uint8_t *addr ,uint8_t datalen)//CRC校验 参考 除校验位的其他全部字节
{
    uint8_t i,j;
    uint16_t crc; //定义crc变量
    crc = 0xffff;//初始化
    for(i = 0; i < datalen; i++)
    {
        crc ^= *addr++;//运算
        for(j=0;j<8;j++)
        {
            if(crc & 0x01)
            {
                crc = (crc >> 1) ^ 0x8408; //该运算厂家提供 
            }
            else {crc >>= 0x01;}
        }
    }    
    return ~crc ;//返回结果
}

/****************************************************************
函数功能：数据接收状态机，在串口中断调用
msgByte：串口接收到的字节数据
****************************************************************/
void My_FaceData_StateMachine(u8 msgByte)
{
    static u8 index_msg_read=0;
    static u8 msgByte_last=0; 
//    u16 dataLength=0;//数据接收长度
    u16 recCmd=0;//接收命令
    if (msgByte_last ==0x5A &&msgByte == 0x2C)//接收到启动标志
    {
        recMsg.payload[0]=msgByte_last;
        recMsg.length = 0;//接收数据长度初始化
        index_dat=1;
    }     
    if(index_dat>0 &&  index_dat<=LEN_MSG_FACE_MAX)
    {
        recMsg.payload[index_dat] = msgByte;
        index_dat++;
    }
    
    if(index_dat==8) //接收到命令结束
    {
        recMsg.length = recMsg.payload[4]*256 + recMsg.payload[5];//获取数据长度       
    }
    else if(index_dat>=(8+(recMsg.length - 10)+2) && recMsg.length != 0) //接收结束 8表示接收到命令结束长度 led-10数据长度 2表示校验2字节
    {
        crcTest =  xbx_crc_calculate(recMsg.payload,8+(recMsg.length - 10));//获取校验
        if(crcTest == (recMsg.payload[8+(recMsg.length - 10)]*256+recMsg.payload[8+(recMsg.length - 10)+1]) )//校验成功
        {
            recCmd = recMsg.payload[6]*256+recMsg.payload[7];//获取接收命令
            switch (recCmd)//判断命令
            {
                case 0x030A://录入人脸 无超时 应答
                    backResult = 1;// 0没有返回 1返回成功
                    break;
                case 0x030D://录入返回结果
                    backResult = 1;// 0没有返回 1返回成功
                    if(recMsg.payload[18] == 0x00) //录入成功
                    {
                        faceId = recMsg.payload[16]*256 + recMsg.payload[17];
                    }
                    else faceId = 0;//返回录入失败 返回0
                    break;            
                case 0x020A://无超时 单次 识别应答
                    backResult = 1;// 0没有返回 1返回成功
                    break;            
                case 0x020D://识别返回结果
                    backResult = 1;// 0没有返回 1返回成功
                    if(recMsg.payload[19] == 0x00) //录入成功
                    {
                        faceId = recMsg.payload[17]*256 + recMsg.payload[18];
                    }
                    else faceId = 0;//返回录入失败 返回0
                    break;            
                case 0x040A://删除单个ID
                    backResult = 1;// 0没有返回 1返回成功
                    break;            
                case 0x0D0A://删除所有ID
                    backResult = 1;// 0没有返回 1返回成功
                    break;   
                default : break;
            }
        
        }
        recMsg.length = 0;//接收数据长度初始化
        index_dat = 0;
    }    

    msgByte_last=msgByte;//最新接收的一个数据    
}

u16 getFaceId(void)//获取当前返回ID
{
    return faceId;
}
u16 getFaceCmdkRes(void)//是否接收到了返回命令 结果也可能是失败的 根据具体操作
{
    return backResult;
}

