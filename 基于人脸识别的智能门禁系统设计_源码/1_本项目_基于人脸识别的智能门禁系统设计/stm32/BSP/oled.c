#include "oled.h"
#include "oledfont.h" 
#include "math.h" 
uint8_t OLED_GRAM[128][8];

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void OLEDIIC_Start()
{
		OLED_SCLK_Set();
		OLED_SDIN_Set();
		OLED_SDIN_Clr();
		OLED_SCLK_Clr();
}
 
/**********************************************
//IIC Stop
**********************************************/
void OLEDIIC_Stop()
{
    OLED_SCLK_Set();
		OLED_SDIN_Clr();
		OLED_SDIN_Set();
	
}
 
void OLEDIIC_Wait_Ack()
{
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/
 
void OLEDWrite_IIC_Byte(uint8_t IIC_Byte)
{
		uint8_t i;
		uint8_t m,da;
		da=IIC_Byte;
		OLED_SCLK_Clr();
		for(i=0;i<8;i++)		
		{
			m=da;
			m=m&0x80;
			if(m==0x80)
					OLED_SDIN_Set();
			else 
					OLED_SDIN_Clr();
			da=da<<1;
			OLED_SCLK_Set();
			OLED_SCLK_Clr();
		}
}
/**********************************************
// IIC Write Command
**********************************************/
void OLEDWrite_IIC_Command(uint8_t IIC_Command)
{
		 OLEDIIC_Start();
		 OLEDWrite_IIC_Byte(0x78);            //Slave address,SA0=0
		 OLEDIIC_Wait_Ack();	
		 OLEDWrite_IIC_Byte(0x00);			//write command
		 OLEDIIC_Wait_Ack();	
		 OLEDWrite_IIC_Byte(IIC_Command); 
		 OLEDIIC_Wait_Ack();	
		 OLEDIIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void OLEDWrite_IIC_Data(uint8_t IIC_Data)
{
		 OLEDIIC_Start();
		 OLEDWrite_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		 OLEDIIC_Wait_Ack();	
		 OLEDWrite_IIC_Byte(0x40);			//write data
		 OLEDIIC_Wait_Ack();	
		OLEDWrite_IIC_Byte(IIC_Data);
		 OLEDIIC_Wait_Ack();	
		 OLEDIIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
		if(cmd)
		{
				OLEDWrite_IIC_Data(dat);  
		}
		else 
		{
				OLEDWrite_IIC_Command(dat);		
		}
}
 
 
/********************************************
// fill_Picture
********************************************/
void fill_picture(uint8_t fill_Data)
{
		uint8_t m,n;
		for(m=0;m<8;m++)
		{
			OLED_WR_Byte(0xb0+m,0);		//page0-page1
			OLED_WR_Byte(0x02,0);		//low column start address
			OLED_WR_Byte(0x10,0);		//high column start address
			for(n=0;n<128;n++)
			{
					OLED_WR_Byte(fill_Data,1);
			}
		}
}
 
 
 
//坐标设置 
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
		OLED_WR_Byte(0xb0+y,OLED_CMD);
		OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  
 //更新显存到OLED
void OLED_Refresh(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
        for(n=0; n<128; n++)
            OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }
}
//清屏函数
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        for(n=0; n<128; n++)
        {
            OLED_GRAM[n][i]=0;//清除所有数据
        }
    }
    OLED_Refresh();//更新显示
}
//清屏函数
void OLED_ClearPart(void)
{
    uint8_t i,n;
    for(i=0; i<6; i++)
    {
        for(n=0; n<128; n++)
        {
            OLED_GRAM[n][i]=0;//清除所有数据
        }
    }
    OLED_Refresh();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)     //用此函数后需要将点更新到LCD，，即OLED_Refresh_Gram();
{
		uint8_t pos,bx,temp=0;
		if(x>127||y>63)
			return;//超出范围了.
		pos = 7-y/8;            //第几页  其实是反过来，然后       然后会翻转
		bx = y%8;
		temp = 1<<(7-bx);       //这里使这个 1 与具体的y相匹配
		if(t)
			OLED_GRAM[x][pos] |= temp;
		else 
			OLED_GRAM[x][pos] &= ~temp;	
}
 
