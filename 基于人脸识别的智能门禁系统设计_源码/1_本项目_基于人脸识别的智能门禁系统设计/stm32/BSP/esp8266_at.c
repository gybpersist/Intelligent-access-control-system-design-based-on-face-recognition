#include "esp8266_at.h"
//#include "utils_hmac.h"   //������Ҫ��ͷ�ļ�
//usart2���ͺͽ�������
uint8_t usart2_txbuf[USART2_MAX_RECV_LEN];
uint8_t usart2_rxbuf[USART2_MAX_RECV_LEN];
uint8_t usart2_rxone[1];
uint16_t usart2_rxcounter;


volatile uint16_t MQTT_TxLen;


char ClientID[128];                                          //��ſͻ���ID�Ļ�����
int  ClientID_len;                                           //��ſͻ���ID�ĳ���

char Username[128];                                          //����û����Ļ�����
int  Username_len;											 //����û����ĳ���

char Passward[256];                                          //�������Ļ�����
int  Passward_len;											 //�������ĳ���

char ServerIP[128];                                          //��ŷ�����IP��������
int  ServerPort;                                             //��ŷ������Ķ˿ں�

/*----------------------------------------------------------*/
/*�������������Ƴ�ʼ���������õ��ͻ���ID���û���������      */
/*��  ������                                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void IoT_Parameter_Init(void)
{	
	char temp[128];                                                       //������ܵ�ʱ����ʱʹ�õĻ�����

	memset(ClientID,128,0);                                               //�ͻ���ID�Ļ�����ȫ������
	sprintf(ClientID,"%s",DEVICENAME);  //�����ͻ���ID�������뻺����
	ClientID_len = strlen(ClientID);                                      //����ͻ���ID�ĳ���
	
	memset(Username,128,0);                                               //�û����Ļ�����ȫ������
	sprintf(Username,"%s",PRODUCTKEY);                      //�����û����������뻺����
	Username_len = strlen(Username);                                      //�����û����ĳ���
	
	memset(Passward,128,0);                                                                      //��ʱ������ȫ������
	memcpy(Passward,
	"version=2018-10-31&res=products%2F8fhVQ062Bz%2Fdevices%2Feq0&et=1806510008&method=md5&sign=kT91EEy0YH62jgp4APN0ZQ%3D%3D", 
	strlen("version=2018-10-31&res=products%2F8fhVQ062Bz%2Fdevices%2Feq0&et=1806510008&method=md5&sign=kT91EEy0YH62jgp4APN0ZQ%3D%3D"));     //��������ʱ������   
	//utils_hmac_sha1(temp,strlen(temp),Passward,DEVICESECRE,DEVICESECRE_LEN);                 //��DeviceSecretΪ��Կ��temp�е����ģ�����hmacsha1���ܣ�����������룬�����浽��������
	Passward_len = strlen(Passward);                                                         //�����û����ĳ���
	
	memset(ServerIP,128,0);  
	sprintf(ServerIP,"mqtts.heclouds.com",PRODUCTKEY);                    //��������������
	//sprintf(ServerIP,"106.54.182.59");                    //��������������
	ServerPort = 1883;                                                                       //�������˿ں�1883
	
	printf("�� �� ����%s:%d\r\n",ServerIP,ServerPort); //�������������Ϣ
	printf("�ͻ���ID��%s\r\n",ClientID);               //�������������Ϣ
	printf("�� �� ����%s\r\n",Username);               //�������������Ϣ
	printf("��    �룺%s\r\n",Passward);               //�������������Ϣ
}

//����1����һ���ֽ�
static void usart2_SendOneByte(uint8_t val)
{
	((UART_HandleTypeDef *)&huart2)->Instance->DR = ((uint16_t)val & (uint16_t)0x01FF);
	while((((UART_HandleTypeDef *)&huart2)->Instance->SR&0X40)==0);//�ȴ��������
}


//��ESP8266���Ͷ�������
void ESP8266_ATSendBuf(uint8_t* buf,uint16_t len)
{
	memset(usart2_rxbuf,0, 256);
	
	//ÿ�η���ǰ�����մ��ڽ���������0,Ϊ�˽���
	usart2_rxcounter = 0;	
	
	//��������
	HAL_UART_Transmit(&huart2,(uint8_t *)buf,len,0xFFFF);
}

//��ESP8266�����ַ���
void ESP8266_ATSendString(char* str)
{
  memset(usart2_rxbuf,0, 256);
	
	//ÿ�η���ǰ�����մ��ڽ���������0,Ϊ�˽���
	usart2_rxcounter = 0;	
	printf("SendCmd :%s\r\n",str);
	//���ͷ���1
	while(*str)		usart2_SendOneByte(*str++);
	
	//���ͷ���2
//	HAL_UART_Transmit(&huart1,(uint8_t *)str,strlen(str),0xFFFF);
}

//�˳�͸��
void ESP8266_ExitUnvarnishedTrans(void)
{
	ESP8266_ATSendString("+++");HAL_Delay(50);
	ESP8266_ATSendString("+++");HAL_Delay(50);	
}

//�����ַ������Ƿ������һ���ַ���
uint8_t FindStr(char* dest,char* src,uint16_t retry_nms)
{
	retry_nms/=10;                   //��ʱʱ��
		HAL_Delay(200);
	while(strstr(usart2_rxbuf,src)==0 && retry_nms--)//�ȴ����ڽ�����ϻ�ʱ�˳�
	{		
		HAL_Delay(10);
	}
	printf("Echo:%s\r\n",usart2_rxbuf);
	if(retry_nms) return 1;                       

	return 0; 
}
//�����ַ������Ƿ������һ���ַ���
uint8_t FindStr1(char* dest,char* src,char* src1,uint16_t retry_nms)
{
	retry_nms/=10;                   //��ʱʱ��
		HAL_Delay(200);
	while(strstr(usart2_rxbuf,src)==0 && strstr(usart2_rxbuf,src1)==0  && retry_nms--)//�ȴ����ڽ�����ϻ�ʱ�˳�
	{		
		HAL_Delay(10);
	}
	printf("Echo:%s\r\n",usart2_rxbuf);
	if(retry_nms) 
	{
if(strstr(usart2_rxbuf,src))return 1;  
if(strstr(usart2_rxbuf,src))return 2;  
	}		
	
	                     

	return 0; 
}

/**
 * ���ܣ����ESP8266�Ƿ�����
 * ������None
 * ����ֵ��ESP8266����״̬
 *        ��0 ESP8266����
 *        0 ESP8266������  
 */
