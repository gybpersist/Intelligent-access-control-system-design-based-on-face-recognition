#include "my_include.h"
#include "CountSensor.h"
#include "stm32f10x.h"                  // Device header


#define F_SIZE      12 //定义显示字体大小 汉字显示要定义了字模才可以
#define MyLCD_Show(m,n,p)     LCD_ShowString(LCD_GetPos_X(F_SIZE,m),LCD_GetPos_Y(16,n),p,F_SIZE,false)  //显示函数 

#define POS_MENU_X                      1 //菜单显示的起始X 即'>'指向的位置
#define POS_MENU_Y                      0//菜单显示起始行数
u16 remCurMenuId = 0xff;//记录当前菜单位置

u16 yPlace = 0; //显示位置y坐标值

void scanKeyAnddealKey(void);
void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str);
unsigned char makeSureLinkCount=0; //确保链接变量
void displayOfCollectedData(void);//显示采集到的数据

unsigned int i;
char dis0[64];//液晶显示暂存数组
char dis1[32];//液晶显示暂存数组

u8 startCmdFlag = 0;//是否启动人脸识别
u16 setId = 0;//设置id
u8 reMenukey =0;//防止第一次进入菜单 相当于已经点击了确定键
u16 outCount =0;//出去计数
u16 intCount =0;//进入计数

#define OPEN_ANGLE     60
#define CLOSE_ANGLE    -60
#define OPEN_DELAY  5
u16 openDjDelay = 0; //打开舵机时间

