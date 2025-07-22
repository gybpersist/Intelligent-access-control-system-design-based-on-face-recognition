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
//此处根据自己的wifi作调整
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
    //先把PA12拉低再拉高，利用D+模拟USB的拔插动作
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



uint8_t uart1_RxBuffer; //接收中断缓冲
uint8_t uart2_RxBuffer; //接收中断缓冲
uint8_t uart3_RxBuffer; //接收中断缓冲
#define USART1_MAX_RECV_LEN 64 // 串口缓存大小
uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN]; //缓存数组
uint16_t USART1_RX_STA=0;// 串口1 接收完成标志和数据计算量
uint16_t USART1_RX_OK=0;


//#define USART2_MAX_RECV_LEN 32 // 串口缓存大小
//uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN]; //缓存数组
uint16_t USART2_RX_STA=0;// 串口1 接收完成标志和数据计算量
uint16_t USART2_RX_OK=0;

#define USART3_MAX_RECV_LEN 32 // 串口缓存大小
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; //缓存数组
uint16_t USART3_RX_STA=0;// 串口1 接收完成标志和数据计算量
uint16_t USART3_RX_OK=0;
uint8_t openCnt=0;//开门计时
#define OPEN_PERIOD   3 //开门周期

uint8_t flag_connection=0;// 0表示未连接 1表示有连接
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
    char s_temp[256];// 用于转换字符串使用
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
    OLED_Init();//初始化显示屏
    OLED_Clear();
    OLED_ShowStringCN(16,0,(unsigned char*)"智能门禁",16);//显示系统开始界面
    OLED_Refresh();

    HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart1_RxBuffer, 1);
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart2_RxBuffer, 1);
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&uart3_RxBuffer, 1);
    HAL_TIM_Base_Start_IT(&htim2);// 开启定时器 用来判断串口信号结束



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

            if(USART1_RX_BUF[0]==0xef && USART1_RX_BUF[1]==0xaa)//获取到TX510返回的数据
            {
                if(USART1_RX_BUF[7]==0x12)// 验证ID
                {
                    if(USART1_RX_BUF[8]==0x0 && USART1_RX_BUF[6]>0x02)//验证通过
                    {
                        OLED_Clear();

                        HAL_GPIO_WritePin(RELAY_GPIO_Port,RELAY_Pin,GPIO_PIN_SET);
                        OLED_ShowStringCN(0,16,(unsigned char*)"验证成功",16);
											sprintf(s_temp,"验证成功,ID:%x%x",USART1_RX_BUF[9],USART1_RX_BUF[10]);
												HAL_UART_Transmit(&huart3,s_temp,strlen(s_temp),0xffff);
											
                        sprintf(s_temp,"ID:%x%x",USART1_RX_BUF[9],USART1_RX_BUF[10]);
                        OLED_ShowString(0,32,(unsigned char*)s_temp,16,1);
                        OLED_Refresh();
                        HAL_Delay(2000);                        
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"智能门禁",16);//显示系统开始界面
                        OLED_Refresh();
               //发送数据流
								sprintf(s_temp,"{\"id\":\"1\"\,\"params\":{\"myid\":{\"value\":%d}\}}",
												(uint16_t)USART1_RX_BUF[9]*256+USART1_RX_BUF[10]);
      
                ESP8266_Pub(P_TOPIC_NAME,s_temp);
                    } else {

                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"验证失败",16);
											HAL_UART_Transmit(&huart3,"验证失败",strlen("验证失败"),0xffff);
                        OLED_Refresh();
                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"智能门禁",16);//显示系统开始界面
                        OLED_Refresh();
                    }
                } else if(USART1_RX_BUF[7]==0x13)// 注册ID
                {
                    if(USART1_RX_BUF[8]==0x0 && USART1_RX_BUF[6]>0x02)//注册通过
                    {
                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"注册成功",16);
											HAL_UART_Transmit(&huart3,"注册成功",strlen("注册成功"),0xffff);
                        sprintf(s_temp,"ID:%x%x",USART1_RX_BUF[9],USART1_RX_BUF[10]);
                        OLED_ShowString(0,32,(unsigned char*)s_temp,16,1);
                        OLED_Refresh();

                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"智能门禁",16);//显示系统开始界面
                        OLED_Refresh();

                    } else {
                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"注册失败",16);
                        OLED_Refresh();

											HAL_UART_Transmit(&huart3,"注册失败",strlen("注册失败"),0xffff);
                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"智能门禁",16);//显示系统开始界面
                        OLED_Refresh();
                    }
                } else if(USART1_RX_BUF[7]==0x21)// 删除所有ID
                {
                    if(USART1_RX_BUF[8]==0x0 && USART1_RX_BUF[6]==0x02)//删除通过
                    {
                        OLED_Clear();
                        OLED_ShowStringCN(0,16,(unsigned char*)"删除成功",16);
                        OLED_Refresh();

                        HAL_Delay(2000);
                        OLED_Clear();
                        OLED_ShowStringCN(16,0,(unsigned char*)"智能门禁",16);//显示系统开始界面
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
    if(huart->Instance == USART1)	// 判断是由哪个串口触发的中断
    {
        flag_rx1=1;
        rx_cnt1=0;
        //将接收到的数据放入接收usart1接收数组
        if(USART1_RX_STA<USART1_MAX_RECV_LEN)	//还可以接收数据
        {
            USART1_RX_BUF[USART1_RX_STA++]=uart1_RxBuffer;	//记录接收到的值
        } else
        {
            USART1_RX_STA=0;				//强制标记接收完成
        }
        HAL_UART_Receive_IT(&huart1,&uart1_RxBuffer,1);//再开启接收中断
    }
    if(huart->Instance == USART2)	// 判断是由哪个串口触发的中断
    {
        flag_rx2=1;
        rx_cnt2=0;
        //将接收到的数据放入接收usart1接收数组
        if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//还可以接收数据
        {
            usart2_rxbuf[usart2_rxcounter++]=uart2_RxBuffer;	//记录接收到的值
        } else
        {
            usart2_rxcounter=0;				//强制标记接收完成
        }
        HAL_UART_Receive_IT(&huart2,&uart2_RxBuffer,1);//再开启接收中断
    }
    if(huart->Instance == USART3)	// 判断是由哪个串口触发的中断
    {
        flag_rx3=1;
        rx_cnt3=0;
        //将接收到的数据放入接收usart1接收数组
        if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//还可以接收数据
        {
            USART3_RX_BUF[USART3_RX_STA++]=uart3_RxBuffer;	//记录接收到的值
        } else
        {
            USART3_RX_STA=0;				//强制标记接收完成
        }
        HAL_UART_Receive_IT(&huart3,&uart3_RxBuffer,1);//再开启接收中断
    }

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        if(flag_rx1>0)//说明在收数据
        {
            rx_cnt1++;
            if(rx_cnt1>20)
            {
                flag_rx1=0;
                rx_cnt1=0;
                USART1_RX_OK=1;				//强制标记接收完成
            }
        }
        if(flag_rx3>0)//说明在收数据
        {
            rx_cnt3++;
            if(rx_cnt3>20)
            {
                flag_rx3=0;
                rx_cnt3=0;
                USART3_RX_OK=1;				//强制标记接收完成
            }
        }

        if(flag_rx2>0)//说明在收数据
        {
            rx_cnt2++;
            if(rx_cnt2>20)
            {
                flag_rx2=0;
                rx_cnt2=0;
                USART2_RX_OK=1;				//强制标记接收完成
            }
        }
    }
}


