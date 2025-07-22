#include "my_include.h"
#include "CountSensor.h"
#include "stm32f10x.h"                  // Device header


#define F_SIZE      12 //������ʾ�����С ������ʾҪ��������ģ�ſ���
#define MyLCD_Show(m,n,p)     LCD_ShowString(LCD_GetPos_X(F_SIZE,m),LCD_GetPos_Y(16,n),p,F_SIZE,false)  //��ʾ���� 

#define POS_MENU_X                      1 //�˵���ʾ����ʼX ��'>'ָ���λ��
#define POS_MENU_Y                      0//�˵���ʾ��ʼ����
u16 remCurMenuId = 0xff;//��¼��ǰ�˵�λ��

u16 yPlace = 0; //��ʾλ��y����ֵ

void scanKeyAnddealKey(void);
void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str);
unsigned char makeSureLinkCount=0; //ȷ�����ӱ���
void displayOfCollectedData(void);//��ʾ�ɼ���������

unsigned int i;
char dis0[64];//Һ����ʾ�ݴ�����
char dis1[32];//Һ����ʾ�ݴ�����

u8 startCmdFlag = 0;//�Ƿ���������ʶ��
u16 setId = 0;//����id
u8 reMenukey =0;//��ֹ��һ�ν���˵� �൱���Ѿ������ȷ����
u16 outCount =0;//��ȥ����
u16 intCount =0;//�������

#define OPEN_ANGLE     60
#define CLOSE_ANGLE    -60
#define OPEN_DELAY  5
u16 openDjDelay = 0; //�򿪶��ʱ��

