/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.2
//修改日期:2020/12/30
//程序功能：封装和简化软件模拟IIC的操作
//V1.0 2020/06/06 完成基本功能
//V1.1 2020/10/22 1、删除部分函数，只保留的基本功能
                  2、修改IIC_Wait_Ack函数的返回值
                  3、IIC_SendAck参数使用枚举类型
//V1.2 2020/12/30 1、重新加入数据读写函数（某些场合需要使用）
************************************************/
#include "my_iic.h"

//初始化IIC
void IIC_Init(void)
{
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SCL,GPIO_Mode_Out_PP);
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);
#else
    GPIO_Pin_Init(PIN_IIC_SCL,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
    PinOut(PIN_IIC_SCL)=1;
    PinOut(PIN_IIC_SDA)=1;
}
//产生IIC起始信号
void IIC_Start(void)
{
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda线输出
#endif
    PinOut(PIN_IIC_SDA)=1;
    PinOut(PIN_IIC_SCL)=1;
    delay_us(4);
    PinOut(PIN_IIC_SDA)=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    PinOut(PIN_IIC_SCL)=0;//钳住I2C总线，准备发送或接收数据 
}
//产生IIC停止信号
void IIC_Stop(void)
{
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda线输出
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda线输出
#endif
    PinOut(PIN_IIC_SCL)=0;
    PinOut(PIN_IIC_SDA)=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    PinOut(PIN_IIC_SCL)=1; 
    PinOut(PIN_IIC_SDA)=1;//发送I2C总线结束信号
    delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
bool IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_IPU);//SDA设置为输入
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_INPUT,GPIO_PULLUP);      //SDA设置为输入
#endif
    PinOut(PIN_IIC_SDA)=1;delay_us(1);
    PinOut(PIN_IIC_SCL)=1;delay_us(1);
    while(PinRead(PIN_IIC_SDA))
    {
        ucErrTime++;
        if(ucErrTime>248)
        {
            IIC_Stop();
            return false;
        }
    }
    PinOut(PIN_IIC_SCL)=0;//时钟输出0
    return true;
} 
//产生ACK应答,入口参数:ack (0:ACK 1:NAK)
void IIC_SendAck(Bit_IICACK ack)
{
    PinOut(PIN_IIC_SCL)=0;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda线输出
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda线输出
#endif
    PinOut(PIN_IIC_SDA)=(u8)ack;
    delay_us(2);
    PinOut(PIN_IIC_SCL)=1;
    delay_us(2);
    PinOut(PIN_IIC_SCL)=0;
}
//IIC发送一个字节
//返回从机有无应答
//true，有应答
//false，无应答
u8 IIC_Send_Byte(u8 sendDat)
{
    u8 t;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda线输出
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda线输出
#endif
    PinOut(PIN_IIC_SCL)=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        PinOut(PIN_IIC_SDA)=(sendDat&0x80)>>7;
        sendDat<<=1;
        delay_us(2);//对TEA5767这三个延时都是必须的
        PinOut(PIN_IIC_SCL)=1;
        delay_us(30); 
        PinOut(PIN_IIC_SCL)=0;
        delay_us(30);
    }
    return IIC_Wait_Ack();
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Recv_Byte(Bit_IICACK ack)
{
    u8 i,receive=0;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_IPU);//SDA设置为输入
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_INPUT,GPIO_PULLUP);      //SDA设置为输入
#endif
    for(i=0;i<8;i++ )
    {
        PinOut(PIN_IIC_SCL)=0; 
        delay_us(2);
        PinOut(PIN_IIC_SCL)=1;
        receive<<=1;
        if(PinRead(PIN_IIC_SDA))receive++;   
        delay_us(1); 
    }
    IIC_SendAck(ack); //发送ACK   
    return receive;
}

//写寄存器
//SlaveAddress:器件地址
//REG_Address:寄存器地址
//REG_data:要写入寄存器的值
//返回值:无
void IIC_WriteOneByte(u8 SlaveAddress, u8 REG_Address,u8 REG_data) 
{
    IIC_Start();//产生IIC起始信号
    IIC_Send_Byte(SlaveAddress);//发送写器件指令
    IIC_Send_Byte(REG_Address);//发送寄存器地址
    IIC_Send_Byte(REG_data);//发送值
    IIC_Stop();//发送IIC停止信号
}
//*********************************************************
void IIC_WriteBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len)
{
    IIC_Start();//产生IIC起始信号
    IIC_Send_Byte(SlaveAddress);//发送写器件指令
    IIC_Send_Byte(REG_Address);//发送寄存器地址
    while(len--)
    {
        IIC_Send_Byte(*buf++);//发送值
    }
    IIC_Stop();//发送IIC停止信号
}
void IIC_WriteString(u8 SlaveAddress, u8 REG_Address,char *buf)
{
    IIC_WriteBytes(SlaveAddress,REG_Address,(u8 *)buf,strlen(buf));
}
//读寄存器
//SlaveAddress:器件地址
//REG_Address:寄存器地址
//返回值:读到的值
u8 IIC_ReadOneByte(u8 SlaveAddress, u8 REG_Address)
{
    u8 temp=0;
    IIC_Start();//产生IIC起始信号
    IIC_Send_Byte(SlaveAddress);//发送写器件指令	 
    IIC_Send_Byte(REG_Address);   //发送寄存器地址
    
    IIC_Start();//重新启动，产生IIC起始信号
    IIC_Send_Byte(SlaveAddress+1);//发送读器件指令
    temp=IIC_Recv_Byte(IIC_BIT_NOACK);//读取一个字节,不继续再读,发送NAK
    IIC_Stop();//发送IIC停止信号
    return temp;//返回读到的值
}

//*********************************************************
void IIC_ReadBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len)
{
    IIC_Start();//起始信号
    IIC_Send_Byte(SlaveAddress);//发送设备地址+写信号
    IIC_Send_Byte(REG_Address);//发送存储单元地址
    IIC_Start();//起始信号
    IIC_Send_Byte(SlaveAddress+1);//发送设备地址+读信号
    while(--len)
    {
        *buf++ = IIC_Recv_Byte(IIC_BIT_ACK);
    }
        *buf++ = IIC_Recv_Byte(IIC_BIT_NOACK);
    IIC_Stop();//停止信号
}

