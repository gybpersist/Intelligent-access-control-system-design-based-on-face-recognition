/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/11/17
//程序功能：封装和简化JR6001语音模块的操作
//V1.0  2020/11/17  1、完成基本功能
************************************************/
/***********************************************
*++++++++++++++++++++++++++++++++++++++++++++++*
*注意：因模块指令结尾无特殊字符，故两条指令之间必须
        有延时(视具体调试而定)，上电初始化也需要延时
        另外，若工程编译优化等级为0，实测模块无响应
*++++++++++++++++++++++++++++++++++++++++++++++*
************************************************/
#ifndef __MY_JR6001_H
#define __MY_JR6001_H
#include "my_include.h"

#define UART_JR6001                 USART3
#define PIN_JR6001_BUSY             PB12

#define CMD_JR6001_PLAY             "A2"//播放
#define CMD_JR6001_PAUSE            "A3"//暂停
#define CMD_JR6001_STOP             "A4"//停止
#define CMD_JR6001_PREV             "A5"//上一曲
#define CMD_JR6001_NEXT             "A6"//下一曲
#define CMD_JR6001_SELECT           "A7:"//指定文件名播放
#define CMD_JR6001_SELECT_FLASH     "A8:02"//指定FLASH内路径和文件名播放
#define CMD_JR6001_SELECT_TF        "A8:01"//指定TF卡内路径和文件名播放
#define CMD_JR6001_SELECT_UDISK     "A8:00"//指定U盘内路径和文件名播放
#define CMD_JR6001_VOLUME_SET       "AF:"//设置音量等级
#define CMD_JR6001_VOLUME_INC       "B0"//音量加
#define CMD_JR6001_VOLUME_DEC       "B1"//音量减
#define CMD_JR6001_MODE_SET         "B4:"//设置播放模式
#define CMD_JR6001_PLAY_GROUP       "B7:"//组合播放 ZH 文件夹内的文件
#define CMD_JR6001_STOP_GROUP       "B8"//组合播放 ZH 文件夹内的文件

typedef enum
{
    JR6001_CYCLE_SINGEL=1,//单曲循环
    JR6001_ONCE_SINGEL,//单曲播放一次
    JR6001_RANDOM_ALL,//全盘随机
    JR6001_CYCLE_DIR,//目录内循环
    JR6001_RANDOM_DIR,//目录内随机
    JsR6001_ORDER_ALL,//全盘顺序播放
    JsR6001_ORDER_DIR//目录内顺序播放
}PlayMode_JR6001;//播放模式


#define JR6001_SEND_CMD(n)          My_USART_SendString(UART_JR6001,n)
#define My_JR6001_Play()            JR6001_SEND_CMD(CMD_JR6001_PLAY)
#define My_JR6001_Stop()            JR6001_SEND_CMD(CMD_JR6001_STOP)
#define My_JR6001_Pause()           JR6001_SEND_CMD(CMD_JR6001_PAUSE)
#define My_JR6001_Previous()        JR6001_SEND_CMD(CMD_JR6001_PREV)
#define My_JR6001_Next()            JR6001_SEND_CMD(CMD_JR6001_NEXT)
#define My_JR6001_VolumeUp()        JR6001_SEND_CMD(CMD_JR6001_VOLUME_INC)
#define My_JR6001_VolumeDown()      JR6001_SEND_CMD(CMD_JR6001_VOLUME_DEC)
#define My_JR6001_StopGroup()       JR6001_SEND_CMD(CMD_JR6001_STOP_GROUP)

void My_JR6001_Init(u8 volume);//初始化
void My_JR6001_PlayByName(const char *name);
void My_JR6001_SetVolume(u8 level);
bool My_JR6001_IsBusy(void);
void My_JR6001_SetPlayMode(PlayMode_JR6001 mode);
void My_JR6001_PlayGroup(char *ptr);

//void My_JR6001_PlayGroup(int count, ...);

#endif
