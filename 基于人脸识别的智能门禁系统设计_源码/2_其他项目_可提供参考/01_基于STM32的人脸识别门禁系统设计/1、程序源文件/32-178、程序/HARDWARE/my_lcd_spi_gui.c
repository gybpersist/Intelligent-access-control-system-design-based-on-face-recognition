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
#include "my_lcd_spi_gui.h"
#include "my_lcd_spi_font.h" 

/*******************************************************************
 * @name       :void GUI_DrawPoint(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :draw a point in LCD screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
                color:the color value of the point
 * @retvalue   :None
********************************************************************/
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
    LCD_SetCursor(x,y);//设置光标位置
    LCD_WriteData_16Bit(color); 
}

/*****************************************************************************
 * @name       :void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :Draw a big point(2*2)
 * @parameters :x:Read x coordinate of the point
                y:Read y coordinate of the point
                color:the color value of the point
 * @retvalue   :None
******************************************************************************/   
void LCD_DrawPoint_big(u16 x,u16 y)
{
    LCD_DrawPoint(x,y);//中心点 
    LCD_DrawPoint(x+1,y);
    LCD_DrawPoint(x,y+1);
    LCD_DrawPoint(x+1,y+1);
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
填充一个矩形
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
                ex:the ending x coordinate of the specified area
                ey:the ending y coordinate of the specified area
                color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	
    u16 i,j;
    u16 width=ex-sx+1;//得到填充的宽度
    u16 height=ey-sy+1;//高度
    LCD_SetWindows(sx,sy,ex,ey);//设置显示窗口
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        LCD_WriteData_16Bit(color);//写入数据
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}

/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
绘制一条线
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
                x2:the ending x coordinate of the line
                y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t; 
    int xerr=0,yerr=0,delta_x,delta_y,distance; 
    int incx,incy,uRow,uCol; 

    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1; 
    uRow=x1; 
    uCol=y1; 
    if(delta_x>0)incx=1; //设置单步方向 
    else if(delta_x==0)incx=0;//垂直线 
    else {incx=-1;delta_x=-delta_x;} 
    if(delta_y>0)incy=1; 
    else if(delta_y==0)incy=0;//水平线 
    else{incy=-1;delta_y=-delta_y;} 
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
    else distance=delta_y; 
    for(t=0;t<=distance+1;t++ )//画线输出 
    {
        LCD_DrawPoint(uRow,uCol);//画点 
        xerr+=delta_x ; 
        yerr+=delta_y ; 
        if(xerr>distance) 
        { 
            xerr-=distance; 
            uRow+=incx; 
        } 
        if(yerr>distance) 
        { 
            yerr-=distance; 
            uCol+=incy; 
        }
    }
}

/*****************************************************************************
 * @name       :void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
绘制一个矩形框
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
                x2:the ending x coordinate of the rectangle
                y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}  

/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
填充一个矩形
 * @function   :Filled a rectangle
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
                x2:the ending x coordinate of the filled rectangle
                y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/  
void LCD_DrawRectangle_Fill(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_Fill(x1,y1,x2,y2,FRONT_COLOR);
}
 
/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
圆绘制方法
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
                x:the x coordinate relative to the Circular center 
                y:the y coordinate relative to the Circular center 
                c:the color value of the circle
 * @retvalue   :None
******************************************************************************/  
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
    GUI_DrawPoint(xc + x, yc + y, c);
    GUI_DrawPoint(xc - x, yc + y, c);
    GUI_DrawPoint(xc + x, yc - y, c);
    GUI_DrawPoint(xc - x, yc - y, c);
    GUI_DrawPoint(xc + y, yc + x, c);
    GUI_DrawPoint(xc - y, yc + x, c);
    GUI_DrawPoint(xc + y, yc - x, c);
    GUI_DrawPoint(xc - y, yc - x, c);
}