//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
		uint8_t i,n;		    
		for(i=0;i<8;i++)  
		{  
				OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
				OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
				OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
				for(n=0;n<128;n++)
					OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); //将数据写入O_LED的GRAM中
		}   
}
 
//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;//画线起点坐标
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if (delta_x==0)incx=0;//垂直线
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平线
    else {
        incy=-1;
        delta_y=-delta_x;
    }
    if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0; t<distance+1; t++)
    {
        OLED_DrawPoint(uRow,uCol,mode);//画点
        xerr+=delta_x;
        yerr+=delta_y;
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
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
    int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);

        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);

        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
    uint8_t i,m,temp,size2,chr1;
    uint8_t x0=x,y0=y;
    if(size1==8)size2=6;
    else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0; i<size2; i++)
    {
        if(size1==8)
        {
            temp=asc2_0806[chr1][i];   //调用0806字体
        }
        else if(size1==12)
        {
            temp=asc2_1206[chr1][i];   //调用1206字体
        }
        else if(size1==16)
        {
            temp=asc2_1608[chr1][i];   //调用1608字体
        }
        else if(size1==24)
        {
            temp=asc2_2412[chr1][i];   //调用2412字体
        }
        else return;
        for(m=0; m<8; m++)
        {
            if(temp&0x01)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((size1!=8)&&((x-x0)==size1/2))
        {
            x=x0;
            y0=y0+8;
        }
        y=y0;
    }
}

#define hz16_num   45

