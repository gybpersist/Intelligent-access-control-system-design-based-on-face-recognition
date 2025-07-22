/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V4.1
//�޸�����:2021/01/14
//�����ܣ���װ�ͼ�ͨ�õĲ���
//V2.0  2020/04/08  ����ϵͳ�����λ����
//V2.1  2020/07/03  ���ӳ���ID���ܹ���
//V3.2  2020/07/16  1���޸��ַ���ָ������Ϊconst�����������뾯��
                    2���޸�GetIntegerFromString����������ʽ���޸���bug
//V3.3  2020/09/21   1�����Ӷ���ַ����жϵ�״̬������
                    2���������ַ���״̬�����������޸�
//V3.4  2020/09/27  1��ͬʱ֧�������rt-thread
//V3.5  2020/10/18  1�����벿�ִ��뵽my_board��
//V3.6  2020/11/13  1�����Ӷ����Ʋ�������ɸ���ʵ����Ҫ�޸�
//V3.7  2020/12/11  1�����¼��ܳ���
//V3.8  2020/12/18  1���Ż��ַ���ʶ��״̬��
//V3.9  2020/12/30  1������SWO��ITM�����Դ�ӡ������DAPLink��ʱ��֧�֣�
//V4.0  2021/01/10  1����AlarmModeö�ٱ�����ΪAlertMode��Alarm����Ϊ���ӣ�
//V4.1  2021/01/14  1����������ܴ���ɾ��
************************************************/
#include "my_sysdef.h"
#include "my_include.h"

#define VERSION     41

//ϵͳ�����λ
void SystemReset_Soft(void)
{
    __set_FAULTMASK(1); //�ر������ж�
    NVIC_SystemReset(); //��λ
}
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//�ر������ж�
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
//����ջ����ַ
//addr:ջ����ַ
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
void MSR_MSP(u32 addr) 
{
    __ASM volatile("MSR MSP, r0");//set Main Stack value
    __ASM volatile("BX r14");
}
#else
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0//set Main Stack value
    BX r14
}
#endif

void printf_debug(const char *format, ...)
{
#if DEBUG_PRINTF//���ʹ���˵��Դ�ӡ����
    u16 m=0,n;
    char myPrintfBuf[512];
    va_list args;
    va_start(args, format);
    n=vsprintf(myPrintfBuf,format, args);
    va_end(args);
    while(n--)//���ݳ��Ȳ�Ϊ0
    {
        ITM_SendChar(myPrintfBuf[m]);
        m++;
    }
#endif
}
//#if DEBUG_PRINTF//���ʹ���˵��Դ�ӡ����
//__IO u32 ITM_RxBuffr = ITM_RXBUFFER_EMPTY;
//int fgetc(FILE *f)
//{
//    int ch = -1;
//    while(ITM_RxBuffr == ITM_RXBUFFER_EMPTY);
//    ch = ITM_RxBuffr;
//    ITM_RxBuffr = ITM_RXBUFFER_EMPTY;
//    return ch;
//}
//#endif