int main(void)
{
    USARTx_Init(USART1,9600);//���ڳ�ʼ��Ϊ9600
    USARTx_Init(USART2,9600);//���ڳ�ʼ��Ϊ9600
    USARTx_Init(USART3,115200);//���ڳ�ʼ��Ϊ9600

    My_KEY_Init();//��ʼ������
    My_LED_Init(); //�����ʼ�� 
	
    My_SG90_Init(0 ,TIM2,TIM_CH_2);//�����ʼ�� -90��90��
    delay_ms(50); 
    
    My_SG90_SetAngle(0,0); //����ת������
    delay_ms(100);
    My_SG90_SetAngle(0,CLOSE_ANGLE); //����ת������
    
    LCD_Init();   //tft��ʼ��    
    LCD_Clear(Color16_BLACK);//��ȫ��    
    BACK_COLOR=Color16_BLACK;FRONT_COLOR=Color16_RED;	 //������ʾ��ɫ    

    My_Menu_Init();//��ʼ���˵�

   while(1)
    {           
		
        if(myReadFlag_tick == true ) //��ʱ��ȡʱ�䵽
        {
            myReadFlag_tick = false; //�����־   

            scanKeyAnddealKey();//����ɨ�輰����            
            switch(My_Menu_GetSelectID())//��ȡ�˵�ID
            {
                case MENU_ID_ADD_NC://///////////////////////////////δ����˵� ����������ʶ��
                        if(remCurMenuId != MENU_ID_ADD_NC)
                        {
                            yPlace = 0;//��ʾλ��
                            FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ    
                            MyLCD_Show(3,yPlace++,"��ӭʹ��");//Һ����ʾ�ַ��� 
                            MyLCD_Show(3,yPlace++,"����ע��ʶ��ϵͳ");//��ʾ  
                        }
                       if(KeyIsPress(KEY_1)) //���������л�ģʽ
                        {
                            shiBieFaceCmd(); //ʶ������ �޳�ʱ 
                            startCmdFlag = 1;//��������ʶ��   
                        }
                     if(KeyIsPress(KEY_2)) //���������л�ģʽ
                        {
                            cancelCurFaceCmd();//ȡ����ǰ����             
                            startCmdFlag = 0;//��������ʶ��  
                        }                         
                        if(startCmdFlag == 1)//��������ʶ��
                        {
                            yPlace=2;                            
                            FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ    
                            MyLCD_Show(3,yPlace++,"����ʶ����...      ");//Һ����ʾ�ַ���                        
                            if(getFaceId() != 0 )//����ʶ��ɹ�
                            {
                                startCmdFlag = 0;//��ձ�־
                                sprintf(dis0,"��������Id:%02d  ",getFaceId());
                                MyLCD_Show(3,yPlace++,dis0);//Һ����ʾ�ַ���                                     
                                /****��������ȵ�*****/
                                openDjDelay = OPEN_DELAY; //�򿪶��ʱ��
                                intCount++;//��������
                                for(i=0;i<10;i++)delay_ms(100);
                            }
                            else
                            {
                                MyLCD_Show(3,yPlace++,"��������Id:**  ");//Һ����ʾ�ַ���   
                            }
                        }
                        else
                        {
                            yPlace=2;
                            FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ                               
                            MyLCD_Show(3,yPlace++,"K1����ʶ�� K2ȡ��");//Һ����ʾ�ַ���                            
                            MyLCD_Show(3,yPlace++,"                 ");//Һ����ʾ�ַ���                           
                        }
                        FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ  
                        sprintf(dis0,"����:%03d ����:%03d  ",intCount,outCount);
                        MyLCD_Show(3,6,dis0);//Һ����ʾ�ַ���  
                    break;
                case MENU_ID_ADD_X01:////////////////////////////////����ʶ��¼��ע��              
                       if(KeyIsPress(KEY_1)) //���������л�ģʽ
                        {
                            sendInputFaceCmd(); //ʶ������ �޳�ʱ 
                            startCmdFlag = 1;//��������ʶ��   
                        }   
                        if(startCmdFlag == 1)//��������ʶ��
                        {
                            yPlace=2;       
                            FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ    
                            MyLCD_Show(2,yPlace++,"����ע����...  ");//Һ����ʾ�ַ���                        
                            if(getFaceId() != 0 )//����ʶ��ɹ�
                            {
                                startCmdFlag = 0;//��ձ�־
                                sprintf(dis0,"��������Id:%02d  ",getFaceId());
                                MyLCD_Show(2,yPlace++,dis0);//Һ����ʾ�ַ��� 
                                for(i=0;i<10;i++)delay_ms(100);
                            }
                            else
                            {
                                MyLCD_Show(2,yPlace++,"��������Id:**  ");//Һ����ʾ�ַ���        
                            }                       
                        }
                        else
                        {
                            yPlace=2;       
                            FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ    
                            MyLCD_Show(2,yPlace++,"K1��������ע�� ");//Һ����ʾ�ַ���
                            MyLCD_Show(2,yPlace++,"                ");//Һ����ʾ�ַ���                                 
                        }
                    break;
                case MENU_ID_ADD_X02:////////////////////////////////����ʶ��ɾ������ID
                        if(remCurMenuId != MENU_ID_ADD_X02)
                        {
                            setId = 1;//Ҫ���õ�ID
                            FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ  
                            MyLCD_Show(3,2,"K1:+ K2:- K3:ȷ��");//Һ����ʾ�ַ���   
                        }
                        if(KeyIsPress(KEY_1)) //���������л�ģʽ
                        {
                            if(setId<99)setId++;//++ 
                        }
                        if(KeyIsPress(KEY_2)) //���������л�ģʽ
                        {
                            if(setId>1)setId--;//++ 
                        } 
                        if(KeyIsPress(KEY_3)&&reMenukey == 1) //���������л�ģʽ
                        {
                            deleteFaceIdCmd( setId ); //ɾ��ĳID������Ϣ
                            for(i=0;i<5;i++)delay_ms(100);
                            FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ  
                            if(getFaceCmdkRes() != 0 ) //�з��� 
                            {MyLCD_Show(3,3,"ɾ���ɹ�!    ");}//Һ����ʾ�ַ���                             
                            else MyLCD_Show(3,3,"ɾ��ʧ��!    ");//Һ����ʾ�ַ��� 
                            for(i=0;i<15;i++)delay_ms(100);
                        }              
                        FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ  
                        sprintf(dis0,"Ҫɾ��Id:%02d  ",setId);
                        MyLCD_Show(3,3,dis0);//Һ����ʾ�ַ���    
                        reMenukey = 1;//��λ
                    break;
                case MENU_ID_ADD_X03:///////////////////////////////����ʶ��ɾ������
                        FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ  
                        MyLCD_Show(3,2,"K1:ɾ��ȫ������ID");//Һ����ʾ�ַ���   
                        if(KeyIsPress(KEY_1)) //���������л�ģʽ
                        {
                             deleteAllFaceCmd(); //ɾ������face id
                            for(i=0;i<5;i++)delay_ms(100);
                            FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ  
                            if(getFaceCmdkRes() != 0 ) //�з��� 
                            {MyLCD_Show(3,3,"ɾ���ɹ�!    ");}//Һ����ʾ�ַ���                             
                            else MyLCD_Show(3,3,"ɾ��ʧ��!    ");//Һ����ʾ�ַ��� 
                            for(i=0;i<15;i++)delay_ms(100);
                        }                              
                    break;
                case MENU_ID_ADD_X04:///////////////////////////////����ʶ�����
                       if(KeyIsPress(KEY_1)) //���������л�ģʽ
                        {
                            shiBieFaceCmd(); //ʶ������ �޳�ʱ 
                            startCmdFlag = 1;//��������ʶ��   
                        }
                     if(KeyIsPress(KEY_2)) //���������л�ģʽ
                        {
                            cancelCurFaceCmd();//ȡ����ǰ����             
                            startCmdFlag = 0;//��������ʶ��  
                        }                         
                        if(startCmdFlag == 1)//��������ʶ��
                        {
                            yPlace=2;                            
                            FRONT_COLOR=Color16_YELLOW;	 //������ʾ��ɫ    
                            MyLCD_Show(3,yPlace++,"����ʶ����...      ");//Һ����ʾ�ַ���                        
                            if(getFaceId() != 0 )//����ʶ��ɹ�
                            {
                                startCmdFlag = 0;//��ձ�־
                                sprintf(dis0,"��������Id:%02d  ",getFaceId());
                                MyLCD_Show(3,yPlace++,dis0);//Һ����ʾ�ַ���                                     
                                /****��������ȵ�*****/
                                
                                for(i=0;i<15;i++)delay_ms(100);
                            }
                            else
                            {
                                MyLCD_Show(3,yPlace++,"��������Id:**  ");//Һ����ʾ�ַ���   
                            }
                        }
                        else
                        {
                            yPlace=2;
                            FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ                               
                            MyLCD_Show(3,yPlace++,"K1����ʶ�� K2ȡ��");//Һ����ʾ�ַ���                            
                            MyLCD_Show(3,yPlace++,"                 ");//Һ����ʾ�ַ���                           
                        }
                default:break;
            }
            if(remCurMenuId != My_Menu_GetSelectID())//�л��˵�ѡ��� ��ʼ����Ӧ����
            {
                remCurMenuId = My_Menu_GetSelectID();  
                cancelCurFaceCmd();//ȡ����ǰ����                
                startCmdFlag = 0;//��������ʶ��  
                reMenukey=0;//��ֹ��һ�ν���˵� �൱���Ѿ������ȷ����
            }            
        }
                
        if(mySendFlag_tick == true )//��ʱ����ʱ�䵽
        {
            mySendFlag_tick = false;//�����־   
            if(openDjDelay >0)//�����������ʱ 
            {
                openDjDelay--;//����ʱ
                My_SG90_SetAngle(0,OPEN_ANGLE); //����ת������
            }
            else {My_SG90_SetAngle(0,CLOSE_ANGLE);}//����ת������            
        }
        My_UartMessage_Process();//����������        
    }
}

