/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�
//�汾��V1.0
//�޸�����:2020/06/30
//�����ܣ�SPI�ӿ�TFT������������
//V1.0 ��ɻ�������
************************************************/
#ifndef __MY_LCD_SPI_H
#define __MY_LCD_SPI_H
#include "my_include.h" 

#define LCD_1_4  //LCD_1_4 ����1.44���� LCD_2_4����2.4����
#define LCD_1_4_GREEN //������ʹ������ �����Ӧ����������

//#define ACS_3216                
//#define ACS_2412

typedef enum
{
    Color16_WHITE         =0xFFFF,
    Color16_BLACK         =0x0000,
    Color16_BLUE          =0x001F,
    Color16_BRED          =0XF81F,
    Color16_GRED          =0XFFE0,
    Color16_GBLUE         =0X07FF,
    Color16_RED           =0xF800,
    Color16_MAGENTA       =0xF81F,
    Color16_GREEN         =0x07E0,
    Color16_CYAN          =0x7FFF,
    Color16_YELLOW        =0xFFE0,
    Color16_BROWN         =0XBC40, //��ɫ
    Color16_BRRED         =0XFC07, //�غ�ɫ
    Color16_GRAY          =0X8430, //��ɫ
    //GUI��ɫ

    Color16_DARKBLUE      =0X01CF,//����ɫ
    Color16_LIGHTBLUE     =0X7D7C,//ǳ��ɫ  
    Color16_GRAYBLUE      =0X5458, //����ɫ
    //������ɫΪPANEL����ɫ 

    Color16_LIGHTGREEN    =0X841F, //ǳ��ɫ
    Color16_LIGHTGRAY     =0XEF5B,//ǳ��ɫ(PANNEL)
    Color16_LGRAY         =0XC618, //ǳ��ɫ(PANNEL),���屳��ɫ

    Color16_LGRAYBLUE     =0XA651, //ǳ����ɫ(�м����ɫ)
    Color16_LBBLUE        =0X2B12, //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
}Color16;

typedef struct
{
    u16 width;//LCD ���
    u16 height;//LCD �߶�
    u16 id;//LCD ID
    u8  dir;//���������������ƣ�0��������1��������
    u16 wramcmd;//��ʼдgramָ��
    u16 setxcmd;//����x����ָ��
    u16 setycmd;//����y����ָ��
    u8  xoffset;
    u8  yoffset;
}_lcd_dev;

//ʹ��PinOut(Pins_LCD_ILI9341[0])�ķ�ʽ����IO�ٶȽ�������ʹ�����´���
//�궨������Pins_LCD_ILI9341����һͬ�޸�
#define LCD_CLK     WritePin(B,0)
#define LCD_MOSI    WritePin(B,1)
#define LCD_RES     WritePin(B,2)
#define LCD_DC      WritePin(B,3)

#ifdef LCD_1_4 //����1.44����
#define LCD_CS1     WritePin(B,4)
#define LCD_LED     WritePin(B,5)
static const MyPinDef Pins_LCD_ILI9341[] = {PB0,PB1,PB2,PB3,PB4,PB5};

#define USE_HORIZONTAL      2//����Һ����˳ʱ����ת����     0-0����ת��1-90����ת��2-180����ת��3-270����ת
//����LCD�ĳߴ�
#define LCD_W 128
#define LCD_H 128
#elif defined(LCD_2_4)
#define LCD_LED     WritePin(B,11)
#define LCD_CS1     WritePin(B,13)
static const MyPinDef Pins_LCD_ILI9341[] = {PB7,PB8,PB9,PB10,PB11,PB13};

#define USE_HORIZONTAL      0//����Һ����˳ʱ����ת����     0-0����ת��1-90����ת��2-180����ת��3-270����ת
//����LCD�ĳߴ�
#define LCD_W 240
#define LCD_H 320
#endif

//////////////////////////////////////////////////////////////////////////////////
//LCD����
//TFTLCD������Ҫ���õĺ���
extern Color16 FRONT_COLOR;//Ĭ�Ϻ�ɫ
extern Color16 BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
extern _lcd_dev lcddev;//����LCD��Ҫ����


void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(Color16 color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_direction(u8 direction );
void SPIv_WriteData(u8 Data);//
void LCD_WriteData_16Bit(u16 Data);

#endif
