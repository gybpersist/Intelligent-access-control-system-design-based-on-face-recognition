/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include "string.h"
#include "esp8266_at.h"

#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//�˴������Լ���wifi������
#define WIFI_NAME "MERCURY_3532"
#define WIFI_PASSWD "aabbccdd123"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

void USB_RESET()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    /*Configure GPIO pin : PA12 */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,
                      GPIO_PIN_RESET);
    HAL_Delay(65);
    //�Ȱ�PA12���������ߣ�����D+ģ��USB�İβ嶯��
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
    HAL_Delay(65);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t get_TimeDiff(uint32_t lstT)
{
    return HAL_GetTick()-lstT;
}



uint8_t uart1_RxBuffer; //�����жϻ���
uint8_t uart2_RxBuffer; //�����жϻ���
uint8_t uart3_RxBuffer; //�����жϻ���
#define USART1_MAX_RECV_LEN 64 // ���ڻ����С
uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN]; //��������
uint16_t USART1_RX_STA=0;// ����1 ������ɱ�־�����ݼ�����
uint16_t USART1_RX_OK=0;


//#define USART2_MAX_RECV_LEN 32 // ���ڻ����С
//uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN]; //��������
uint16_t USART2_RX_STA=0;// ����1 ������ɱ�־�����ݼ�����
uint16_t USART2_RX_OK=0;

#define USART3_MAX_RECV_LEN 32 // ���ڻ����С
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; //��������
uint16_t USART3_RX_STA=0;// ����1 ������ɱ�־�����ݼ�����
uint16_t USART3_RX_OK=0;
uint8_t openCnt=0;//���ż�ʱ
#define OPEN_PERIOD   3 //��������

