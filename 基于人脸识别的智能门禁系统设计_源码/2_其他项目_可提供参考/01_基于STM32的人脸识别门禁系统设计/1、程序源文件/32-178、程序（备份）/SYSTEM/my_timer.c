/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//YXDZ STM32������
//���ε��ӹ�����
//���ߣ�tianqingyong
//�汾��V1.1
//�޸�����:2020/06/06
//�����ܣ���װ��ʱ������
//V1.0 ��ɻ�������
//V1.1 ���򲿷ֹ���
************************************************/
 #include "my_timer.h"

/***********************************************
�������ܣ���ʼ����ʱ��
TIMx����ʱ��ѡ��TIM1��TIM2��TIM3��TIM4
arr���Զ�����ֵ
psc��ʱ�ӷ�Ƶϵ��
itEnable���Ƿ�ʹ���жϣ�trueΪʹ�ܣ�falseΪ��ʹ��
************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc,bool itEnable)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    if(TIMx == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
    }
    else if(TIMx == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    }

    TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    if(itEnable)
    {
        TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);//ʹ��TIM�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    }

    TIM_Cmd(TIMx, ENABLE);  //ʹ��TIMx����
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
    }
}

