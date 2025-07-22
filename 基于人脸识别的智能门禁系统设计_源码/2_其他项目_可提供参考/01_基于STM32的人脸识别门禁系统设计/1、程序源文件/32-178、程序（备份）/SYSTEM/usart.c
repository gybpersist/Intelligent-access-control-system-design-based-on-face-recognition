/****************************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V2.7
//修改日期:2020/11/24
//程序功能：封装和简化USART的操作
//V2.3  2020/06/03
//V2.4  2020/07/14  修改函数名称并增加宏定义兼容旧版本
//V2.5  2020/08/27  优化部分程序
//V2.6  2020/10/22  1、删除USARTx_Printf的set与get程序，改为头文件extern
//V2.7  2020/11/24  1、优化部分程序
****************************************************************/
#include "usart.h"

#define VERSION         27

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#if !defined ( __GNUC__ ) || defined (__CC_ARM) /* GNU Compiler */
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE 
{ 
    int handle; 
}; 
#endif

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) 
{
    x = x; 
}
USART_TypeDef* USARTx_Printf = USART1;
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
    USARTSendByte(USARTx_Printf,ch);
    return ch;
}
#endif 

void My_USART_SendByte(USART_TypeDef* USARTx, uint8_t dat)
{
    while((USARTx->SR&USART_SR_TC)==0);//等待发送完毕
//  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    USART_SendData(USARTx,dat);
}
void My_USART_SendBytes(USART_TypeDef* USARTx, const uint8_t *dat, uint16_t length)
{
    while(length--)//数据长度不为0
    {
        USARTSendByte(USARTx,*dat);
        dat++;
    }
}

void My_USART_SendString(USART_TypeDef* USARTx, const char *str)
{
    while(*str!='\0')
    {
        USARTSendByte(USARTx,*str);
        str++;
    }
}

void My_USART_printf(USART_TypeDef* USARTx,const char *format, ...)
{
    u16 n;
    char myPrintfBuf[512];
    va_list args;
    va_start(args, format);
    n=vsprintf(myPrintfBuf,format, args);
    va_end(args);
    USARTSendBytes(USARTx,(u8 *)myPrintfBuf,n);
}

void My_USART_Init(USART_TypeDef* USARTx, u32 baud)
{
    //GPIO端口设置
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    if(USARTx == USART1)
    {
#if EN_USART1   //如果使能了串口1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1，GPIOA时钟
        //USART1_TX   PA.9
        GPIO_Pin_Init(PIN_USART1_TX,GPIO_Mode_AF_PP);//复用推挽输出
        //USART1_RX	  PA.10
        GPIO_Pin_Init(PIN_USART1_RX,GPIO_Mode_IN_FLOATING);//浮空输入
        //USART1 NVIC 配置
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级0
#endif
    }
#if EN_USART2   //如果使能了串口2
    else if(USARTx == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART2
        //USART2_TX   PA.2
        GPIO_Pin_Init(PIN_USART2_TX,GPIO_Mode_AF_PP);//复用推挽输出
        //USART2_RX   PA.3
        GPIO_Pin_Init(PIN_USART2_RX,GPIO_Mode_IN_FLOATING);//浮空输入
        //USART2 NVIC 配置
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级1
    }
#endif
#if EN_USART3   //如果使能了串口3
    else if(USARTx == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//使能USART3
        //USART3_TX   PB.10
        GPIO_Pin_Init(PIN_USART3_TX,GPIO_Mode_AF_PP);//复用推挽输出
        //USART3_RX	  PB.11
        GPIO_Pin_Init(PIN_USART3_RX,GPIO_Mode_IN_FLOATING);//浮空输入
        //USART3 NVIC 配置
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级2
    }
#endif

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_DeInit(USARTx);//复位串口
    USART_Init(USARTx, &USART_InitStructure); //初始化串口
    if(USARTx == USART1)
    {
#if EN_USART1   //如果使能了串口1
    #if USART1_IT_RXNE>0
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接收中断
    #endif
    #if USART1_IT_TXE>0
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//开启串口发送中断
    #endif
    #if USART1_IT_IDLE>0
        USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//开启串口空闲中断
    #endif
#endif
    }
#if EN_USART2   //如果使能了串口2
    else if(USARTx == USART2)
    {
    #if USART2_IT_RXNE>0
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接收中断
    #endif
    #if USART2_IT_TXE>0
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//开启串口发送中断
    #endif
    #if USART2_IT_IDLE>0
        USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//开启串口空闲中断
    #endif
    }
#endif
#if EN_USART3   //如果使能了串口3
    else if(USARTx == USART3)
    {
    #if USART3_IT_RXNE>0
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接收中断
    #endif
    #if USART3_IT_TXE>0
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//开启串口发送中断
    #endif
    #if USART3_IT_IDLE>0
        USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//开启串口空闲中断
    #endif
    }
#endif
    USART_Cmd(USARTx, ENABLE);//使能串口 
}

void checkWireLessMode(u8 recBuf);

#if EN_USART1   //如果使能了串口1
//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
void USART1_IRQHandler(void)//串口1中断服务程序
{
    u8 Res;
#if USART1_IT_RXNE>0
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断
    {
        Res = USART_ReceiveData(USART1);//读取接收到的数据        
//        checkWireLessMode(Res);//无线类型检查        
        My_UARTMessage_Receive(Res);
        
//        My_ESPMessage_Receive(Res);//数据处理
//        ProcessSim800a(Res);//数据处理
//        My_UARTMessage_Receive(Res);//数据处理
//        GPS_Process(Res);//GPS数据处理
//        MessageStateMachine(Res);//自定义串口协议
//        FPM10A_MsgStateMachine(Res);//指纹模块          
    }
#endif
#if USART1_IT_IDLE>0
    {
        u8 USART1_RX_Buff[LEN_BUF_USART1_RX];//数据缓冲区
        if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲帧中断
        {
            Res=USART1->SR;
            Res=USART1->DR;//软件序列清除IDLE标志位
            Res = LEN_BUF_USART1_RX - DMA_GetCurrDataCounter(DMA1_Channel5);//计算接收数据长度
            OnUSART1_ReceiveBuff(USART1_RX_Buff,Res);
            My_DMA_Enable(DMA1_Channel5,LEN_BUF_USART1_RX);
        }
    }
#endif
}
#if USART1_IT_IDLE>0
void __weak OnUSART1_ReceiveBuff(u8 *buf,u16 len)
{

}
#endif
#endif

#if EN_USART2   //如果使能了串口2
//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   
void USART2_IRQHandler(void)//串口2中断服务程序
{
    u8 Res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收中断
    {
        Res = USART_ReceiveData(USART2);//读取接收到的数据
        
//        GPS_Process(Res);        
//        OnUSART2_ReceiveData(Res);//数据处理
    } 
}
#if USART2_IT_IDLE>0
void __weak OnUSART2_ReceiveBuff(u8 *buf,u16 len)
{

}
#endif
#endif

#if EN_USART3   //如果使能了串口3
//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
void USART3_IRQHandler(void)//串口3中断服务程序
{
    u8 Res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收中断
    {
        Res = USART_ReceiveData(USART3);//读取接收到的数据
        My_FaceData_Receive(Res);//人脸识别接收数据
//        OnUSART3_ReceiveData(Res);
    }
} 
#if USART3_IT_IDLE>0
void __weak OnUSART3_ReceiveBuff(u8 *buf,u16 len)
{

}
#endif
#endif

u16 My_USART_GetVersion(void)
{
    return VERSION;
}
