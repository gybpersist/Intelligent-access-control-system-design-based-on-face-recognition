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
#ifndef __MY_FACE_MSG_H
#define __MY_FACE_MSG_H
#include "my_include.h"


#define FACE_USART_MESSAGE            USART3


#define COUNT_MSG_FACE_MAX                      4//
#define LEN_MSG_FACE_MAX                        32//信息容量
/****************************************************************************************/

#define My_FaceData_Receive                  My_FaceData_StateMachine

typedef struct 
{
    bool enable;
    u16 length;
    u8 payload[LEN_MSG_FACE_MAX];
}_FaceData_msg_obj;

uint16_t xbx_crc_calculate(uint8_t *addr ,uint8_t datalen);


void My_FaceData_StateMachine(u8 msgByte);
void My_FaceData_Process(void);

void sendInputFaceCmd(void);//录入人脸 无超时
void shiBieFaceCmd(void); //识别命令 无超时
void deleteAllFaceCmd(void);//删除所有	
void deleteFaceIdCmd(u16 midId ); //删除某ID人脸信息
void cancelCurFaceCmd(void);//取消当前命令

u16 getFaceId(void);//获取当前返回ID
u16 getFaceCmdkRes(void);//是否接收到了返回命令 结果也可能是失败的 根据具体操作

#endif
