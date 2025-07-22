/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2020/11/17
//�����ܣ���װ�ͼ�JR6001����ģ��Ĳ���
//V1.0  2020/11/17  1����ɻ�������
************************************************/
/***********************************************
*++++++++++++++++++++++++++++++++++++++++++++++*
*ע�⣺��ģ��ָ���β�������ַ���������ָ��֮�����
        ����ʱ(�Ӿ�����Զ���)���ϵ��ʼ��Ҳ��Ҫ��ʱ
        ���⣬�����̱����Ż��ȼ�Ϊ0��ʵ��ģ������Ӧ
*++++++++++++++++++++++++++++++++++++++++++++++*
************************************************/
#include "my_jr6001.h"

void My_JR6001_Init(u8 volume)
{
    USARTx_Init(UART_JR6001,9600);//���ڳ�ʼ��
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_JR6001_BUSY,GPIO_Mode_IPD);
#else
    GPIO_Pin_Init(PIN_JR6001_BUSY,GPIO_MODE_INPUT,GPIO_PULLDOWN);
#endif
    delay_ms(800);//ģ���ϵ��ʼ����Ҫһ��ʱ�䣬�˴����Խ��Ϊ800����
    My_JR6001_SetVolume(volume);
    delay_ms(100);//
}

/*-------------------------------------- 
;��    ��:ʵ��WT588D���Ŷ�������
;��    ��:addr ������ַ���飻len���ŵ�����������timeout �ȴ���ʱʱ�䣨���룩
;��    ��:0	   ����������
;-------------------------------------*/
void My_JR6001_PlayByName(const char *name)
{
    My_USART_printf(UART_JR6001,"%s%s",CMD_JR6001_SELECT,name);
}
void My_JR6001_SetVolume(u8 level)
{
    if(level>31)//��������ȼ�31
    {
        level=31;
    }
    My_USART_printf(UART_JR6001,"%s%02d",CMD_JR6001_VOLUME_SET,level);
}

bool My_JR6001_IsBusy(void)
{
    if(PinRead(PIN_JR6001_BUSY))//��������ʱ����ߵ�ƽ
    {
        return true;
    }
    return false;
}

void My_JR6001_SetPlayMode(PlayMode_JR6001 mode)
{
    My_USART_printf(UART_JR6001,"%s%02d",CMD_JR6001_MODE_SET,mode);
}

void My_JR6001_PlayGroup(char *ptr) //����һ�鲥������ �����������ZH�ļ����� ptrֻ������ַ����
{                                   //"B7:00010203" ��ʾ��������B7��00 01 02 03��ַ����
    USARTSendString(UART_JR6001,"B7:");
    USARTSendString(UART_JR6001,ptr); 
}
 
