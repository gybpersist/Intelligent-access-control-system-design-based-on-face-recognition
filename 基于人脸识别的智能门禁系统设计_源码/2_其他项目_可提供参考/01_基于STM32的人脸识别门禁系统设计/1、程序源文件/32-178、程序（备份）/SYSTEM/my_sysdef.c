/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V4.1
//修改日期:2021/01/14
//程序功能：封装和简化通用的操作
//V2.0  2020/04/08  增加系统软件复位函数
//V2.1  2020/07/03  增加程序ID加密功能
//V3.2  2020/07/16  1、修改字符串指针类型为const，避免程序编译警告
                    2、修改GetIntegerFromString函数参数形式并修复其bug
//V3.3  2020/09/21   1、增加多个字符串判断的状态机函数
                    2、单个子字符串状态机函数名称修改
//V3.4  2020/09/27  1、同时支持裸机和rt-thread
//V3.5  2020/10/18  1、剥离部分代码到my_board中
//V3.6  2020/11/13  1、增加二进制查表函数，可根据实际需要修改
//V3.7  2020/12/11  1、更新加密程序
//V3.8  2020/12/18  1、优化字符串识别状态机
//V3.9  2020/12/30  1、增加SWO（ITM）调试打印函数（DAPLink暂时不支持）
//V4.0  2021/01/10  1、将AlarmMode枚举变量改为AlertMode（Alarm本意为闹钟）
//V4.1  2021/01/14  1、将程序加密代码删除
************************************************/
#include "my_sysdef.h"
#include "my_include.h"

#define VERSION     41

//系统软件复位
void SystemReset_Soft(void)
{
    __set_FAULTMASK(1); //关闭所有中断
    NVIC_SystemReset(); //复位
}
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//关闭所有中断
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
//设置栈顶地址
//addr:栈顶地址
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
#if DEBUG_PRINTF//如果使能了调试打印功能
    u16 m=0,n;
    char myPrintfBuf[512];
    va_list args;
    va_start(args, format);
    n=vsprintf(myPrintfBuf,format, args);
    va_end(args);
    while(n--)//数据长度不为0
    {
        ITM_SendChar(myPrintfBuf[m]);
        m++;
    }
#endif
}
//#if DEBUG_PRINTF//如果使能了调试打印功能
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
            return true; //全都相等则返回是
        }
    }
    else
    {
        *state = 0;
    }
    return false; //全都相等则返回是
}
/****************************************************************
函数功能：判断多个字符串的状态机
strPtr[]：需要匹配的字符串的指针数组
strCount：strPtr[]中元素的个数
state：保存程序状态的变量，标记每个字符串是否匹配
msgByte：程序接收的数据与字符串比较
返回：匹配的字符串在strPtr[]数组中的索引(返回最后一个)
使用示例：再调用程序的位置写如下语句
static const strTest[] = {"1234","abcd","asf23"};//需要判断的字符串
static u8 flag_state[ArrayCount(strTest)];//状态机运行时保存状态的变量
#define StateMachine_GetStr(n)              My_GetStr(strTest,flag_state,ArrayCount(strTest),n)
注意：如果strPtr[]中的字符串存在包含关系，则函数返回第一个匹配的字符串索引
****************************************************************/
u8 My_GetStrings(const char **strPtr,u8 *state,u8 strCount,u8 msgByte)
{
    u8 index_str=0,result=0xff;
    for(index_str=0;index_str<strCount;index_str++,state++)//遍历每一个字符串，与接收字节对比
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
