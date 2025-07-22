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
#include "my_jr6001.h"

void My_JR6001_Init(u8 volume)
{
    USARTx_Init(UART_JR6001,9600);//串口初始化
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_JR6001_BUSY,GPIO_Mode_IPD);
#else
    GPIO_Pin_Init(PIN_JR6001_BUSY,GPIO_MODE_INPUT,GPIO_PULLDOWN);
#endif
    delay_ms(800);//模块上电初始化需要一定时间，此处测试结果为800毫秒
    My_JR6001_SetVolume(volume);
    delay_ms(100);//
}

/*-------------------------------------- 
;功    能:实现WT588D播放多组语音
;入    参:addr 语音地址数组；len播放的语音个数；timeout 等待超时时间（毫秒）
;出    参:0	   返回主函数
;-------------------------------------*/
void My_JR6001_PlayByName(const char *name)
{
    My_USART_printf(UART_JR6001,"%s%s",CMD_JR6001_SELECT,name);
}
void My_JR6001_SetVolume(u8 level)
{
    if(level>31)//最大音量等级31
    {
        level=31;
    }
    My_USART_printf(UART_JR6001,"%s%02d",CMD_JR6001_VOLUME_SET,level);
}

bool My_JR6001_IsBusy(void)
{
    if(PinRead(PIN_JR6001_BUSY))//播放语音时输出高电平
    {
        return true;
    }
    return false;
}

void My_JR6001_SetPlayMode(PlayMode_JR6001 mode)
{
    My_USART_printf(UART_JR6001,"%s%02d",CMD_JR6001_MODE_SET,mode);
}

void My_JR6001_PlayGroup(char *ptr) //发送一组播报命令 语音必须放在ZH文件夹中 ptr只包函地址部分
{                                   //"B7:00010203" 表示连续播报B7中00 01 02 03地址语音
    USARTSendString(UART_JR6001,"B7:");
    USARTSendString(UART_JR6001,ptr); 
}
 