uint8_t flag_connection=0;// 0��ʾδ���� 1��ʾ������
void ES8266_MQTT_Init(uint8_t status1 );
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    uint32_t lst_t=0;
    char s_temp[256];// ����ת���ַ���ʹ��
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
USB_RESET();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
    OLED_Init();//��ʼ����ʾ��
    OLED_Clear();
    OLED_ShowStringCN(16,0,(unsigned char*)"�����Ž�",16);//��ʾϵͳ��ʼ����
    OLED_Refresh();

    HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart1_RxBuffer, 1);
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart2_RxBuffer, 1);
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&uart3_RxBuffer, 1);
    HAL_TIM_Base_Start_IT(&htim2);// ������ʱ�� �����жϴ����źŽ���



    IoT_Parameter_Init();
    ES8266_MQTT_Init(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
				if(get_TimeDiff(lst_t)>1000)
				{
					lst_t = HAL_GetTick();
					
					if(openCnt >0)
					{
						openCnt--;
						HAL_GPIO_WritePin(RELAY_GPIO_Port,RELAY_Pin,GPIO_PIN_SET);
					}else{
						
						HAL_GPIO_WritePin(RELAY_GPIO_Port,RELAY_Pin,GPIO_PIN_RESET);
					}
				}
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==RESET)
        {
            HAL_Delay(100);
            while(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==RESET);
            uint8_t ss[8]= {0xef,0xaa,0x12,0,0,0,0,0x12};
            HAL_UART_Transmit(&huart1,ss,8,0xffff);
        }
        else 				if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==RESET)
        {
            HAL_Delay(100);
            while(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==RESET);
            uint8_t ss[8]= {0xef,0xaa,0x13,0,0,0,0,0x13};
            HAL_UART_Transmit(&huart1,ss,8,0xffff);
        }
        else 				if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==RESET)
        {
            HAL_Delay(100);
            while(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==RESET);
            uint8_t ss[8]= {0xef,0xaa,0x21,0,0,0,0,0x21};
            HAL_UART_Transmit(&huart1,ss,8,0xffff);
        }

        if (USART1_RX_OK==1)
        {
            USART1_RX_OK=0;

            if(USART1_RX_BUF[0]==0xef && USART1_RX_BUF[1]==0xaa)//��ȡ��TX510���ص�����
            {
                if(USART1_RX_BUF[7]==0x12)// ��֤ID
                {
                    if(USART1_RX_BUF[8]==0x0 && USART1_RX_BUF[6]>0x02)//��֤ͨ��
                    {
                        OLED_Clear();

                        HAL_GPIO_WritePin(RELAY_GPIO_Port,RELAY_Pin,GPIO_PIN_SET);
                        OLED_ShowStringCN(0,16,(unsigned char*)"��֤�ɹ�",16);
											sprintf(s_temp,"��֤�ɹ�,ID:%x%x",USART1_RX_BUF[9],USART1_RX_BUF[10]);
												HAL_UART_Transmit(&huart3,s_temp,strlen(s_temp),0xffff);
											
                        sprintf(s_temp,"ID:%x%x",USART1_RX_BUF[9],USART1_RX_BUF[10]);
                        OLED_ShowString(0,32,(unsigned char*)s_temp,16,1);
                        OLED_Refresh();
                        HAL_Delay(2000);                        
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"�����Ž�",16);//��ʾϵͳ��ʼ����
                        OLED_Refresh();
               //����������
								sprintf(s_temp,"{\"id\":\"1\"\,\"params\":{\"myid\":{\"value\":%d}\}}",
												(uint16_t)USART1_RX_BUF[9]*256+USART1_RX_BUF[10]);
      
                ESP8266_Pub(P_TOPIC_NAME,s_temp);
                    } else {

                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"��֤ʧ��",16);
											HAL_UART_Transmit(&huart3,"��֤ʧ��",strlen("��֤ʧ��"),0xffff);
                        OLED_Refresh();
                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"�����Ž�",16);//��ʾϵͳ��ʼ����
                        OLED_Refresh();
                    }
                } else if(USART1_RX_BUF[7]==0x13)// ע��ID
                {
                    if(USART1_RX_BUF[8]==0x0 && USART1_RX_BUF[6]>0x02)//ע��ͨ��
                    {
                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"ע��ɹ�",16);
											HAL_UART_Transmit(&huart3,"ע��ɹ�",strlen("ע��ɹ�"),0xffff);
                        sprintf(s_temp,"ID:%x%x",USART1_RX_BUF[9],USART1_RX_BUF[10]);
                        OLED_ShowString(0,32,(unsigned char*)s_temp,16,1);
                        OLED_Refresh();

                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"�����Ž�",16);//��ʾϵͳ��ʼ����
                        OLED_Refresh();

                    } else {
                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"ע��ʧ��",16);
                        OLED_Refresh();

											HAL_UART_Transmit(&huart3,"ע��ʧ��",strlen("ע��ʧ��"),0xffff);
                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"�����Ž�",16);//��ʾϵͳ��ʼ����
                        OLED_Refresh();
                    }
                } else if(USART1_RX_BUF[7]==0x21)// ɾ������ID
                {
                    if(USART1_RX_BUF[8]==0x0 && USART1_RX_BUF[6]==0x02)//ɾ��ͨ��
                    {
                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"ɾ���ɹ�",16);
                        OLED_Refresh();

                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"�����Ž�",16);//��ʾϵͳ��ʼ����
                        OLED_Refresh();

                    }
                }
            }
            memset(USART1_RX_BUF,0,32);
            USART1_RX_STA=0;
        }

        if (USART3_RX_OK==1)
        {
            USART3_RX_OK=0;
      
            memset(USART3_RX_BUF,0,32);
            USART3_RX_STA=0;

        }

        if (USART2_RX_OK==1)
        {
            USART2_RX_OK=0;

					printf(" Get msg from OneNet:\r\n%s \r\n----------------------------------------",usart2_rxbuf);
            memset(usart2_rxbuf,0,USART2_MAX_RECV_LEN);
            usart2_rxcounter=0;

        }
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t flag_rx1=0;
uint32_t rx_cnt1=0;
uint8_t flag_rx2=0;
uint32_t rx_cnt2=0;
uint8_t flag_rx3=0;
uint32_t rx_cnt3=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */

    /* NOTE: This function Should not be modified, when the callback is needed,
             the HAL_UART_TxCpltCallback could be implemented in the user file
     */
    if(huart->Instance == USART1)	// �ж������ĸ����ڴ������ж�
    {
        flag_rx1=1;
        rx_cnt1=0;
        //�����յ������ݷ������usart1��������
        if(USART1_RX_STA<USART1_MAX_RECV_LEN)	//�����Խ�������
        {
            USART1_RX_BUF[USART1_RX_STA++]=uart1_RxBuffer;	//��¼���յ���ֵ
        } else
        {
            USART1_RX_STA=0;				//ǿ�Ʊ�ǽ������
        }
        HAL_UART_Receive_IT(&huart1,&uart1_RxBuffer,1);//�ٿ��������ж�
    }
    if(huart->Instance == USART2)	// �ж������ĸ����ڴ������ж�
    {
        flag_rx2=1;
        rx_cnt2=0;
        //�����յ������ݷ������usart1��������
        if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//�����Խ�������
        {
            usart2_rxbuf[usart2_rxcounter++]=uart2_RxBuffer;	//��¼���յ���ֵ
        } else
        {
            usart2_rxcounter=0;				//ǿ�Ʊ�ǽ������
        }
        HAL_UART_Receive_IT(&huart2,&uart2_RxBuffer,1);//�ٿ��������ж�
    }
    if(huart->Instance == USART3)	// �ж������ĸ����ڴ������ж�
    {
        flag_rx3=1;
        rx_cnt3=0;
        //�����յ������ݷ������usart1��������
        if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//�����Խ�������
        {
            USART3_RX_BUF[USART3_RX_STA++]=uart3_RxBuffer;	//��¼���յ���ֵ
        } else
        {
            USART3_RX_STA=0;				//ǿ�Ʊ�ǽ������
        }
        HAL_UART_Receive_IT(&huart3,&uart3_RxBuffer,1);//�ٿ��������ж�
    }

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        if(flag_rx1>0)//˵����������
        {
            rx_cnt1++;
            if(rx_cnt1>20)
            {
                flag_rx1=0;
                rx_cnt1=0;
                USART1_RX_OK=1;				//ǿ�Ʊ�ǽ������
            }
        }
        if(flag_rx3>0)//˵����������
        {
            rx_cnt3++;
            if(rx_cnt3>20)
            {
                flag_rx3=0;
                rx_cnt3=0;
                USART3_RX_OK=1;				//ǿ�Ʊ�ǽ������
            }
        }

        if(flag_rx2>0)//˵����������
        {
            rx_cnt2++;
            if(rx_cnt2>20)
            {
                flag_rx2=0;
                rx_cnt2=0;
                USART2_RX_OK=1;				//ǿ�Ʊ�ǽ������
            }
        }
    }
}


