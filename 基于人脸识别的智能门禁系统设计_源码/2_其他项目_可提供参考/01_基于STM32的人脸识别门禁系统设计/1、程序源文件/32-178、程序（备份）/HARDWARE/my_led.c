/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.2
//�޸�����:2020/10/19
//�����ܣ���װ�ͼ�����ܽŵĲ���
//V1.0 2019/10/22 ��ɻ�������
//V1.1 2019/12/05 ɾ�����������궨��
//V1.2 2020/10/19 1�����ӳ�ʼ�����Ĭ�������ƽ
************************************************/
#include "my_led.h"

//LED IO��ʼ��
void My_LED_Init(void)
{
    u8 i;
    for(i=0;i<ArrayCount(Pins_LED);i++)
    {
#if !defined (USE_HAL_DRIVER)
        GPIO_Pin_Init(Pins_LED[i],GPIO_Mode_Out_PP);
#else
        GPIO_Pin_Init(Pins_LED[i],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
        PinOut(Pins_LED[i])=(PIN_STATE_DEF>>i)&0x01;
    }
}
 
