/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：
//版本：V1.0
//修改日期:2020/06/30
//程序功能：SPI接口TFT彩屏驱动程序
//V1.0 完成基本功能
************************************************/
#include "my_lcd_spi.h"

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
Color16 FRONT_COLOR,BACK_COLOR;
u16 DeviceCode;


void  SPIv_WriteData(u8 Data)
{
    u8 i=0;
    for(i=8;i>0;i--)
    {
        if(Data&0x80)
        {
            LCD_MOSI = 1;//输出数据
        }
        else
        {
            LCD_MOSI = 0;
        }
        LCD_CLK = 0;
        LCD_CLK = 1;
        Data<<=1;
    }
}
/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{
    LCD_CS1 = 0;
    LCD_DC = 0;
    SPIv_WriteData(data);
    LCD_CS1 = 1;
}
/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
    LCD_CS1 = 0;
    LCD_DC = 1;
    SPIv_WriteData(data);
    LCD_CS1 = 1;
}
/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}
/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}
/*****************************************************************************
 * @name       :void LCD_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteData_16Bit(u16 Data)
{
    LCD_CS1 = 0;
    LCD_DC = 1;
    SPIv_WriteData(Data>>8);
    SPIv_WriteData(Data);
    LCD_CS1 = 1;
}
/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x,y);//设置光标位置 
    LCD_WriteData_16Bit(FRONT_COLOR);
}
/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color16)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(Color16 color)
{
    u16 i,m;
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
    for(i=0;i<lcddev.height;i++)
    {
        for(m=0;m<lcddev.width;m++)
        {
            LCD_WriteData_16Bit(color);
        }
    }
}
/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
    u8 i;
    for(i=0;i<sizeof(Pins_LCD_ILI9341)/sizeof(MyPinDef);i++)
    {
#if !defined (USE_HAL_DRIVER)
        GPIO_Pin_Init(Pins_LCD_ILI9341[i],GPIO_Mode_Out_PP);
#else
        GPIO_Pin_Init(Pins_LCD_ILI9341[i],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
    }
}
/*****************************************************************************
 * @name       :void LCD_DCET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_DCET(void)
{
    LCD_RES = 0;
    delay_ms(100);
    LCD_RES = 1;
    delay_ms(50);
}
/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
                Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);
} 

/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
                yStar:the bebinning y coordinate of the LCD display window
                xEnd:the endning x coordinate of the LCD display window
                yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar>>8);
    LCD_WR_DATA(xStar+lcddev.xoffset);
    LCD_WR_DATA(xEnd>>8);
    LCD_WR_DATA(xEnd+lcddev.xoffset);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar>>8);
    LCD_WR_DATA(yStar+lcddev.yoffset);
    LCD_WR_DATA(yEnd>>8);
    LCD_WR_DATA(yEnd+lcddev.yoffset);
    LCD_WriteRAM_Prepare();	//开始写入GRAM
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
                          2-180 degree
                          3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
    lcddev.setxcmd=0x2A;
    lcddev.setycmd=0x2B;
    lcddev.wramcmd=0x2C;

#ifdef LCD_1_4_GREEN
        switch(direction){
            case 0:
                lcddev.width=LCD_W;
                lcddev.height=LCD_H;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=0;
                lcddev.yoffset=32;
    #endif
                LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
                break;
            case 1:
                lcddev.width=LCD_H;
                lcddev.height=LCD_W;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=32;
                lcddev.yoffset=0;
                LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
    #endif
    //            LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
                break;
            case 2:
                lcddev.width=LCD_W;
                lcddev.height=LCD_H;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=0;
                lcddev.yoffset=0;
    #endif
                LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
                break;
            case 3:
                lcddev.width=LCD_H;
                lcddev.height=LCD_W;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=0;
                lcddev.yoffset=0;
    #endif
                LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
                break;
            default:break;
        }
#else    
        switch(direction){
            case 0:
                lcddev.width=LCD_W;
                lcddev.height=LCD_H;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=2;
                lcddev.yoffset=3;
    #endif
                LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
                break;
            case 1:
                lcddev.width=LCD_H;
                lcddev.height=LCD_W;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=3;
                lcddev.yoffset=2;
                LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
    #endif
                LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
                break;
            case 2:
                lcddev.width=LCD_W;
                lcddev.height=LCD_H;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=2;
                lcddev.yoffset=1;
    #endif
                LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
                break;
            case 3:
                lcddev.width=LCD_H;
                lcddev.height=LCD_W;
    #ifdef LCD_1_4 //采用1.44寸屏
                lcddev.xoffset=1;
                lcddev.yoffset=2;
    #endif
                LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
                break;
            default:break;
        }
#endif
}
/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_Init(void)
{
    LCD_GPIOInit();
    LCD_DCET();//LCD 复位
#ifdef LCD_1_4 //采用1.44寸屏
    //************* ST7735S初始化**********//
    LCD_WR_REG(0x11);//Sleep exit 
    delay_ms (120);	
//ST7735R Frame Rate
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_REG(0xB2);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_REG(0xB3);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_REG(0xB4);//Column inversion 
    LCD_WR_DATA(0x07);
//ST7735R Power Sequence
    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0xA2);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x84);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0xC5);
    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xC3);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x2A);
    LCD_WR_REG(0xC4);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0xEE);
    LCD_WR_REG(0xC5);//VCOM 
    LCD_WR_DATA(0x0E);
    LCD_WR_REG(0x36);//MX, MY, RGB mode 
    LCD_WR_DATA(0xC8);
//ST7735R Gamma Sequence
    LCD_WR_REG(0xe0);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x1a);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x2f);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x1f);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x23);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x10);
    LCD_WR_REG(0xe1);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x2c);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x2e);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x3f);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x10);
    LCD_WR_REG(0x2a);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x7f);
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x9f);
    LCD_WR_REG(0xF0);//Enable test command  
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xF6);//Disable ram power save mode 
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x3A);//65k mode 
    LCD_WR_DATA(0x05);
#else//选择2.4寸屏
//*************2.4inch ILI9341初始化**********//
    LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD9);//0xC1 
    LCD_WR_DATA(0X30);
    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0X12);
    LCD_WR_DATA(0X81);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x85);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x7A);
    LCD_WR_REG(0xCB);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xF7);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xEA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xC0);//Power control 
    LCD_WR_DATA(0x1B);//VRH[5:0] 
    LCD_WR_REG(0xC1);//Power control 
    LCD_WR_DATA(0x12);//SAP[2:0];BT[3:0] 0x01
    LCD_WR_REG(0xC5);//VCM control 
    LCD_WR_DATA(0x08);//30
    LCD_WR_DATA(0x26);//30
    LCD_WR_REG(0xC7);//VCM control2 
    LCD_WR_DATA(0XB7);
    LCD_WR_REG(0x36);// Memory Access Control 
    LCD_WR_DATA(0x08);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1A);
    LCD_WR_REG(0xB6);// Display Function Control 
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0xA2);
    LCD_WR_REG(0xF2);// 3Gamma Function Disable 
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x26);//Gamma curve selected 
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xE0);//Set Gamma 
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x1A);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x49);
    LCD_WR_DATA(0X66);
    LCD_WR_DATA(0x3B);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0XE1);//Set Gamma 
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x4C);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x2E);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x05);
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x3f);
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xef);
    LCD_WR_REG(0x11);//Exit Sleep
    delay_ms(120);
#endif
    LCD_WR_REG(0x29);//display on
    LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
    LCD_LED = 1;//点亮背光
//LCD_Clear(WHITE);//清全屏白色
}

