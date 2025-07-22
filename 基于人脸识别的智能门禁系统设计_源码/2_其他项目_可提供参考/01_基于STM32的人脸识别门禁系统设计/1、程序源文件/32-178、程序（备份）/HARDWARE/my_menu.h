/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.1
//�޸�����:2020/07/08
//�����ܣ���װ�ͼ��Զ���˵��Ĳ���
//V1.0  2020/07/07  ��ɻ������ܣ�֧��3���˵������֧��15*15*15����ײ�˵���Ŀ
                    ���˵���һ���˵�Ϊmenu_root��Ҳ���滻Ϊmenu_level_0��
                    �����˵�Ϊmenu_level_1
                    �����˵�Ϊmenu_level_2
//V1.1  2020/07/08  �˵����ƺ����������Ƿ�ˢ����ʾ�Ĳ���
************************************************/
#ifndef __MY_MENU_H
#define __MY_MENU_H
#include "my_include.h"

typedef struct __packed
{
    u8 itemCount;
    const char **contentPtrArray;
}_my_menu_item_obj;

typedef struct
{
    const char *title;
    const char **contentPtrArray;
    struct _my_menu_obj *parent;
    struct _my_menu_obj *child;
    u8 itemCount;
    u8 selectIndex;
}_my_menu_obj;

#define TITLE_MENU_ROOT             " ��ӭʹ��"//���˵��ı���
#define MAX_LEVEL_MENU              1//�˵������㼶��
#define MAX_ITEM_BOTTOM             10//�ղ˵�����,�ղ˵���Ϊ��ײ�˵����Ӳ˵�������Ҫ��С����ײ�˵���������Ŀ��
                                       //�ܹ���һ��˵�Ҳ������ײ��ˣ����ܵ�������Ŀ��
                                       //���������˵� ��ײ�˵�ĳһ�����3��ѡ���� ��ֵΪ3
// �������������� 8 ��ʾ�����˸ü��˵���ײ��� û��ѡ������
//��0x0011 ��ʾ����ָ��ġ�һ��>��һ� ��0x8011��ʾ���롰һ��>��һ�������ײ�
#define MENU_ID_ADD_NC              0x0000 //δ��������
#define MENU_ID_ADD_X01              0x8001 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 
#define MENU_ID_ADD_X02              0x8002 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 
#define MENU_ID_ADD_X03              0x8003 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 
#define MENU_ID_ADD_X04              0x8004 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 
//#define MENU_ID_ADD_X05              0x8005 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 
//#define MENU_ID_ADD_X06              0x8006 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 
//#define MENU_ID_ADD_X07              0x8007 //���������� ��һ���˵� ��һ�� �� ��>1�����¶�>��ǰ 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char *MenuItemStr_root[] = {"1�����������","2ɾ����������ID","3ɾ��ȫ��������Ϣ","4������������"};
const static _my_menu_item_obj MenuItemObj_root = {ArrayCount(MenuItemStr_root),&MenuItemStr_root[0]};
#if MAX_LEVEL_MENU>1
const static char *MenuItemStr_level_1_0[] = {"2���˵�A-1","2���˵�A-2","2���˵�A-3","2���˵�A-4"};
const static char *MenuItemStr_level_1_1[] = {"2���˵�B-1","2���˵�B-2"};
const static char *MenuItemStr_level_1_2[] = {"2���˵�C-1","2���˵�C-2","2���˵�C-3"};
const static char *MenuItemStr_level_1_3[] = {"2���˵�D-1","2���˵�D-2"};
const static _my_menu_item_obj MenuItemObj_level_1[ArrayCount(MenuItemStr_root)] = {
                                                            {ArrayCount(MenuItemStr_level_1_0),&MenuItemStr_level_1_0[0]},
                                                            {ArrayCount(MenuItemStr_level_1_1),&MenuItemStr_level_1_1[0]},
                                                            {ArrayCount(MenuItemStr_level_1_2),&MenuItemStr_level_1_2[0]},
                                                            {ArrayCount(MenuItemStr_level_1_3),&MenuItemStr_level_1_3[0]}
                                                            };
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
#if MAX_LEVEL_MENU>2
const static char *MenuItemStr_level_2_0_0[] = {"Menu_0_0_0","Menu_0_0_1","Menu_0_0_2"};
const static char *MenuItemStr_level_2_0_1[] = {"Menu_0_1_0","Menu_0_1_1","Menu_0_1_2"};
const static char *MenuItemStr_level_2_0_2[] = {"Menu_0_2_0","Menu_0_2_1","Menu_0_2_2"};
//const static char *MenuItemStr_level_2_0_3[] = {"Menu_0_3_0","Menu_0_3_1","Menu_0_3_2"};
const static _my_menu_item_obj MenuItemObj_level_2_0[ArrayCount(MenuItemStr_level_1_0)] = {
                                                            {ArrayCount(MenuItemStr_level_2_0_0),&MenuItemStr_level_2_0_0[0]},
                                                            {ArrayCount(MenuItemStr_level_2_0_1),&MenuItemStr_level_2_0_1[0]},
                                                            {ArrayCount(MenuItemStr_level_2_0_2),&MenuItemStr_level_2_0_2[0]},
//                                                            {ArrayCount(MenuItemStr_level_2_0_3),&MenuItemStr_level_2_0_3[0]}
                                                            };

