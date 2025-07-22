#include "esp8266_at.h"
//#include "utils_hmac.h"   //包含需要的头文件
//usart2发送和接收数组
uint8_t usart2_txbuf[USART2_MAX_RECV_LEN];
uint8_t usart2_rxbuf[USART2_MAX_RECV_LEN];
uint8_t usart2_rxone[1];
uint16_t usart2_rxcounter;


volatile uint16_t MQTT_TxLen;


char ClientID[128];                                          //存放客户端ID的缓冲区
int  ClientID_len;                                           //存放客户端ID的长度

char Username[128];                                          //存放用户名的缓冲区
int  Username_len;											 //存放用户名的长度

char Passward[256];                                          //存放密码的缓冲区
int  Passward_len;											 //存放密码的长度

char ServerIP[128];                                          //存放服务器IP或是域名
int  ServerPort;                                             //存放服务器的端口号

/*----------------------------------------------------------*/
/*函数名：阿里云初始化参数，得到客户端ID，用户名和密码      */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void IoT_Parameter_Init(void)
{	
	char temp[128];                                                       //计算加密的时候，临时使用的缓冲区

	memset(ClientID,128,0);                                               //客户端ID的缓冲区全部清零
	sprintf(ClientID,"%s",DEVICENAME);  //构建客户端ID，并存入缓冲区
	ClientID_len = strlen(ClientID);                                      //计算客户端ID的长度
	
	memset(Username,128,0);                                               //用户名的缓冲区全部清零
	sprintf(Username,"%s",PRODUCTKEY);                      //构建用户名，并存入缓冲区
	Username_len = strlen(Username);                                      //计算用户名的长度
	
	memset(Passward,128,0);                                                                      //临时缓冲区全部清零
	memcpy(Passward,
	"version=2018-10-31&res=products%2F8fhVQ062Bz%2Fdevices%2Feq0&et=1806510008&method=md5&sign=kT91EEy0YH62jgp4APN0ZQ%3D%3D", 
	strlen("version=2018-10-31&res=products%2F8fhVQ062Bz%2Fdevices%2Feq0&et=1806510008&method=md5&sign=kT91EEy0YH62jgp4APN0ZQ%3D%3D"));     //构建加密时的明文   
	//utils_hmac_sha1(temp,strlen(temp),Passward,DEVICESECRE,DEVICESECRE_LEN);                 //以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
	Passward_len = strlen(Passward);                                                         //计算用户名的长度
	
	memset(ServerIP,128,0);  
	sprintf(ServerIP,"mqtts.heclouds.com",PRODUCTKEY);                    //构建服务器域名
	//sprintf(ServerIP,"106.54.182.59");                    //构建服务器域名
	ServerPort = 1883;                                                                       //服务器端口号1883
	
	printf("服 务 器：%s:%d\r\n",ServerIP,ServerPort); //串口输出调试信息
	printf("客户端ID：%s\r\n",ClientID);               //串口输出调试信息
	printf("用 户 名：%s\r\n",Username);               //串口输出调试信息
	printf("密    码：%s\r\n",Passward);               //串口输出调试信息
}

//串口1发送一个字节
static void usart2_SendOneByte(uint8_t val)
{
	((UART_HandleTypeDef *)&huart2)->Instance->DR = ((uint16_t)val & (uint16_t)0x01FF);
	while((((UART_HandleTypeDef *)&huart2)->Instance->SR&0X40)==0);//等待发送完成
}


//向ESP8266发送定长数据
void ESP8266_ATSendBuf(uint8_t* buf,uint16_t len)
{
	memset(usart2_rxbuf,0, 256);
	
	//每次发送前将接收串口接收总数置0,为了接收
	usart2_rxcounter = 0;	
	
	//定长发送
	HAL_UART_Transmit(&huart2,(uint8_t *)buf,len,0xFFFF);
}

//向ESP8266发送字符串
void ESP8266_ATSendString(char* str)
{
  memset(usart2_rxbuf,0, 256);
	
	//每次发送前将接收串口接收总数置0,为了接收
	usart2_rxcounter = 0;	
	printf("SendCmd :%s\r\n",str);
	//发送方法1
	while(*str)		usart2_SendOneByte(*str++);
	
	//发送法法2
//	HAL_UART_Transmit(&huart1,(uint8_t *)str,strlen(str),0xFFFF);
}