uint8_t ESP8266_Check(void)
{
	uint8_t check_cnt=5;
	while(check_cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf)); 	 //��ս��ջ���
		ESP8266_ATSendString("AT\r\n");     		 			//����AT����ָ��	
		if(FindStr((char*)usart2_rxbuf,"OK",200) != 0)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * ���ܣ���ʼ��ESP8266
 * ������None
 * ����ֵ����ʼ���������0Ϊ��ʼ���ɹ�,0Ϊʧ��
 */
uint8_t ESP8266_Init(void)
{
	//��շ��ͺͽ�������
	memset(usart2_txbuf,0,sizeof(usart2_txbuf));
	memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));
	
	ESP8266_ExitUnvarnishedTrans();		//�˳�͸��
	HAL_Delay(500);
	ESP8266_ATSendString("AT+RST\r\n");
	HAL_Delay(800);
	if(ESP8266_Check()==0)              //ʹ��ATָ����ESP8266�Ƿ����
	{
		return 0;
	}
	ESP8266_Restore();HAL_Delay(800);
	memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));    //��ս��ջ���
	ESP8266_ATSendString("ATE0\r\n");     	//�رջ��� 
	if(FindStr((char*)usart2_rxbuf,"OK",500)==0)  //���ò��ɹ�
	{
			return 0;      
	}
	return 1;                         //���óɹ�
}

/**
 * ���ܣ��ָ���������
 * ������None
 * ����ֵ��None
 * ˵��:��ʱESP8266�е��û����ý�ȫ����ʧ�ظ��ɳ���״̬
 */
void ESP8266_Restore(void)
{
	//ESP8266_ExitUnvarnishedTrans();          	//�˳�͸��
  //HAL_Delay(500);
	//ESP8266_ATSendString("AT+RESTORE\r\n");		//�ָ����� 	
	//HAL_Delay(500);
}