/*****************************************************************************
 * @name       :void gui_circle(int xc, int yc,u16 c,int r, int fill)
绘制一个圆 实心或空心
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
                r:Circular radius
                fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/
void LCD_DrawCircle(int xc, int yc,int r, int fill)
{
    int x = 0, y = r, yi, d;
    d = 3 - 2 * r;
    if (fill) 
    {
        // 如果填充（画实心圆）
        while (x <= y) 
        {
            for (yi = x; yi <= y; yi++)
            {
                _draw_circle_8(xc, yc, x, yi, FRONT_COLOR);
            }
            if (d < 0) 
            {
                d = d + 4 * x + 6;
            } 
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
    else 
    {
        // 如果不填充（画空心圆）
        while (x <= y) 
        {
            _draw_circle_8(xc, yc, x, y, FRONT_COLOR);
            if (d < 0) 
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

/*****************************************************************************
 * @name       :void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
绘制一个三角形框
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
                x1:the vertex x coordinate of the triangular
                y1:the vertex y coordinate of the triangular
                x2:the ending x coordinate of the triangular edge 
                y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void LCD_DrawTriangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
    LCD_DrawLine(x0,y0,x1,y1);
    LCD_DrawLine(x1,y1,x2,y2);
    LCD_DrawLine(x2,y2,x0,y0);
}

static void _swap(u16 *a, u16 *b)
{
    u16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/*****************************************************************************
 * @name       :void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
绘制一个三角形框填充
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
                x1:the vertex x coordinate of the triangular
                y1:the vertex y coordinate of the triangular
                x2:the ending x coordinate of the triangular edge 
                y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void LCD_DrawTriangel_Fill(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
    u16 a, b, y, last;
    int dx01, dy01, dx02, dy02, dx12, dy12;
    long sa = 0;
    long sb = 0;
    if (y0 > y1) 
    {
        _swap(&y0,&y1); 
        _swap(&x0,&x1);
    }
    if (y1 > y2) 
    {
        _swap(&y2,&y1); 
        _swap(&x2,&x1);
    }
    if (y0 > y1) 
    {
        _swap(&y0,&y1); 
        _swap(&x0,&x1);
    }
    if(y0 == y2) 
    { 
        a = b = x0;
        if(x1 < a)
        {
            a = x1;
        }
        else if(x1 > b)
        {
            b = x1;
        }
        if(x2 < a)
        {
            a = x2;
        }
        else if(x2 > b)
        {
            b = x2;
        }
        LCD_Fill(a,y0,b,y0,FRONT_COLOR);
        return;
    }
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;

    if(y1 == y2)
    {
        last = y1; 
    }
    else
    {
        last = y1-1; 
    }
    for(y=y0; y<=last; y++) 
    {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if(a > b)
        {
            _swap(&a,&b);
        }
        LCD_Fill(a,y,b,y,FRONT_COLOR);
    }
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) 
    {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if(a > b)
        {
            _swap(&a,&b);
        }
        LCD_Fill(a,y,b,y,FRONT_COLOR);
    }
}
 
/*****************************************************************************
 * @name       :void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
                p:the start address of image array
 * @retvalue   :None
******************************************************************************/ 
void GUI_Drawbmp16(u16 x,u16 y,const u8 *p) //显示40*40 QQ图片
{
    int i; 
    unsigned char picH,picL; 
    LCD_SetWindows(x,y,x+40-1,y+40-1);//窗口设置
    for(i=0;i<40*40;i++)
    {
        picL=*(p+i*2);//数据低位在前
        picH=*(p+i*2+1);
        LCD_WriteData_16Bit(picH<<8|picL);
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏
}


/*****************************************************************************
 * @name       :void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
显示遗传英文字符 只限英文
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
                p:the start address of the English string
                size:the size of display character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_JustString(u16 x,u16 y,char *p,u8 size)
{
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {   
        if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
        return;     
        LCD_ShowChar(x,y,*p,size,false);
        x+=size/2;
        p++;
    }
}

/*****************************************************************************
 * @name       :u32 mypow(u8 m,u8 n)
 * @date       :2018-08-09 
 * @function   :get the nth power of m (internal call)
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
******************************************************************************/ 
u32 mypow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}