const static char *MenuItemStr_level_2_1_0[] = {"Menu_1_0_0","Menu_1_0_1","Menu_1_0_2"};
const static char *MenuItemStr_level_2_1_1[] = {"Menu_1_1_0","Menu_1_1_1","Menu_1_1_2"};
const static char *MenuItemStr_level_2_1_2[] = {"Menu_1_2_0","Menu_1_2_1","Menu_1_2_2"};
const static char *MenuItemStr_level_2_1_3[] = {"Menu_1_3_0","Menu_1_3_1"};
const static _my_menu_item_obj MenuItemObj_level_2_1[ArrayCount(MenuItemStr_level_1_1)] = {
                                                            {ArrayCount(MenuItemStr_level_2_1_0),&MenuItemStr_level_2_1_0[0]},
                                                            {ArrayCount(MenuItemStr_level_2_1_1),&MenuItemStr_level_2_1_1[0]},
                                                            {ArrayCount(MenuItemStr_level_2_1_2),&MenuItemStr_level_2_1_2[0]},
                                                            {ArrayCount(MenuItemStr_level_2_1_3),&MenuItemStr_level_2_1_3[0]}
                                                            };

const static char *MenuItemStr_level_2_2_0[] = {"Menu_2_0_0","Menu_2_0_1","Menu_2_0_2"};
const static char *MenuItemStr_level_2_2_1[] = {"Menu_2_1_0","Menu_2_1_1","Menu_2_1_2"};
//const static char *MenuItemStr_level_2_2_2[] = {"Menu_2_2_0","Menu_2_2_1","Menu_2_2_2"};
//const static char *MenuItemStr_level_2_2_3[] = {"Menu_2_3_0","Menu_2_3_1","Menu_2_3_2"};
const static _my_menu_item_obj MenuItemObj_level_2_2[ArrayCount(MenuItemStr_level_1_2)] = {
                                                            {ArrayCount(MenuItemStr_level_2_2_0),&MenuItemStr_level_2_2_0[0]},
                                                            {ArrayCount(MenuItemStr_level_2_2_1),&MenuItemStr_level_2_2_1[0]},
//                                                            {ArrayCount(MenuItemStr_level_2_2_2),&MenuItemStr_level_2_2_2[0]},
//                                                            {ArrayCount(MenuItemStr_level_2_2_3),&MenuItemStr_level_2_2_3[0]}
                                                            };

//const static char *MenuItemStr_level_2_3_0[] = {"menu_3_0_0","Menu_3_0_1","Menu_3_0_2"};
//const static char *MenuItemStr_level_2_3_1[] = {"Menu_3_1_0","Menu_3_1_1","Menu_3_1_2"};
//const static char *MenuItemStr_level_2_3_2[] = {"Menu_3_2_0","Menu_3_2_1","Menu_3_2_2"};
//const static char *MenuItemStr_level_2_3_3[] = {"Menu_3_3_0","Menu_3_3_1","Menu_3_3_2"};
//const static _my_menu_item_obj MenuItemObj_level_2_3[ArrayCount(MenuItemStr_level_1_0)] = {
//                                                            {ArrayCount(MenuItemStr_level_2_3_0),&MenuItemStr_level_2_3_0[0]},
//                                                            {ArrayCount(MenuItemStr_level_2_3_1),&MenuItemStr_level_2_3_1[0]},
//                                                            {ArrayCount(MenuItemStr_level_2_3_2),&MenuItemStr_level_2_3_2[0]},
//                                                            {ArrayCount(MenuItemStr_level_2_3_3),&MenuItemStr_level_2_3_3[0]}
//                                                            };
const static _my_menu_item_obj *MenuItemObjPtr_level_2[] = {
                                                            MenuItemObj_level_2_0,
                                                            MenuItemObj_level_2_1,
                                                            MenuItemObj_level_2_2,
//                                                            MenuItemObj_level_2_3
                                                            };

static _my_menu_obj menu_level_2_0[ArrayCount(MenuItemObj_level_2_0)];//��1�������˵���Ŀ�µ������˵�
static _my_menu_obj menu_level_2_1[ArrayCount(MenuItemObj_level_2_1)];//��2�������˵���Ŀ�µ������˵�
static _my_menu_obj menu_level_2_2[ArrayCount(MenuItemObj_level_2_2)];//��3�������˵���Ŀ�µ������˵�
//static _my_menu_obj menu_level_2_3[ArrayCount(MenuItemObj_level_2_3)];//��4�������˵���Ŀ�µ������˵�
static _my_menu_obj *menu_level_2[ArrayCount(MenuItemStr_root)] = {menu_level_2_0,menu_level_2_1,menu_level_2_2,NULL/*menu_level_2_3*/};
#endif

#if MAX_LEVEL_MENU>3
#error  MAX_LEVEL_MENU can't be bigger then 3 !
#endif

extern _my_menu_obj *selectedMenu;

void My_Menu_Init(void);
void My_Menu_SelectUp(bool updateDis);
void My_Menu_SelectDown(bool updateDis);
bool My_Menu_Into(bool updateDis);
bool My_Menu_Back(bool updateDis);
void My_Menu_Exit(void);

u32 My_Menu_GetSelectID(void);
bool My_Menu_UnderBottom(void);
void My_Menu_Display(void);
void My_Menu_Clear(void);
void My_Menu_Contrl(void);
u16 My_Menu_GetVersion(void);

#endif