//退出透传
void ESP8266_ExitUnvarnishedTrans(void)
{
	ESP8266_ATSendString("+++");HAL_Delay(50);
	ESP8266_ATSendString("+++");HAL_Delay(50);	
}

//查找字符串中是否包含另一个字符串
uint8_t FindStr(char* dest,char* src,uint16_t retry_nms)
{
	retry_nms/=10;                   //超时时间
		HAL_Delay(200);
	while(strstr(usart2_rxbuf,src)==0 && retry_nms--)//等待串口接收完毕或超时退出
	{		
		HAL_Delay(10);
	}
	printf("Echo:%s\r\n",usart2_rxbuf);
	if(retry_nms) return 1;                       

	return 0; 
}
//查找字符串中是否包含另一个字符串
uint8_t FindStr1(char* dest,char* src,char* src1,uint16_t retry_nms)
{
	retry_nms/=10;                   //超时时间
		HAL_Delay(200);
	while(strstr(usart2_rxbuf,src)==0 && strstr(usart2_rxbuf,src1)==0  && retry_nms--)//等待串口接收完毕或超时退出
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
 * 功能：检查ESP8266是否正常
 * 参数：None
 * 返回值：ESP8266返回状态
 *        非0 ESP8266正常
 *        0 ESP8266有问题  
 */
uint8_t ESP8266_Check(void)
{
	uint8_t check_cnt=5;
	while(check_cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf)); 	 //清空接收缓冲
		ESP8266_ATSendString("AT\r\n");     		 			//发送AT握手指令	
		if(FindStr((char*)usart2_rxbuf,"OK",200) != 0)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * 功能：初始化ESP8266
 * 参数：None
 * 返回值：初始化结果，非0为初始化成功,0为失败
 */
uint8_t ESP8266_Init(void)
{
	//清空发送和接收数组
	memset(usart2_txbuf,0,sizeof(usart2_txbuf));
	memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));
	
	ESP8266_ExitUnvarnishedTrans();		//退出透传
	HAL_Delay(500);
	ESP8266_ATSendString("AT+RST\r\n");
	HAL_Delay(800);
	if(ESP8266_Check()==0)              //使用AT指令检查ESP8266是否存在
	{
		return 0;
	}
	ESP8266_Restore();HAL_Delay(800);
	memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));    //清空接收缓冲
	ESP8266_ATSendString("ATE0\r\n");     	//关闭回显 
	if(FindStr((char*)usart2_rxbuf,"OK",500)==0)  //设置不成功
	{
			return 0;      
	}
	return 1;                         //设置成功
}

/**
 * 功能：恢复出厂设置
 * 参数：None
 * 返回值：None
 * 说明:此时ESP8266中的用户设置将全部丢失回复成出厂状态
 */
void ESP8266_Restore(void)
{
	//ESP8266_ExitUnvarnishedTrans();          	//退出透传
  //HAL_Delay(500);
	//ESP8266_ATSendString("AT+RESTORE\r\n");		//恢复出厂 	
	//HAL_Delay(500);
}

/**
 * 功能：连接热点
 * 参数：
 *         ssid:热点名
 *         pwd:热点密码
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. WIFI名和密码不正确
 *         2. 路由器连接设备太多,未能给ESP8266分配IP
 */
uint8_t ESP8266_ConnectAP(char* ssid,char* pswd)
{
	uint8_t cnt=5;
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));     
		ESP8266_ATSendString("AT+CWMODE=1\r\n");              //设置为STATION模式	
		if(FindStr((char*)usart2_rxbuf,"OK",200) != 0)
		{
			break;
		}             		
	}
 

	cnt=2;
	while(cnt--)
	{                    
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲
		sprintf((char*)usart2_txbuf,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pswd);//连接目标AP
		ESP8266_ATSendString((char*)usart2_txbuf);	
		if(FindStr((char*)usart2_rxbuf,"WIFI GOT IP",20000)!=0)                      //连接成功且分配到IP
		{
			return 1;
		}
	}
	return 0;
}