int main(void)
{
    USARTx_Init(USART1,9600);//串口初始化为9600
    USARTx_Init(USART2,9600);//串口初始化为9600
    USARTx_Init(USART3,115200);//串口初始化为9600

    My_KEY_Init();//初始化输入
    My_LED_Init(); //输出初始化 
	
    My_SG90_Init(0 ,TIM2,TIM_CH_2);//舵机初始化 -90到90度
    delay_ms(50); 
    
    My_SG90_SetAngle(0,0); //控制转动方向
    delay_ms(100);
    My_SG90_SetAngle(0,CLOSE_ANGLE); //控制转动方向
    
    LCD_Init();   //tft初始化    
    LCD_Clear(Color16_BLACK);//清全屏    
    BACK_COLOR=Color16_BLACK;FRONT_COLOR=Color16_RED;	 //设置显示颜色    

    My_Menu_Init();//初始化菜单

   while(1)
    {           
		
        if(myReadFlag_tick == true ) //定时读取时间到
        {
            myReadFlag_tick = false; //清除标志   

            scanKeyAnddealKey();//按键扫描及处理            
            switch(My_Menu_GetSelectID())//获取菜单ID
            {
                case MENU_ID_ADD_NC://///////////////////////////////未进入菜单 可启动人脸识别
                        if(remCurMenuId != MENU_ID_ADD_NC)
                        {
                            yPlace = 0;//显示位置
                            FRONT_COLOR=Color16_WHITE;	 //设置显示颜色    
                            MyLCD_Show(3,yPlace++,"欢迎使用");//液晶显示字符串 
                            MyLCD_Show(3,yPlace++,"人脸注册识别系统");//显示  
                        }
                       if(KeyIsPress(KEY_1)) //按键按下切换模式
                        {
                            shiBieFaceCmd(); //识别命令 无超时 
                            startCmdFlag = 1;//启动人脸识别   
                        }
                     if(KeyIsPress(KEY_2)) //按键按下切换模式
                        {
                            cancelCurFaceCmd();//取消当前命令             
                            startCmdFlag = 0;//启动人脸识别  
                        }                         
                        if(startCmdFlag == 1)//启动人脸识别
                        {
                            yPlace=2;                            
                            FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色    
                            MyLCD_Show(3,yPlace++,"人脸识别中...      ");//液晶显示字符串                        
                            if(getFaceId() != 0 )//人脸识别成功
                            {
                                startCmdFlag = 0;//清空标志
                                sprintf(dis0,"人脸特征Id:%02d  ",getFaceId());
                                MyLCD_Show(3,yPlace++,dis0);//液晶显示字符串                                     
                                /****舵机动作等等*****/
                                openDjDelay = OPEN_DELAY; //打开舵机时间
                                intCount++;//进数增加
                                for(i=0;i<10;i++)delay_ms(100);
                            }
                            else
                            {
                                MyLCD_Show(3,yPlace++,"人脸特征Id:**  ");//液晶显示字符串   
                            }
                        }
                        else
                        {
                            yPlace=2;
                            FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色                               
                            MyLCD_Show(3,yPlace++,"K1启动识别 K2取消");//液晶显示字符串                            
                            MyLCD_Show(3,yPlace++,"                 ");//液晶显示字符串                           
                        }
                        FRONT_COLOR=Color16_WHITE;	 //设置显示颜色  
                        sprintf(dis0,"进数:%03d 出数:%03d  ",intCount,outCount);
                        MyLCD_Show(3,6,dis0);//液晶显示字符串  
                    break;
                case MENU_ID_ADD_X01:////////////////////////////////人脸识别录入注册              
                       if(KeyIsPress(KEY_1)) //按键按下切换模式
                        {
                            sendInputFaceCmd(); //识别命令 无超时 
                            startCmdFlag = 1;//启动人脸识别   
                        }   
                        if(startCmdFlag == 1)//启动人脸识别
                        {
                            yPlace=2;       
                            FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色    
                            MyLCD_Show(2,yPlace++,"人脸注册中...  ");//液晶显示字符串                        
                            if(getFaceId() != 0 )//人脸识别成功
                            {
                                startCmdFlag = 0;//清空标志
                                sprintf(dis0,"人脸特征Id:%02d  ",getFaceId());
                                MyLCD_Show(2,yPlace++,dis0);//液晶显示字符串 
                                for(i=0;i<10;i++)delay_ms(100);
                            }
                            else
                            {
                                MyLCD_Show(2,yPlace++,"人脸特征Id:**  ");//液晶显示字符串        
                            }                       
                        }
                        else
                        {
                            yPlace=2;       
                            FRONT_COLOR=Color16_WHITE;	 //设置显示颜色    
                            MyLCD_Show(2,yPlace++,"K1启动人脸注册 ");//液晶显示字符串
                            MyLCD_Show(2,yPlace++,"                ");//液晶显示字符串                                 
                        }
                    break;
                case MENU_ID_ADD_X02:////////////////////////////////人脸识别删除单个ID
                        if(remCurMenuId != MENU_ID_ADD_X02)
                        {
                            setId = 1;//要设置的ID
                            FRONT_COLOR=Color16_WHITE;	 //设置显示颜色  
                            MyLCD_Show(3,2,"K1:+ K2:- K3:确定");//液晶显示字符串   
                        }
                        if(KeyIsPress(KEY_1)) //按键按下切换模式
                        {
                            if(setId<99)setId++;//++ 
                        }
                        if(KeyIsPress(KEY_2)) //按键按下切换模式
                        {
                            if(setId>1)setId--;//++ 
                        } 
                        if(KeyIsPress(KEY_3)&&reMenukey == 1) //按键按下切换模式
                        {
                            deleteFaceIdCmd( setId ); //删除某ID人脸信息
                            for(i=0;i<5;i++)delay_ms(100);
                            FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色  
                            if(getFaceCmdkRes() != 0 ) //有反馈 
                            {MyLCD_Show(3,3,"删除成功!    ");}//液晶显示字符串                             
                            else MyLCD_Show(3,3,"删除失败!    ");//液晶显示字符串 
                            for(i=0;i<15;i++)delay_ms(100);
                        }              
                        FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色  
                        sprintf(dis0,"要删除Id:%02d  ",setId);
                        MyLCD_Show(3,3,dis0);//液晶显示字符串    
                        reMenukey = 1;//置位
                    break;
                case MENU_ID_ADD_X03:///////////////////////////////人脸识别删除所有
                        FRONT_COLOR=Color16_WHITE;	 //设置显示颜色  
                        MyLCD_Show(3,2,"K1:删除全部人脸ID");//液晶显示字符串   
                        if(KeyIsPress(KEY_1)) //按键按下切换模式
                        {
                             deleteAllFaceCmd(); //删除所有face id
                            for(i=0;i<5;i++)delay_ms(100);
                            FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色  
                            if(getFaceCmdkRes() != 0 ) //有反馈 
                            {MyLCD_Show(3,3,"删除成功!    ");}//液晶显示字符串                             
                            else MyLCD_Show(3,3,"删除失败!    ");//液晶显示字符串 
                            for(i=0;i<15;i++)delay_ms(100);
                        }                              
                    break;
                case MENU_ID_ADD_X04:///////////////////////////////人脸识别测试
                       if(KeyIsPress(KEY_1)) //按键按下切换模式
                        {
                            shiBieFaceCmd(); //识别命令 无超时 
                            startCmdFlag = 1;//启动人脸识别   
                        }
                     if(KeyIsPress(KEY_2)) //按键按下切换模式
                        {
                            cancelCurFaceCmd();//取消当前命令             
                            startCmdFlag = 0;//启动人脸识别  
                        }                         
                        if(startCmdFlag == 1)//启动人脸识别
                        {
                            yPlace=2;                            
                            FRONT_COLOR=Color16_YELLOW;	 //设置显示颜色    
                            MyLCD_Show(3,yPlace++,"人脸识别中...      ");//液晶显示字符串                        
                            if(getFaceId() != 0 )//人脸识别成功
                            {
                                startCmdFlag = 0;//清空标志
                                sprintf(dis0,"人脸特征Id:%02d  ",getFaceId());
                                MyLCD_Show(3,yPlace++,dis0);//液晶显示字符串                                     
                                /****舵机动作等等*****/
                                
                                for(i=0;i<15;i++)delay_ms(100);
                            }
                            else
                            {
                                MyLCD_Show(3,yPlace++,"人脸特征Id:**  ");//液晶显示字符串   
                            }
                        }
                        else
                        {
                            yPlace=2;
                            FRONT_COLOR=Color16_WHITE;	 //设置显示颜色                               
                            MyLCD_Show(3,yPlace++,"K1启动识别 K2取消");//液晶显示字符串                            
                            MyLCD_Show(3,yPlace++,"                 ");//液晶显示字符串                           
                        }
                default:break;
            }
            if(remCurMenuId != My_Menu_GetSelectID())//切换菜单选择后 初始化相应变量
            {
                remCurMenuId = My_Menu_GetSelectID();  
                cancelCurFaceCmd();//取消当前命令                
                startCmdFlag = 0;//启动人脸识别  
                reMenukey=0;//防止第一次进入菜单 相当于已经点击了确定键
            }            
        }
                
        if(mySendFlag_tick == true )//定时发送时间到
        {
            mySendFlag_tick = false;//清除标志   
            if(openDjDelay >0)//舵机动作倒计时 
            {
                openDjDelay--;//倒计时
                My_SG90_SetAngle(0,OPEN_ANGLE); //控制转动方向
            }
            else {My_SG90_SetAngle(0,CLOSE_ANGLE);}//控制转动方向            
        }
        My_UartMessage_Process();//处理串口数据        
    }
}