bool StateMachine_GetStr(char *str,u8 msgByte,u8 *state)
{
    if(msgByte == *(str+*state))
    {
        *state = *state + 1;
        if(*state == strlen(str))
        {
            *state = 0;
            return true; //ȫ������򷵻���
        }
    }
    else
    {
        *state = 0;
    }
    return false; //ȫ������򷵻���
}
/****************************************************************
�������ܣ��ж϶���ַ�����״̬��
strPtr[]����Ҫƥ����ַ�����ָ������
strCount��strPtr[]��Ԫ�صĸ���
state���������״̬�ı��������ÿ���ַ����Ƿ�ƥ��
msgByte��������յ��������ַ����Ƚ�
���أ�ƥ����ַ�����strPtr[]�����е�����(�������һ��)
ʹ��ʾ�����ٵ��ó����λ��д�������
static const strTest[] = {"1234","abcd","asf23"};//��Ҫ�жϵ��ַ���
static u8 flag_state[ArrayCount(strTest)];//״̬������ʱ����״̬�ı���
#define StateMachine_GetStr(n)              My_GetStr(strTest,flag_state,ArrayCount(strTest),n)
ע�⣺���strPtr[]�е��ַ������ڰ�����ϵ���������ص�һ��ƥ����ַ�������
****************************************************************/
u8 My_GetStrings(const char **strPtr,u8 *state,u8 strCount,u8 msgByte)
{
    u8 index_str=0,result=0xff;
    for(index_str=0;index_str<strCount;index_str++,state++)//����ÿһ���ַ�����������ֽڶԱ�
    {
        if(msgByte == (u8)*(strPtr[index_str]+*state))
        {
            (*state)++;
            if(*(strPtr[index_str]+*state)==0)
            {
                *state = 0;
                result = index_str;
//                break;
            }
        }
        else if(msgByte == (u8)*(strPtr[index_str]))
        {
            *state = 1;
        }
        else
        {
            *state = 0;
        }
    }
    return result;
}
void ByteArrayToHalfWordArray(u8 *byteArray,u16 *halfWordArray,u16 lengthOfByteArray)
{
    u16 i=0;
    union_u16 temp;
    while(i<lengthOfByteArray)
    {
        temp.Bytes.byte_1 = byteArray[i];
        i++;
        if(i==lengthOfByteArray)
        {
            temp.Bytes.byte_0 = 0;
        }
        else
        {
            temp.Bytes.byte_0 = byteArray[i];
        }
        halfWordArray[i>>1] = temp.value;
        i++;
    }
}
void HalfWordArrayToByteArray(u16 *halfWordArray,u8 *byteArray,u16 lengthOfHalfWordArray)
{
    u16 i=0;
    union_u16 temp;
    while(i<lengthOfHalfWordArray)
    {
        temp.value = halfWordArray[i];
        byteArray[i<<1] = temp.Bytes.byte_1;
        byteArray[(i<<1)+1] = temp.Bytes.byte_0;
        i++;
    }
}
void SortShort(short *arr,u16 len,bool toBig)
{
    u16 i,j;
    short temp;
    for(i=0;i<len;i++)
    {
        for(j=0;j<len-i;j++)
        {
            if(arr[j]>arr[j+1])
            {
                temp = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
int64 ParseInteger(const char *str,u16 len)
{
    int64 value = 0;
    char *strPtr = (char *)str;
    while(len--)
    {
        value = (value<<3) +value +value + (*strPtr++-0x30);
    }
    return value;
}

u16 BinarySearch_Float(float *tab,float value,u16 len)
{
    u16 index_min=0,index_max=len,index_mid;
    while(index_max>index_min+1)
    {
        index_mid = (index_min+index_max)>>1;
        if( value < (*(tab+index_mid)))
        {
            index_max = index_mid;
        }
        else
        {
            index_min = index_mid;
        }
    }
    if((value*2)<((*(tab+index_min) + *(tab+index_max))))
    {
        return index_min;
    }
    return index_max;
}
u16 HexStringToBytes(char *hexStr,u8 *byteArray)
{
    u16 byteCount = 0;
    char *strPtr = hexStr;
    if((byteCount=strlen(hexStr))%2)
    {
        return 0;
    }
    while(*strPtr)
    {
        if((*strPtr>='0' && *strPtr<='9') || (*strPtr>='A' && *strPtr<='F') || (*strPtr>='a' && *strPtr<='f'))
        {
            strPtr++;
        }
        else
        {
            return 0;
        }
    }
    strPtr = hexStr;
    while(*strPtr)
    {
        if((*strPtr>='0' && *strPtr<='9'))
        {
            *byteArray = ((*strPtr)-'0')<<4;
        }
        else if((*strPtr>='A' && *strPtr<='F'))
        {
            *byteArray = ((*strPtr)-'A'+10)<<4;
        }
        else
        {
            *byteArray = ((*strPtr)-'a'+10)<<4;
        }
        strPtr++;
        if((*strPtr>='0' && *strPtr<='9'))
        {
            *byteArray += ((*strPtr)-'0');
        }
        else if((*strPtr>='A' && *strPtr<='F'))
        {
            *byteArray += ((*strPtr)-'A'+10);
        }
        else
        {
            *byteArray += ((*strPtr)-'a'+10);
        }
        strPtr++;
        byteArray++;
    }
    return byteCount;
}
u16 My_Sysdef_GetVersion(void)
{
    return VERSION;
}