// 重定向printf函数
int fputc(int ch,FILE *f)
{
    uint8_t temp[1]= {ch};
    //HAL_UART_Transmit(&huart1,temp,1,0xffff);
    uint32_t cnt=1000;
    while(CDC_Transmit_FS((uint8_t *)&ch, 1) == USBD_BUSY && cnt>0)cnt--;

    return 0;
}


//进入错误模式等待手动重启
void Enter_ErrorMode(uint8_t mode)
{

    //while(1)
    {
        switch(mode) {
        case 0:
            printf("ESP8266初始化失败！\r\n");
            break;
        case 1:
            printf("ESP8266连接热点失败！\r\n");
            break;
        case 2:
            printf("ESP8266连接 服务器失败！\r\n");
            break;
        case 3:
            printf("ESP8266 MQTT登陆失败！\r\n");
            break;
        case 4:
            printf("ESP8266 MQTT订阅主题失败！\r\n");
            break;
        default:
            printf("Nothing\r\n");
            break;
        }
        printf("请重启开发板");
        //HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
        HAL_Delay(200);
    }
}
//MQTT初始化函数
void ES8266_MQTT_Init(uint8_t status1 )
{
    uint8_t status=status1;

    //初始化
    if(status==0)
    {
        if(ESP8266_Init())
        {
            printf("ESP8266初始化成功！\r\n");
            status++;
        }
        else Enter_ErrorMode(0);
    }
    //连接热点
    if(status==1)
    {
        if(ESP8266_ConnectAP(WIFI_NAME,WIFI_PASSWD))
        {
            printf("ESP8266连接热点成功！\r\n");
            status++;
        }
        else Enter_ErrorMode(1);
    }
    if(status ==2)
    {
        if(ESP8266_USERCFG("NULL",Username,Passward)!=0)
        {
            printf("配置mqtt信息成功！\r\n");
            status++;
        }
        else Enter_ErrorMode(2);


    }
    if(status ==3)
    {
        if(ESP8266_USERCLIENT(ClientID)!=0)
        {
            printf("配置ClientID信息成功！\r\n");
            status++;
        }
        else Enter_ErrorMode(2);


    }
    //连接中移动IOT服务器
    if(status==4)
    {
        if(ESP8266_ConnectIOT(ServerIP,ServerPort)!=0)
        {
            printf("ESP8266连接中移动服务器成功！\r\n");
            status++;

        }
        else Enter_ErrorMode(2);
    }



    //订阅主题
    if(status==5)
    {
        if(ESP8266_Sub(S_TOPIC_NAME) != 0)
        {
            printf("ESP8266中移动MQTT订阅主题成功！\r\n");
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
