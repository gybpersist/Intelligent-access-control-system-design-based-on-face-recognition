/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/07/08
//程序功能：封装和简化自定义菜单的操作
//V1.0  2020/07/07  完成基本功能，支持3级菜单，最多支持15*15*15个最底层菜单条目
                    根菜单即一级菜单为menu_root（也可替换为menu_level_0）
                    二级菜单为menu_level_1
                    二级菜单为menu_level_2
//V1.1  2020/07/08  菜单控制函数中增加是否刷新显示的参数
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

#define TITLE_MENU_ROOT             " 欢迎使用"//根菜单的标题
#define MAX_LEVEL_MENU              1//菜单的最大层级数
#define MAX_ITEM_BOTTOM             10//空菜单个数,空菜单作为最底层菜单的子菜单，个数要不小于最底层菜单中最大的条目数
                                       //总共就一层菜单也就是最底层了，不能低于总条目数
                                       //如果有两层菜单 最底层菜单某一项最多3个选项则 该值为3
// 以下所有数据中 8 表示进入了该级菜单最底层了 没有选择项了
//如0x0011 表示处于指向的“一级>第一项” 而0x8011表示进入“一级>第一项”且是最底层
#define MENU_ID_ADD_NC              0x0000 //未进入设置
#define MENU_ID_ADD_X01              0x8001 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 
#define MENU_ID_ADD_X02              0x8002 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 
#define MENU_ID_ADD_X03              0x8003 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 
#define MENU_ID_ADD_X04              0x8004 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 
//#define MENU_ID_ADD_X05              0x8005 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 
//#define MENU_ID_ADD_X06              0x8006 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 
//#define MENU_ID_ADD_X07              0x8007 //设置条件下 第一级菜单 第一项 如 根>1设置温度>当前 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char *MenuItemStr_root[] = {"1添加人脸特征","2删除单个人脸ID","3删除全部人脸信息","4人脸特征测试"};
const static _my_menu_item_obj MenuItemObj_root = {ArrayCount(MenuItemStr_root),&MenuItemStr_root[0]};
#if MAX_LEVEL_MENU>1
const static char *MenuItemStr_level_1_0[] = {"2级菜单A-1","2级菜单A-2","2级菜单A-3","2级菜单A-4"};
const static char *MenuItemStr_level_1_1[] = {"2级菜单B-1","2级菜单B-2"};
const static char *MenuItemStr_level_1_2[] = {"2级菜单C-1","2级菜单C-2","2级菜单C-3"};
const static char *MenuItemStr_level_1_3[] = {"2级菜单D-1","2级菜单D-2"};
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

static _my_menu_obj menu_level_2_0[ArrayCount(MenuItemObj_level_2_0)];//第1个二级菜单条目下的三级菜单
static _my_menu_obj menu_level_2_1[ArrayCount(MenuItemObj_level_2_1)];//第2个二级菜单条目下的三级菜单
static _my_menu_obj menu_level_2_2[ArrayCount(MenuItemObj_level_2_2)];//第3个二级菜单条目下的三级菜单
//static _my_menu_obj menu_level_2_3[ArrayCount(MenuItemObj_level_2_3)];//第4个二级菜单条目下的三级菜单
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