//开启透传模式
static uint8_t ESP8266_OpenTransmission(void)
{
	//设置透传模式
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
 
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲
		sprintf((char*)usart2_txbuf,"AT+MQTTUSERCFG=0\,1\,\"%s\"\,\"%s\"\,\"%s\"\,0\,0\,\"\"\r\n",clientid,user,pswd);//连接目标AP
		ESP8266_ATSendString((char*)usart2_txbuf);	
		if(FindStr((char*)usart2_rxbuf,"OK",80)!=0)                      //连接成功且分配到IP
		{
			return 1;
		}
 
	return 0;
}
uint8_t ESP8266_USERCLIENT(char* clientid)
{
             
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲
		sprintf((char*)usart2_txbuf,"AT+MQTTCLIENTID=0,\"%s\"\r\n",clientid );//连接目标AP
		ESP8266_ATSendString((char*)usart2_txbuf);	
		if(FindStr((char*)usart2_rxbuf,"OK",80)!=0)                      //连接成功且分配到IP
		{
			return 1;
		}
 
	return 0;
}

uint8_t ESP8266_ConnectIOT( char* ip,uint16_t port)
{
	uint8_t cnt;
   
 
	//连接服务器
	cnt=1;
	while(cnt--)
	{
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲   
 
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
   
 
	//连接服务器
	cnt=2;
	while(cnt--)
	{
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲   
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

							//发送OneJson格式
//								sprintf(c_Str,"{\"id\":\"1\"\,\"params\":{\"t\":{\"value\":%d}\,\"h\":{\"value\":%d}\,\"pow\":{\"value\":%d}\,\"CO2\":{\"value\":%d}}}",
//												(int)temperature,humidity,CC.power_mW,co2Data);
//      
//                ESP8266_Pub(P_TOPIC_NAME,c_Str);
							//发送数据流
//														sprintf(c_Str,"{\"id\":1\,\"dp\":{\"t\":[{\"v\":%d}]\,\"h\":[{\"v\":%d}]\,\"pow\":[{\"v\":%d}]\,\"CO2\":[{\"v\":%d}]}}",
//												(int)temperature,humidity,CC.power_mW,co2Data);
//							ESP8266_Pub(P_TOPIC_NAME_DP,c_Str);
uint8_t ESP8266_Pub( char* topic,char * payload)
{
	uint8_t cnt;
   
 
 


		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲   
 //AT+MQTTSUB=0,"/sys/test",1AT+MQTTPUB=0,"testtopic/123","上报的json数据",0,0
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
 * 功能：使用指定协议(TCP/UDP)连接到服务器
 * 参数：
 *         mode:协议类型 "TCP","UDP"
 *         ip:目标服务器IP
 *         port:目标是服务器端口号
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. 远程服务器IP和端口号有误
 *         2. 未连接AP
 *         3. 服务器端禁止添加(一般不会发生)
 */
uint8_t ESP8266_ConnectServer(char* mode,char* ip,uint16_t port)
{
	uint8_t cnt;
   
	ESP8266_ExitUnvarnishedTrans();                   //多次连接需退出透传
	HAL_Delay(500);
	ESP8266_ExitUnvarnishedTrans();                   //多次连接需退出透传
	HAL_Delay(500);
	//连接服务器
	cnt=2;
	while(cnt--)
	{
		memset(usart2_txbuf,0,sizeof(usart2_txbuf));//清空发送缓冲
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf));//清空接收缓冲   
		sprintf((char*)usart2_txbuf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
		ESP8266_ATSendString((char*)usart2_txbuf);		HAL_Delay(200);
		if(FindStr((char*)usart2_rxbuf,"CONNECT",8000) !=0 )
		{
			break;
		}
	}
	if(cnt == 0) 
		return 0;
	
	//设置透传模式
	if(ESP8266_OpenTransmission()==0) return 0;
	
	//开启发送状态
	cnt=2;
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf)); //清空接收缓冲   
		ESP8266_ATSendString("AT+CIPSEND\r\n");//开始处于透传发送状态
		if(FindStr((char*)usart2_rxbuf,">",200)!=0)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * 功能：主动和服务器断开连接
 * 参数：None
 * 返回值：
 *         连接结果,非0断开成功,0断开失败
 */
uint8_t DisconnectServer(void)
{
	uint8_t cnt;
	
	ESP8266_ExitUnvarnishedTrans();	//退出透传
	HAL_Delay(500);
	
	while(cnt--)
	{
		memset(usart2_rxbuf,0,sizeof(usart2_rxbuf)); //清空接收缓冲   
		ESP8266_ATSendString("AT+CIPCLOSE\r\n");//关闭链接

		if(FindStr((char*)usart2_rxbuf,"CLOSED",200)!=0)//操作成功,和服务器成功断开
		{
			break;
		}
	}
	if(cnt) return 1;
	return 0;
}
