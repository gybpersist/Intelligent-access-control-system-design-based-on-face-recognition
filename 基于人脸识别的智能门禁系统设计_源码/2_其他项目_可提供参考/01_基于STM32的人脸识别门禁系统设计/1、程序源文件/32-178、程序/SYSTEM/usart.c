/****************************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V2.7
//�޸�����:2020/11/24
//�����ܣ���װ�ͼ�USART�Ĳ���
//V2.3  2020/06/03
//V2.4  2020/07/14  �޸ĺ������Ʋ����Ӻ궨����ݾɰ汾
//V2.5  2020/08/27  �Ż����ֳ���
//V2.6  2020/10/22  1��ɾ��USARTx_Printf��set��get���򣬸�Ϊͷ�ļ�extern
//V2.7  2020/11/24  1���Ż����ֳ���
****************************************************************/
#include "usart.h"

#define VERSION         27

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#if !defined ( __GNUC__ ) || defined (__CC_ARM) /* GNU Compiler */
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE 
{ 
    int handle; 
}; 
#endif

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x) 
{
    x = x; 
}
USART_TypeDef* USARTx_Printf = USART1;
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
    USARTSendByte(USARTx_Printf,ch);
    return ch;
}
#endif 

void My_USART_SendByte(USART_TypeDef* USARTx, uint8_t dat)
{
    while((USARTx->SR&USART_SR_TC)==0);//�ȴ��������
//  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    USART_SendData(USARTx,dat);
}
void My_USART_SendBytes(USART_TypeDef* USARTx, const uint8_t *dat, uint16_t length)
{
    while(length--)//���ݳ��Ȳ�Ϊ0
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
    //GPIO�˿�����
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    if(USARTx == USART1)
    {
#if EN_USART1   //���ʹ���˴���1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART1��GPIOAʱ��
        //USART1_TX   PA.9
        GPIO_Pin_Init(PIN_USART1_TX,GPIO_Mode_AF_PP);//�����������
        //USART1_RX	  PA.10
        GPIO_Pin_Init(PIN_USART1_RX,GPIO_Mode_IN_FLOATING);//��������
        //USART1 NVIC ����
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�0
#endif
    }
#if EN_USART2   //���ʹ���˴���2
    else if(USARTx == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//ʹ��USART2
        //USART2_TX   PA.2
        GPIO_Pin_Init(PIN_USART2_TX,GPIO_Mode_AF_PP);//�����������
        //USART2_RX   PA.3
        GPIO_Pin_Init(PIN_USART2_RX,GPIO_Mode_IN_FLOATING);//��������
        //USART2 NVIC ����
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�1
    }
#endif
#if EN_USART3   //���ʹ���˴���3
    else if(USARTx == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//ʹ��USART3
        //USART3_TX   PB.10
        GPIO_Pin_Init(PIN_USART3_TX,GPIO_Mode_AF_PP);//�����������
        //USART3_RX	  PB.11
        GPIO_Pin_Init(PIN_USART3_RX,GPIO_Mode_IN_FLOATING);//��������
        //USART3 NVIC ����
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�2
    }
#endif

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = baud;//һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_DeInit(USARTx);//��λ����
    USART_Init(USARTx, &USART_InitStructure); //��ʼ������
    if(USARTx == USART1)
    {
#if EN_USART1   //���ʹ���˴���1
    #if USART1_IT_RXNE>0
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    #endif
    #if USART1_IT_TXE>0
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//�������ڷ����ж�
    #endif
    #if USART1_IT_IDLE>0
        USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//�������ڿ����ж�
    #endif
#endif
    }
#if EN_USART2   //���ʹ���˴���2
    else if(USARTx == USART2)
    {
    #if USART2_IT_RXNE>0
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    #endif
    #if USART2_IT_TXE>0
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//�������ڷ����ж�
    #endif
    #if USART2_IT_IDLE>0
        USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//�������ڿ����ж�
    #endif
    }
#endif
#if EN_USART3   //���ʹ���˴���3
    else if(USARTx == USART3)
    {
    #if USART3_IT_RXNE>0
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    #endif
    #if USART3_IT_TXE>0
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//�������ڷ����ж�
    #endif
    #if USART3_IT_IDLE>0
        USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//�������ڿ����ж�
    #endif
    }
#endif
    USART_Cmd(USARTx, ENABLE);//ʹ�ܴ��� 
}

void checkWireLessMode(u8 recBuf);

#if EN_USART1   //���ʹ���˴���1
//�����жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
void USART1_IRQHandler(void)//����1�жϷ������
{
    u8 Res;
#if USART1_IT_RXNE>0
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�����ж�
    {
        Res = USART_ReceiveData(USART1);//��ȡ���յ�������        
//        checkWireLessMode(Res);//�������ͼ��        
        My_UARTMessage_Receive(Res);
        
//        My_ESPMessage_Receive(Res);//���ݴ���
//        ProcessSim800a(Res);//���ݴ���
//        My_UARTMessage_Receive(Res);//���ݴ���
//        GPS_Process(Res);//GPS���ݴ���
//        MessageStateMachine(Res);//�Զ��崮��Э��
//        FPM10A_MsgStateMachine(Res);//ָ��ģ��          
    }
#endif
#if USART1_IT_IDLE>0
    {
        u8 USART1_RX_Buff[LEN_BUF_USART1_RX];//���ݻ�����
        if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//����֡�ж�
        {
            Res=USART1->SR;
            Res=USART1->DR;//����������IDLE��־λ
            Res = LEN_BUF_USART1_RX - DMA_GetCurrDataCounter(DMA1_Channel5);//����������ݳ���
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

#if EN_USART2   //���ʹ���˴���2
//�����жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   
void USART2_IRQHandler(void)//����2�жϷ������
{
    u8 Res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//�����ж�
    {
        Res = USART_ReceiveData(USART2);//��ȡ���յ�������
        
//        GPS_Process(Res);        
//        OnUSART2_ReceiveData(Res);//���ݴ���
    } 
}
#if USART2_IT_IDLE>0
void __weak OnUSART2_ReceiveBuff(u8 *buf,u16 len)
{

}
#endif
#endif

#if EN_USART3   //���ʹ���˴���3
//�����жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
void USART3_IRQHandler(void)//����3�жϷ������
{
    u8 Res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//�����ж�
    {
        Res = USART_ReceiveData(USART3);//��ȡ���յ�������
        My_FaceData_Receive(Res);//����ʶ���������
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