void scanKeyAnddealKey(void)
{
    My_KeyScan();//����ɨ��

    if(selectedMenu!=NULL)
    {
        if(KeyIsPress(KEY_1))//���������ϼ�
        {
            My_Menu_SelectUp(true);
        }
        if(KeyIsPress(KEY_2))//���������ϼ�
        {
            My_Menu_SelectDown(true);
        }
        if(KeyIsPress(KEY_3))//������OK��
        {
            My_Menu_Into(true);
        }
        if(KeyIsPress(KEY_4))
        {
//            workState_menu=0;//�ص���ʼ״̬
            My_Menu_Back(true);
        }
    }
    else if(KeyIsPress(KEY_4))
    {
        My_Menu_Into(true);
    }
    else if(KeyIsPress(KEY_5))
    {
        outCount++;//��ȥ����
        openDjDelay = OPEN_DELAY; //�򿪶��ʱ��
    }
}

void My_Menu_Display(void)
{    
    static u8 remBack = 0xff;//�״��Ƴ��˵�
    static u8 firstBack = 0;//�״��Ƴ��˵�    
    if(selectedMenu==NULL)//����Ѿ��˳��˵�
    {
        firstBack = 0;//��¼��ǰ����ģʽ
        if(remBack != firstBack) //�״ν���������
        {remBack = firstBack;LCD_Clear(Color16_BLACK);}//��ȫ��   
        return;//�˳�����
    }
    else
    {
        firstBack = 1;//��¼��ǰ����ģʽ
        if(remBack != firstBack) //�״ν���������
        {remBack = firstBack;LCD_Clear(Color16_BLACK);}//��ȫ��    
        
    }    
    FRONT_COLOR = Color16_YELLOW;//����ǰ��ɫ      
    if(My_Menu_UnderBottom())//���һ��˵� ˵��������û��ѡ����ֻ�б���
    {
        MyLCD_Show(POS_MENU_X+1,POS_MENU_Y,(char *)*(selectedMenu->contentPtrArray+selectedMenu->selectIndex));//Һ����ʾ�ַ���
        return;//�˳�����
    }      
    MyLCD_Show(POS_MENU_X+1,POS_MENU_Y,(char *)selectedMenu->title);//���˵� ��һ���˵�����
    
    for(i=0;i<selectedMenu->itemCount;i++)//�ü��˵�ѡ��
    {
        if(i==selectedMenu->selectIndex)
        {
            FRONT_COLOR = Color16_BLUE;//����ǰ��ɫ
            MyLCD_Show(POS_MENU_X,i+POS_MENU_Y+1,">");//Һ����ʾ�ַ���
            MyLCD_Show(POS_MENU_X+1,POS_MENU_Y+1+i,(char *)*(selectedMenu->contentPtrArray+i));//Һ����ʾ�ַ���
            FRONT_COLOR = Color16_WHITE;//����ǰ��ɫ��������ɫ��Ϊ��ɫ
        }
        else
        {
            FRONT_COLOR=Color16_WHITE;	 //������ʾ��ɫ  
            MyLCD_Show(POS_MENU_X,i+POS_MENU_Y+1," ");//Һ����ʾ�ַ���
            MyLCD_Show(POS_MENU_X+1,POS_MENU_Y+1+i,(char *)*(selectedMenu->contentPtrArray+i));//Һ����ʾ�ַ���  
        }
    }
}

void My_Menu_Clear(void)
{
    LCD_Clear(Color16_BLACK);//��ȫ�� ���ز˵� ֱ������
}