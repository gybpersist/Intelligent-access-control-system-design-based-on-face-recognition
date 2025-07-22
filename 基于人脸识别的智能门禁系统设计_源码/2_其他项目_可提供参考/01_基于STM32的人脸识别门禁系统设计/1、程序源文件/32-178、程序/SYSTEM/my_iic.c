/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.2
//�޸�����:2020/12/30
//�����ܣ���װ�ͼ����ģ��IIC�Ĳ���
//V1.0 2020/06/06 ��ɻ�������
//V1.1 2020/10/22 1��ɾ�����ֺ�����ֻ�����Ļ�������
                  2���޸�IIC_Wait_Ack�����ķ���ֵ
                  3��IIC_SendAck����ʹ��ö������
//V1.2 2020/12/30 1�����¼������ݶ�д������ĳЩ������Ҫʹ�ã�
************************************************/
#include "my_iic.h"

//��ʼ��IIC
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
//����IIC��ʼ�ź�
void IIC_Start(void)
{
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda�����
#endif
    PinOut(PIN_IIC_SDA)=1;
    PinOut(PIN_IIC_SCL)=1;
    delay_us(4);
    PinOut(PIN_IIC_SDA)=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    PinOut(PIN_IIC_SCL)=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda�����
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda�����
#endif
    PinOut(PIN_IIC_SCL)=0;
    PinOut(PIN_IIC_SDA)=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    PinOut(PIN_IIC_SCL)=1; 
    PinOut(PIN_IIC_SDA)=1;//����I2C���߽����ź�
    delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
bool IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_IPU);//SDA����Ϊ����
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_INPUT,GPIO_PULLUP);      //SDA����Ϊ����
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
    PinOut(PIN_IIC_SCL)=0;//ʱ�����0
    return true;
} 
//����ACKӦ��,��ڲ���:ack (0:ACK 1:NAK)
void IIC_SendAck(Bit_IICACK ack)
{
    PinOut(PIN_IIC_SCL)=0;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda�����
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda�����
#endif
    PinOut(PIN_IIC_SDA)=(u8)ack;
    delay_us(2);
    PinOut(PIN_IIC_SCL)=1;
    delay_us(2);
    PinOut(PIN_IIC_SCL)=0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//true����Ӧ��
//false����Ӧ��
u8 IIC_Send_Byte(u8 sendDat)
{
    u8 t;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda�����
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);//sda�����
#endif
    PinOut(PIN_IIC_SCL)=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        PinOut(PIN_IIC_SDA)=(sendDat&0x80)>>7;
        sendDat<<=1;
        delay_us(2);//��TEA5767��������ʱ���Ǳ����
        PinOut(PIN_IIC_SCL)=1;
        delay_us(30); 
        PinOut(PIN_IIC_SCL)=0;
        delay_us(30);
    }
    return IIC_Wait_Ack();
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Recv_Byte(Bit_IICACK ack)
{
    u8 i,receive=0;
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_IPU);//SDA����Ϊ����
#else
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_MODE_INPUT,GPIO_PULLUP);      //SDA����Ϊ����
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
    IIC_SendAck(ack); //����ACK   
    return receive;
}

//д�Ĵ���
//SlaveAddress:������ַ
//REG_Address:�Ĵ�����ַ
//REG_data:Ҫд��Ĵ�����ֵ
//����ֵ:��
void IIC_WriteOneByte(u8 SlaveAddress, u8 REG_Address,u8 REG_data) 
{
    IIC_Start();//����IIC��ʼ�ź�
    IIC_Send_Byte(SlaveAddress);//����д����ָ��
    IIC_Send_Byte(REG_Address);//���ͼĴ�����ַ
    IIC_Send_Byte(REG_data);//����ֵ
    IIC_Stop();//����IICֹͣ�ź�
}
//*********************************************************
void IIC_WriteBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len)
{
    IIC_Start();//����IIC��ʼ�ź�
    IIC_Send_Byte(SlaveAddress);//����д����ָ��
    IIC_Send_Byte(REG_Address);//���ͼĴ�����ַ
    while(len--)
    {
        IIC_Send_Byte(*buf++);//����ֵ
    }
    IIC_Stop();//����IICֹͣ�ź�
}
void IIC_WriteString(u8 SlaveAddress, u8 REG_Address,char *buf)
{
    IIC_WriteBytes(SlaveAddress,REG_Address,(u8 *)buf,strlen(buf));
}
//���Ĵ���
//SlaveAddress:������ַ
//REG_Address:�Ĵ�����ַ
//����ֵ:������ֵ
u8 IIC_ReadOneByte(u8 SlaveAddress, u8 REG_Address)
{
    u8 temp=0;
    IIC_Start();//����IIC��ʼ�ź�
    IIC_Send_Byte(SlaveAddress);//����д����ָ��	 
    IIC_Send_Byte(REG_Address);   //���ͼĴ�����ַ
    
    IIC_Start();//��������������IIC��ʼ�ź�
    IIC_Send_Byte(SlaveAddress+1);//���Ͷ�����ָ��
    temp=IIC_Recv_Byte(IIC_BIT_NOACK);//��ȡһ���ֽ�,�������ٶ�,����NAK
    IIC_Stop();//����IICֹͣ�ź�
    return temp;//���ض�����ֵ
}

//*********************************************************
void IIC_ReadBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len)
{
    IIC_Start();//��ʼ�ź�
    IIC_Send_Byte(SlaveAddress);//�����豸��ַ+д�ź�
    IIC_Send_Byte(REG_Address);//���ʹ洢��Ԫ��ַ
    IIC_Start();//��ʼ�ź�
    IIC_Send_Byte(SlaveAddress+1);//�����豸��ַ+���ź�
    while(--len)
    {
        *buf++ = IIC_Recv_Byte(IIC_BIT_ACK);
    }
        *buf++ = IIC_Recv_Byte(IIC_BIT_NOACK);
    IIC_Stop();//ֹͣ�ź�
}

