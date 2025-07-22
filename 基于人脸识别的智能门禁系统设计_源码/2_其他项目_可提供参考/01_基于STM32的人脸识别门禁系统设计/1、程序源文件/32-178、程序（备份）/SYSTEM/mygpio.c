/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.3
//�޸�����:2020/11/26
//�����ܣ���װ�ͼ�GPIO�Ķ�д����
//V1.0 ��ɻ�������
//V1.1  2019/12/03  ���򲿷ֹ���
//V1.2  2020/07/03  ���˳��������Ϊ��λ���㣬��߳���ִ��Ч��
//V1.3  2020/11/26  1���޸�PC13��PC14��ʼ����bug
************************************************/
#include "mygpio.h"

#define VERSION     13

void GPIO_Pin_Init(MyPinDef pin,GPIOMode_TypeDef Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;//�����ṹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA<<(pin>>4), ENABLE);//�򿪶˿�ʱ�� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
    if(pin==PB3 || pin==PB4 || pin==PA15)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��GPIO��ʱ�ӣ��ȴ򿪸��ò����޸ĸ��ù���
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//Ҫ�ȿ�ʱ�ӣ�����ӳ�䣻����ʾ�ر�jtag��ʹ��swd��
    }
    else if(pin==PA13 || pin==PA14)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��GPIO��ʱ�ӣ��ȴ򿪸��ò����޸ĸ��ù���
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//Ҫ�ȿ�ʱ�ӣ�����ӳ��
    }
    else if(pin==PC13)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        PWR_BackupAccessCmd(ENABLE);/* �����޸�RTC�ͺ󱸼Ĵ���*/
        BKP_TamperPinCmd(DISABLE);  /* PC13����������ͨIO*/
        PWR_BackupAccessCmd(DISABLE);/* ��ֹ�޸�RTC�ͺ󱸼Ĵ���*/
        //BKP_ITConfig(DISABLE);/* ��ֹTAMPER �ж�*/
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2Mʱ���ٶ�
    }
    else if(pin==PD0 || pin==PD1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_PD01,ENABLE);
    }
    GPIO_InitStructure.GPIO_Pin  = GET_PIN_GPIO(pin); //�ܽź�
    GPIO_InitStructure.GPIO_Mode = Mode;//ģʽ
    GPIO_Init(GET_PORT_GPIO(pin), &GPIO_InitStructure);//��ʼ���˿�
}
//BSRR  �ԼĴ����� 16bit д1 ��Ӧ�ܽ�Ϊ�͵�ƽ���ԼĴ�����16bitд1��Ӧ�ܽ�Ϊ�ߵ�ƽ��д 0 ,�޶���
//��GPIO��8λд������
void GPIO_WriteHigh(GPIO_TypeDef* GPIOx,u8 dat)
{
    GPIOx->BRR = 0xff00;
    GPIOx->BSRR = dat<<8;
}
//��GPIO��8λд������
void GPIO_WriteLow(GPIO_TypeDef* GPIOx,u8 dat)
{
    GPIOx->BRR = 0x00ff;
    GPIOx->BSRR = dat;
}
u16 My_GPIO_GetVersion(void)
{
    return VERSION;
}