/**
 * ���ܣ������ȵ�
 * ������
 *         ssid:�ȵ���
 *         pwd:�ȵ�����
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. WIFI�������벻��ȷ
 *         2. ·���������豸̫��,δ�ܸ�ESP8266����IP
 */
uint8_t ESP8266_ConnectAP(char* ssid,char* pswd)
{
	uint8_t cnt=5;
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));     
		ESP8266_ATSendString("AT+CWMODE=1\r\n");              //����ΪSTATIONģʽ	
		if(FindStr((char*)usart2_rxbuf,"OK",200) != 0)
		{
			break;
		}             		
	}
 

	cnt=2;
	while(cnt--)
	{                    
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���
		sprintf((char*)usart2_txbuf,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pswd);//����Ŀ��AP
		ESP8266_ATSendString((char*)usart2_txbuf);	
		if(FindStr((char*)usart2_rxbuf,"WIFI GOT IP",20000)!=0)                      //���ӳɹ��ҷ��䵽IP
		{
			return 1;
		}
	}
	return 0;
}

//����͸��ģʽ
static uint8_t ESP8266_OpenTransmission(void)
{
	//����͸��ģʽ
	uint8_t cnt=2;
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));    
		ESP8266_ATSendString("AT+CIPMODE=1\r\n");  
		HAL_Delay(200);
		if(FindStr((char*)usart2_rxbuf,"OK",200)!=0)
		{	
			return 1;
		}
	}
	return 0;
}
uint8_t ESP8266_USERCFG(char* clientid,char* user,char* pswd)
{
 
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���
		sprintf((char*)usart2_txbuf,"AT+MQTTUSERCFG=0\,1\,\"%s\"\,\"%s\"\,\"%s\"\,0\,0\,\"\"\r\n",clientid,user,pswd);//����Ŀ��AP
		ESP8266_ATSendString((char*)usart2_txbuf);	
		if(FindStr((char*)usart2_rxbuf,"OK",80)!=0)                      //���ӳɹ��ҷ��䵽IP
		{
			return 1;
		}
 
	return 0;
}
uint8_t ESP8266_USERCLIENT(char* clientid)
{
             
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���
		sprintf((char*)usart2_txbuf,"AT+MQTTCLIENTID=0,\"%s\"\r\n",clientid );//����Ŀ��AP
		ESP8266_ATSendString((char*)usart2_txbuf);	
		if(FindStr((char*)usart2_rxbuf,"OK",80)!=0)                      //���ӳɹ��ҷ��䵽IP
		{
			return 1;
		}
 
	return 0;
}

uint8_t ESP8266_ConnectIOT( char* ip,uint16_t port)
{
	uint8_t cnt;
   
 
	//���ӷ�����
	cnt=1;
	while(cnt--)
	{
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���   
 
			sprintf((char*)usart2_txbuf,"AT+MQTTCONN=0,\"%s\",%d,1\r\n",ip,port);
		ESP8266_ATSendString((char*)usart2_txbuf);		HAL_Delay(200);
		if(FindStr((char*)usart2_rxbuf,"OK",8000) !=0 )
		{
			return 1;
		}
	}
	
	return 0;
}

uint8_t ESP8266_Sub( char* topic)
{
	uint8_t cnt;
   
 
	//���ӷ�����
	cnt=2;
	while(cnt--)
	{
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���   
 //AT+MQTTSUB=0,"/sys/test",1
			sprintf((char*)usart2_txbuf,"AT+MQTTSUB=0,\"%s\",1\r\n",topic);
		ESP8266_ATSendString((char*)usart2_txbuf);		HAL_Delay(200);

		if(FindStr((char*)usart2_rxbuf,"OK",300) !=0 )
		{
			return 1;
		}
	}
	
	return 0;
}

							//����OneJson��ʽ
//								sprintf(c_Str,"{\"id\":\"1\"\,\"params\":{\"t\":{\"value\":%d}\,\"h\":{\"value\":%d}\,\"pow\":{\"value\":%d}\,\"CO2\":{\"value\":%d}}}",
//												(int)temperature,humidity,CC.power_mW,co2Data);
//      
//                ESP8266_Pub(P_TOPIC_NAME,c_Str);
							//����������
//														sprintf(c_Str,"{\"id\":1\,\"dp\":{\"t\":[{\"v\":%d}]\,\"h\":[{\"v\":%d}]\,\"pow\":[{\"v\":%d}]\,\"CO2\":[{\"v\":%d}]}}",
//												(int)temperature,humidity,CC.power_mW,co2Data);
//							ESP8266_Pub(P_TOPIC_NAME_DP,c_Str);
uint8_t ESP8266_Pub( char* topic,char * payload)
{
	uint8_t cnt;
   
 
 


		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���   
 //AT+MQTTSUB=0,"/sys/test",1AT+MQTTPUB=0,"testtopic/123","�ϱ���json����",0,0
	//	sprintf((char*)usart2_txbuf,"AT+MQTTPUB=0,\"%s\",\"%s\",1,0\r\n",topic,payload);// +3);
		sprintf((char*)usart2_txbuf,"AT+MQTTPUBRAW=0,\"%s\",%d,0,0\r\n",topic,strlen(payload));// +3);
		ESP8266_ATSendString((char*)usart2_txbuf);		HAL_Delay(200);
	//		printf("payload:%s\r\n",payload);
		if(FindStr((char*)usart2_rxbuf,"OK",300) !=0 )
		{
//			
//			char d[3];
//		uint16_t streamLen = strlen(payload);
////		 d[0] = 0x03;
////		d[1] = (streamLen >> 8);
////		d[2] = (streamLen & 0xFF);
////			ESP8266_ATSendBuf(d,3);

			ESP8266_ATSendBuf(payload, strlen(payload));			printf("payload : %s \r\n",payload);
			if(FindStr((char*)usart2_rxbuf,"MQTTPUB:OK",8000) !=0 )
			{
			return 1;
			}
		}
	
	return 0;
}
/**
 * ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
 * ������
 *         mode:Э������ "TCP","UDP"
 *         ip:Ŀ�������IP
 *         port:Ŀ���Ƿ������˿ں�
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. Զ�̷�����IP�Ͷ˿ں�����
 *         2. δ����AP
 *         3. �������˽�ֹ���(һ�㲻�ᷢ��)
 */
uint8_t ESP8266_ConnectServer(char* mode,char* ip,uint16_t port)
{
	uint8_t cnt;
   
	ESP8266_ExitUnvarnishedTrans();                   //����������˳�͸��
	HAL_Delay(500);
	ESP8266_ExitUnvarnishedTrans();                   //����������˳�͸��
	HAL_Delay(500);
	//���ӷ�����
	cnt=2;
	while(cnt--)
	{
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//��շ��ͻ���
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//��ս��ջ���   
		sprintf((char*)usart2_txbuf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
		ESP8266_ATSendString((char*)usart2_txbuf);		HAL_Delay(200);
		if(FindStr((char*)usart2_rxbuf,"CONNECT",8000) !=0 )
		{
			break;
		}
	}
	if(cnt == 0) 
		return 0;
	
	//����͸��ģʽ
	if(ESP8266_OpenTransmission()==0) return 0;
	
	//��������״̬
	cnt=2;
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf)); //��ս��ջ���   
		ESP8266_ATSendString("AT+CIPSEND\r\n");//��ʼ����͸������״̬
		if(FindStr((char*)usart2_rxbuf,">",200)!=0)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * ���ܣ������ͷ������Ͽ�����
 * ������None
 * ����ֵ��
 *         ���ӽ��,��0�Ͽ��ɹ�,0�Ͽ�ʧ��
 */
uint8_t DisconnectServer(void)
{
	uint8_t cnt;
	
	ESP8266_ExitUnvarnishedTrans();	//�˳�͸��
	HAL_Delay(500);
	
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf)); //��ս��ջ���   
		ESP8266_ATSendString("AT+CIPCLOSE\r\n");//�ر�����

		if(FindStr((char*)usart2_rxbuf,"CLOSED",200)!=0)//�����ɹ�,�ͷ������ɹ��Ͽ�
		{
			break;
		}
	}
	if(cnt) return 1;
	return 0;
}