// �ض���printf����
int fputc(int ch,FILE *f)
{
    uint8_t temp[1]= {ch};
    //HAL_UART_Transmit(&huart1,temp,1,0xffff);
    uint32_t cnt=1000;
    while(CDC_Transmit_FS((uint8_t *)&ch, 1) == USBD_BUSY && cnt>0)cnt--;

    return 0;
}


//�������ģʽ�ȴ��ֶ�����
void Enter_ErrorMode(uint8_t mode)
{

    //while(1)
    {
        switch(mode) {
        case 0:
            printf("ESP8266��ʼ��ʧ�ܣ�\r\n");
            break;
        case 1:
            printf("ESP8266�����ȵ�ʧ�ܣ�\r\n");
            break;
        case 2:
            printf("ESP8266���� ������ʧ�ܣ�\r\n");
            break;
        case 3:
            printf("ESP8266 MQTT��½ʧ�ܣ�\r\n");
            break;
        case 4:
            printf("ESP8266 MQTT��������ʧ�ܣ�\r\n");
            break;
        default:
            printf("Nothing\r\n");
            break;
        }
        printf("������������");
        //HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
        HAL_Delay(200);
    }
}
//MQTT��ʼ������
void ES8266_MQTT_Init(uint8_t status1 )
{
    uint8_t status=status1;

    //��ʼ��
    if(status==0)
    {
        if(ESP8266_Init())
        {
            printf("ESP8266��ʼ���ɹ���\r\n");
            status++;
        }
        else Enter_ErrorMode(0);
    }
    //�����ȵ�
    if(status==1)
    {
        if(ESP8266_ConnectAP(WIFI_NAME,WIFI_PASSWD))
        {
            printf("ESP8266�����ȵ�ɹ���\r\n");
            status++;
        }
        else Enter_ErrorMode(1);
    }
    if(status ==2)
    {
        if(ESP8266_USERCFG("NULL",Username,Passward)!=0)
        {
            printf("����mqtt��Ϣ�ɹ���\r\n");
            status++;
        }
        else Enter_ErrorMode(2);


    }
    if(status ==3)
    {
        if(ESP8266_USERCLIENT(ClientID)!=0)
        {
            printf("����ClientID��Ϣ�ɹ���\r\n");
            status++;
        }
        else Enter_ErrorMode(2);


    }
    //�������ƶ�IOT������
    if(status==4)
    {
        if(ESP8266_ConnectIOT(ServerIP,ServerPort)!=0)
        {
            printf("ESP8266�������ƶ��������ɹ���\r\n");
            status++;

        }
        else Enter_ErrorMode(2);
    }



    //��������
    if(status==5)
    {
        if(ESP8266_Sub(S_TOPIC_NAME) != 0)
        {
            printf("ESP8266���ƶ�MQTT��������ɹ���\r\n");
            flag_connection=1;
        }
        else Enter_ErrorMode(4);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
