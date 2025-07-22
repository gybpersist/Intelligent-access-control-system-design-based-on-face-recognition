#ifndef __OLED_H
#define __OLED_H	
#include "gpio.h"
 #include "main.h"
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    		
 #define OLED_SCLK_Pin GPIO_PIN_0
#define OLED_SCLK_GPIO_Port GPIOA
#define OLED_SDIN_Pin GPIO_PIN_1
#define OLED_SDIN_GPIO_Port GPIOA
//-----------------OLED IIC端口定义----------------  					   
 
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port,OLED_SCLK_Pin,GPIO_PIN_RESET)//SCL  拉低
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port,OLED_SCLK_Pin,GPIO_PIN_SET)  //SCL  拉高
 
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port,OLED_SDIN_Pin,GPIO_PIN_RESET)//SDA  拉低
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port,OLED_SDIN_Pin,GPIO_PIN_SET)  //SDA  拉高
 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
 
//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_Init(void);
void OLED_ShowStringCN(uint8_t x,uint8_t y,uint8_t *chr,uint8_t mode);
void OLED_ShowStringCN1(uint8_t x,uint8_t y,uint8_t *chr,uint8_t mode);
void OLEDIIC_Start(void);
void OLEDIIC_Stop(void);
void OLEDWrite_IIC_Command(uint8_t IIC_Command);
void OLEDWrite_IIC_Data(uint8_t IIC_Data);
void OLEDWrite_IIC_Byte(uint8_t IIC_Byte);
 void OLED_ClearPart(void);
void OLEDIIC_Wait_Ack(void);
#endif    