void scanKeyAnddealKey(void)
{
    My_KeyScan();//按键扫描

    if(selectedMenu!=NULL)
    {
        if(KeyIsPress(KEY_1))//按下了向上键
        {
            My_Menu_SelectUp(true);
        }
        if(KeyIsPress(KEY_2))//按下了向上键
        {
            My_Menu_SelectDown(true);
        }
        if(KeyIsPress(KEY_3))//按下了OK键
        {
            My_Menu_Into(true);
        }
        if(KeyIsPress(KEY_4))
        {
//            workState_menu=0;//回到初始状态
            My_Menu_Back(true);
        }
    }
    else if(KeyIsPress(KEY_4))
    {
        My_Menu_Into(true);
    }
    else if(KeyIsPress(KEY_5))
    {
        outCount++;//出去人数
        openDjDelay = OPEN_DELAY; //打开舵机时间
    }
}

void My_Menu_Display(void)
{    
    static u8 remBack = 0xff;//首次推出菜单
    static u8 firstBack = 0;//首次推出菜单    
    if(selectedMenu==NULL)//如果已经退出菜单
    {
        firstBack = 0;//记录当前所处模式
        if(remBack != firstBack) //首次进入设置项
        {remBack = firstBack;LCD_Clear(Color16_BLACK);}//清全屏   
        return;//退出函数
    }
    else
    {
        firstBack = 1;//记录当前所处模式
        if(remBack != firstBack) //首次进入设置项
        {remBack = firstBack;LCD_Clear(Color16_BLACK);}//清全屏    
        
    }    
    FRONT_COLOR = Color16_YELLOW;//设置前景色      
    if(My_Menu_UnderBottom())//最低一层菜单 说明其下面没有选择项只有标题
    {
        MyLCD_Show(POS_MENU_X+1,POS_MENU_Y,(char *)*(selectedMenu->contentPtrArray+selectedMenu->selectIndex));//液晶显示字符串
        return;//退出函数
    }      
    MyLCD_Show(POS_MENU_X+1,POS_MENU_Y,(char *)selectedMenu->title);//根菜单 第一级菜单标题
    
    for(i=0;i<selectedMenu->itemCount;i++)//该级菜单选项
    {
        if(i==selectedMenu->selectIndex)
        {
            FRONT_COLOR = Color16_BLUE;//设置前景色
            MyLCD_Show(POS_MENU_X,i+POS_MENU_Y+1,">");//液晶显示字符串
            MyLCD_Show(POS_MENU_X+1,POS_MENU_Y+1+i,(char *)*(selectedMenu->contentPtrArray+i));//液晶显示字符串
            FRONT_COLOR = Color16_WHITE;//设置前景色（画笔颜色）为白色
        }
        else
        {
            FRONT_COLOR=Color16_WHITE;	 //设置显示颜色  
            MyLCD_Show(POS_MENU_X,i+POS_MENU_Y+1," ");//液晶显示字符串
            MyLCD_Show(POS_MENU_X+1,POS_MENU_Y+1+i,(char *)*(selectedMenu->contentPtrArray+i));//液晶显示字符串  
        }
    }
}

void My_Menu_Clear(void)
{
    LCD_Clear(Color16_BLACK);//清全屏 返回菜单 直接清屏
}