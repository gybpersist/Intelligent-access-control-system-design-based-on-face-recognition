/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.2
//修改日期:2020/10/19
//程序功能：封装和简化输出管脚的操作
//V1.0 2019/10/22 完成基本功能
//V1.1 2019/12/05 删除按键个数宏定义
//V1.2 2020/10/19 1、增加初始化后的默认输出电平
************************************************/
#include "my_led.h"

//LED IO初始化
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
 
