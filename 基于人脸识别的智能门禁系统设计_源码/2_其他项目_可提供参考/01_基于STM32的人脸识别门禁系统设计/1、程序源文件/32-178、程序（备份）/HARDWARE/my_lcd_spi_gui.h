/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：
//版本：V1.0
//修改日期:2020/06/30
//程序功能：TFT彩屏显示方法
//V1.0 完成基本功能
************************************************/
#ifndef __MY_LCD_SPI_GUI_H
#define __MY_LCD_SPI_GUI_H
#include "my_include.h"


void GUI_DrawPoint(u16 x,u16 y,u16 color);
void GUI_Drawbmp16(u16 x,u16 y,const unsigned char *p);
void LCD_DrawPoint_big(u16 x,u16 y);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle_Fill(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawCircle(int xc, int yc,int r, int fill);
void LCD_DrawTriangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_DrawTriangel_Fill(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_DrawMatrixCode(u16 x, u16 y, u8 width,u8 high,u8 *charPtr,bool overlap);
void LCD_ShowChinese(u16 x, u16 y, u8 *s, u8 charSize,bool overlap);
void LCD_ShowChar(u16 x,u16 y, u8 num,u8 charSize,bool overlap);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 charSize);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 charSize,bool overlap);

void LCD_JustString(u16 x,u16 y,char *p,u8 charSize);
void LCD_ShowString(u16 x, u16 y, char *str,u8 charSize,bool overlap);
void LCD_StrCenter(u16 x, u16 y, char *str,u8 charSize,bool overlap);


u16 LCD_GetPos_X(u8 charSize,u8 index);
u16 LCD_GetPos_Y(u8 charSize,u8 index);
#endif