/*****************************************************************************
 * @name       :void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
 * @date       :2018-08-09 
 * @function   :Display number
 * @parameters :x:the bebinning x coordinate of the number
                y:the bebinning y coordinate of the number
                num:the number(0~4294967295)
                len:the length of the display number
                size:the size of display number
 * @retvalue   :None
******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,false);
                continue;
            }else enshow=1; 
             
        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,false); 
    }
}
//计算汉字字模的大小（字节数）
u8 My_Font_GetCodeSize_CH(u8 charSize)
{
    return charSize*(charSize/8 + (charSize%8?1:0));
}
//计算ASCII字模的大小（字节数）
u8 My_Font_GetCodeSize_ASCII(u8 charSize)
{
    charSize /= 2;
    return charSize*(charSize/8 + (charSize%8?1:0))*2;
}
void LCD_ScanLine_Byte(u16 x, u16 y, u8 *charPtr,u8 pointCount,bool overlap)
{
    u8 i;
    if(pointCount>8)
    {
        pointCount = 8;
    }
    for(i=0;i<pointCount;i++)
    {
//        if((*charPtr)&(0x01<<i))//逆向扫描
        if(((*charPtr)&(0x80>>i)))//顺向扫描
        {
            if(overlap)
            {
                LCD_DrawPoint(x,y);//画一个点
            }
            else
            {
                LCD_WriteData_16Bit(FRONT_COLOR); 
            }
        }
        else
        {
            if(!overlap)
            {
                LCD_WriteData_16Bit(BACK_COLOR); 
            }
        }
        x++;
    }
}
//显示字符或图片取模后的数据，如自定义特殊字符
void LCD_DrawMatrixCode(u16 x, u16 y, u8 width,u8 high,u8 *charPtr,bool overlap)
{
    u8 i,scanPointCount;
    u16 x0 = x;
    LCD_SetWindows(x,y,x+width-1,y+high-1);
    for(i=0;i<high;i++)//逐行扫描
    {
        scanPointCount = width;//计算出每一行的点数
        while(scanPointCount>0)//如果这一行的点数大于7
        {
            if(scanPointCount>7)
            {
                LCD_ScanLine_Byte(x,y,charPtr,8,overlap);
                scanPointCount -= 8;
                charPtr++;
                x += 8;
            }
            else
            {
                LCD_ScanLine_Byte(x,y,charPtr,scanPointCount,overlap);
                charPtr++;
                break;
            }
        }
        x = x0;
        y++;
    }
}
/*****************************************************************************
 * @name       :void LCD_ShowChinese(u16 x, u16 y, u8 *s, u8 charSize,u8 mode)
 * @date       :2018-08-09 
 * @function   :Display a single Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                s:the start address of the Chinese character
                charSize:size of the Chinese character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void LCD_ShowChinese(u16 x, u16 y, u8 *s, u8 charSize,bool overlap)
{
    u8 fontCodeSize,fontCharCount,*fontCodePtr;
    u16 k;
    if(x>(lcddev.width-charSize)||y>(lcddev.height-charSize)) 
    return;
    fontCodeSize = My_Font_GetCodeSize_CH(charSize)+2;
    switch(charSize)
    {
        case 12:fontCharCount = ArrayCount(fontGBK12);fontCodePtr = (u8 *)fontGBK12;break;        
        case 16:fontCharCount = ArrayCount(tfont16);fontCodePtr = (u8 *)tfont16;break;
        case 24:fontCharCount = ArrayCount(tfont24);fontCodePtr = (u8 *)tfont24;break;
        case 32:fontCharCount = ArrayCount(tfont32);fontCodePtr = (u8 *)tfont32;break;
        default:break;
    }
    for (k=0;k<fontCharCount;k++)//遍历每个字模
    {
        if ((*(fontCodePtr)==*(s))&&(*(fontCodePtr+1)==*(s+1)))//对比字符编码
        {
            fontCodePtr+=2;
            LCD_DrawMatrixCode(x,y,charSize,charSize,fontCodePtr,overlap);
            break;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
        fontCodePtr += fontCodeSize;//指向下一个字模的地址
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
}
/*****************************************************************************
 * @name       :void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
                                num:the ascii code of display character(0~94)
                                size:the size of display character
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y, u8 charCode,u8 charSize,bool overlap)
{
    u8 *matrixPtr;
    if(x>(lcddev.width-charSize/2)||y>(lcddev.height-charSize)) 
    return; 
    charCode=charCode-' ';//得到偏移后的值
    LCD_SetWindows(x,y,x+charSize/2-1,y+charSize-1);//设置单个文字显示窗口
    switch(charSize)
    {
        case 12:matrixPtr=(u8 *)asc2_1206;break;
        case 16:matrixPtr=(u8 *)asc2_1608;break;
#ifdef ACS_2412        
        case 24:matrixPtr=(u8 *)asc2_2412;break;
#endif 
        
#ifdef ACS_3216
        case 32:matrixPtr=(u8 *)asc2_3216;break;
#endif
        default:break;
    }
    matrixPtr += charCode*My_Font_GetCodeSize_ASCII(charSize);
    LCD_DrawMatrixCode(x,y,charSize/2,charSize,matrixPtr,overlap);
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏
}

/*****************************************************************************
 * @name       :void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
显示一个字符串 可以为中文 显示的中文取模必须在放到lcd_font.h中
 * @function   :Display Chinese and English strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
                                str:the start address of the Chinese and English strings
                                size:the size of Chinese and English strings
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowString(u16 x, u16 y, char *str,u8 charSize,bool overlap)
{
    u16 x0=x;
    if(charSize>32)//
    {
        charSize = 32;
    }
    while(*str!=0)//数据未结束
    {
        if((u8)(*str)<0x80)
        {
            if(*str=='\r')//回车符号
            {
                y+=charSize;
                x=x0;
                if(*(str+1)=='\n')//换行符号
                {
                    str++;
                }
            }
            else
            {
                LCD_ShowChar(x,y,*str,charSize,overlap);
                x+=charSize/2; //字符,为全字的一半
            }
            str++;
        }
        else//中文
        {
            LCD_ShowChinese(x,y,(u8 *)str,charSize,overlap);
            str+=2; 
            x+=charSize;//下一个汉字偏移
        }
    }
}

/*****************************************************************************
 * @name       :void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
                str:the start address of the Chinese and English strings
                size:the size of Chinese and English strings
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void LCD_StrCenter(u16 x, u16 y, char *str,u8 charSize,bool overlap)
{
    u16 len=strlen((const char *)str);
    u16 x1=(lcddev.width-len*8)/2;
    LCD_ShowString(x1,y,str,charSize,overlap);
} 

u16 LCD_GetPos_X(u8 charSize,u8 index)//
{
    if(index<=lcddev.width/(charSize>>1))
    {
        return index*(charSize>>1);
    }
    return 0;
}
u16 LCD_GetPos_Y(u8 charSize,u8 index)//
{
    if(index<=lcddev.height/(charSize))
    {
        return index*(charSize);
    }
    return 0;
}