//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
//mode:0,反色显示;1,正常显示
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode)
{
	uint8_t x0=x;
    while((*chr>=' ')&&(*chr<='~') ||*chr==0x0a|| *chr==0x0d)//判断是不是非法字符!
    {
			if(*chr==0x0a)
			{
				y+=8;
				x=x0;
			}
			else if(*chr==0x0d)
			{
			}
			else
			{
        OLED_ShowChar(x,y,*chr,size1,mode);
        if(size1==8)x+=6;
        else x+=size1/2;
        
			}
			chr++;
    }
}
void OLED_ShowStringCN(uint8_t x,uint8_t y,uint8_t *chr,uint8_t mode)
{   unsigned short k,x0;
    unsigned char i,j;
    while(*chr) {
        if((*chr) < 128)
        {
            OLED_ShowChar(x,y,*chr,16,mode);
            x+=8;
            chr++;
        } else {

            for (k=0; k<hz16_num; k++)
            {
                if ((hz16[k].Index[0]==*(chr))&&(hz16[k].Index[1]==*(chr+1)))
                {
                    for(i=0; i<16; i++)
                    {
                        for(j=0; j<8; j++)
                        {
                            if(hz16[k].Msk[i*2]&(0x80>>j))	OLED_DrawPoint(x+j,y+i,mode);
                            else {
                                OLED_DrawPoint(x+j,y+i,!mode);;
                            }
                        }
                        for(j=0; j<8; j++)
                        {
                            if(hz16[k].Msk[i*2+1]&(0x80>>j))	OLED_DrawPoint(x+j+8,y+i,mode);
                            else
                            {
                                OLED_DrawPoint(x+j+8,y+i,!mode);;
                            }
                        }
                    }
                }
            }
            chr+=2;
            x+=16;
        }

    }
}
void OLED_ShowStringCN1(uint8_t x,uint8_t y,uint8_t *chr,uint8_t mode)
{   unsigned short k,x0;
    unsigned char i,j;
    while(*chr) {
        if((*chr) < 128)
        {
					OLED_ShowChar(x,y,' ',16,mode);
            OLED_ShowChar(x,y+4,*chr,8,mode);
            x+=8;
            chr++;
        } else {

            for (k=0; k<hz16_num; k++)
            {
                if ((hz16[k].Index[0]==*(chr))&&(hz16[k].Index[1]==*(chr+1)))
                {
                    for(i=0; i<16; i++)
                    {
                        for(j=0; j<8; j++)
                        {
                            if(hz16[k].Msk[i*2]&(0x80>>j))	OLED_DrawPoint(x+j,y+i,mode);
                            else {
                                OLED_DrawPoint(x+j,y+i,!mode);;
                            }
                        }
                        for(j=0; j<8; j++)
                        {
                            if(hz16[k].Msk[i*2+1]&(0x80>>j))	OLED_DrawPoint(x+j+8,y+i,mode);
                            else
                            {
                                OLED_DrawPoint(x+j+8,y+i,!mode);;
                            }
                        }
                    }
                }
            }
            chr+=2;
            x+=16;
        }

    }
}
//m^n
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
    {
        result*=m;
    }
    return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
    uint8_t t,temp,m=0;
    if(size1==8)m=2;
    for(t=0; t<len; t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(temp==0)
        {
            OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
        }
        else
        {
            OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
        }
    }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode)
{
    uint8_t m,temp;
    uint8_t x0=x,y0=y;
    uint16_t i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
    for(i=0; i<size3; i++)
    {
        if(size1==16)
        {
            temp=Hzk1[num][i];   //调用16*16字体
        }
        else if(size1==24)
        {
            temp=Hzk2[num][i];   //调用24*24字体
        }
        else if(size1==32)
        {
            temp=Hzk3[num][i];   //调用32*32字体
        }
        else if(size1==64)
        {
            temp=Hzk4[num][i];   //调用64*64字体
        }
        else return;
        for(m=0; m<8; m++)
        {
            if(temp&0x01)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((x-x0)==size1)
        {
            x=x0;
            y0=y0+8;
        }
        y=y0;
    }
}

//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode)
{
    uint8_t i,n,t=0,m=0,r;
    while(1)
    {
        if(m==0)
        {
            OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
            t++;
        }
        if(t==num)
        {
            for(r=0; r<16*space; r++)    //显示间隔
            {
                for(i=1; i<144; i++)
                {
                    for(n=0; n<8; n++)
                    {
                        OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh();
            }
            t=0;
        }
        m++;
        if(m==16) {
            m=0;
        }
        for(i=1; i<144; i++) //实现左移
        {
            for(n=0; n<8; n++)
            {
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
    uint16_t j=0;
    uint8_t i,n,temp,m;
    uint8_t x0=x,y0=y;
    sizey=sizey/8+((sizey%8)?1:0);
    for(n=0; n<sizey; n++)
    {
        for(i=0; i<sizex; i++)
        {
            temp=BMP[j];
            j++;
            for(m=0; m<8; m++)
            {
                if(temp&0x01)OLED_DrawPoint(x,y,mode);
                else OLED_DrawPoint(x,y,!mode);
                temp>>=1;
                y++;
            }
            x++;
            if((x-x0)==sizex)
            {
                x=x0;
                y0=y0+8;
            }
            y=y0;
        }
    }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
	 uint16_t j=0;
	 uint8_t x,y;
  
  if(y1%8==0) 
		y=y1/8;      
  else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	  {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	  }
	}
} 
 
//初始化SSD1306					    
void OLED_Init(void)
{ 	
		GPIO_InitTypeDef GPIO_InitStruct = {0};
	 
		__HAL_RCC_GPIOB_CLK_ENABLE();
	 
		HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SCLK_Pin|OLED_SDIN_Pin, GPIO_PIN_RESET);//设置SCL，SDA
		/*Configure GPIO pins : PB10 PB11 */
		GPIO_InitStruct.Pin = OLED_SCLK_Pin|OLED_SDIN_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(OLED_SDIN_GPIO_Port, &GPIO_InitStruct);
		HAL_Delay(500);
		OLED_WR_Byte(0xAE,OLED_CMD);//--display off
		OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
		OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
		OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
		OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
		OLED_WR_Byte(0x81,OLED_CMD); // contract control
		OLED_WR_Byte(0xFF,OLED_CMD);//--128   
		OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
		OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
		OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
		OLED_WR_Byte(0xC0,OLED_CMD);//Com scan direction//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
		OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
		OLED_WR_Byte(0x00,OLED_CMD);//
		
		OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
		OLED_WR_Byte(0x80,OLED_CMD);//
		
		OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
		OLED_WR_Byte(0x05,OLED_CMD);//
		
		OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
		OLED_WR_Byte(0xF1,OLED_CMD);//
		
		OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
		OLED_WR_Byte(0x12,OLED_CMD);//
		
		OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
		OLED_WR_Byte(0x30,OLED_CMD);//
		
		OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
		OLED_WR_Byte(0x14,OLED_CMD);//
		
		OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}   
