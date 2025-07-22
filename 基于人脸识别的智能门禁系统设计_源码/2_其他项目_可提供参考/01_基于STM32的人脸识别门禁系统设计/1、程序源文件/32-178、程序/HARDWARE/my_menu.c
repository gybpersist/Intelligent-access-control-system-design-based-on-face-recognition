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
#include "my_menu.h"

#define VERSION         11

static u16 selectMenuID = 0;//选中的菜单条目的ID，使用My_Menu_GetSelectID方法获取时最高位（bit31）始终为1，次高位（bit30）代表是不是在菜单的最底层。(16位数最多支持3级菜单)
static _my_menu_obj menu_root;//根菜单
#if MAX_LEVEL_MENU>1
static _my_menu_obj menu_level_1[ArrayCount(MenuItemStr_root)];//二级菜单（根据根菜单条目个数定义）
#endif
const static _my_menu_obj menu_null[MAX_ITEM_BOTTOM];//空菜单,作为最底层菜单的子菜单（数组元素个数要不小于三级菜单中最大的条目数）
static _my_menu_obj *menu_level_bottom[MAX_ITEM_BOTTOM];//作为最底层菜单的子菜单的指针（数组元素个数要不小于三级菜单中最大的条目数）
_my_menu_obj *selectedMenu = NULL;//当前选中的（正在显示的）菜单


void My_Menu_Init(void)
{
    u8 i,j;
    for(i=0;i<MAX_ITEM_BOTTOM;i++)
    {
        menu_level_bottom[i] = (_my_menu_obj *)&menu_null[0];
    }
    menu_root.itemCount = MenuItemObj_root.itemCount;//根菜单条目总数
    menu_root.title = TITLE_MENU_ROOT;//根菜单标题
    menu_root.contentPtrArray = MenuItemObj_root.contentPtrArray;//根菜单第一个条目的内容（指向指针的指针）
    menu_root.parent = NULL;//根菜单没有父菜单
    #if MAX_LEVEL_MENU>1
    menu_root.child = (struct _my_menu_obj *)&menu_level_1[0];//根菜单默认的子菜单(menu_level_1是 保存子菜单 的数组)
    #else
    menu_root.child = (struct _my_menu_obj *)menu_level_bottom[0];//根菜单默认的子菜单(menu_level_1是 保存子菜单 的数组)
    #endif
    menu_root.selectIndex = 0;//根菜单默认选择第一条
    
#if MAX_LEVEL_MENU>1
    for(i=0;i<ArrayCount(menu_level_1);i++)//遍历根菜单的每一个子菜单
    {
        menu_level_1[i].itemCount = MenuItemObj_level_1[i].itemCount;//设置子菜单的条目数
        if(menu_level_1[i].itemCount)
        {
            menu_level_1[i].title = *(menu_root.contentPtrArray + i);//设置子菜单的标题
            menu_level_1[i].contentPtrArray = MenuItemObj_level_1[i].contentPtrArray;//设置子菜单第一个条目的内容（指向指针的指针）
            menu_level_1[i].parent = (struct _my_menu_obj *)&menu_root;//设置子菜单的父菜单
        #if MAX_LEVEL_MENU>2
            menu_level_1[i].child = (struct _my_menu_obj *)menu_level_2[i];//设置子菜单的子菜单(menu_level_2是 保存子菜单 的数组)
        #else
            menu_level_1[i].child = (struct _my_menu_obj *)menu_level_bottom[0];//设置子菜单的子菜单(menu_level_2是 保存子菜单 的数组)
        #endif
            menu_level_1[i].selectIndex = 0;//菜单默认选择第一条
        }
    }
#endif
#if MAX_LEVEL_MENU>2
    for(i=0;i<ArrayCount(MenuItemObjPtr_level_2);i++)//遍历每一组二级菜单
    {
        for(j=0;j<MenuItemObj_level_1[i].itemCount;j++)//遍历每一个二级菜单
        {
            (menu_level_2[i]+j)->itemCount = (MenuItemObjPtr_level_2[i]+j)->itemCount;//设置菜单的条目数
            (menu_level_2[i]+j)->title = *(menu_level_1[i].contentPtrArray+j);//设置菜单的标题
            (menu_level_2[i]+j)->contentPtrArray = (MenuItemObjPtr_level_2[i]+j)->contentPtrArray;//设置菜单第一个条目的内容（指向指针的指针）
            (menu_level_2[i]+j)->parent = (struct _my_menu_obj *)&menu_level_1[i];//设置菜单的父菜单
            (menu_level_2[i]+j)->child = (struct _my_menu_obj *)menu_level_bottom[0];//设置子菜单的子菜单(均为NULL)
            (menu_level_2[i]+j)->selectIndex = 0;//菜单默认选择第一条
        }
    }
#endif
}
/************************************************
选择菜单的上一条，如果是第一条就选中最后一条
************************************************/
void My_Menu_SelectUp(bool updateDis)
{
    if(My_Menu_UnderBottom())//菜单是否在最底层
    {
        return;//不进行选项位置的操作
    }
    if(selectedMenu->selectIndex==0)//当前选中的是第一个菜单条目
    {
        selectedMenu->selectIndex = selectedMenu->itemCount-1;//选中最后一个菜单条目
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child+selectedMenu->selectIndex);//指向对应的子菜单
    }
    else
    {
        selectedMenu->selectIndex--;//指向上一个菜单条目
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child-1);//指向对应的子菜单
    }
    selectMenuID &= ~0x0f;//将表示当前层级菜单条目ID的数据清零
    selectMenuID |= selectedMenu->selectIndex + 1;//标记菜单条目ID
    if(updateDis)
    {
        My_Menu_Display();//显示当前菜单
    }
}
/************************************************
选择菜单的下一条，如果是最后一条就选中第一条
************************************************/
void My_Menu_SelectDown(bool updateDis)
{
    if(My_Menu_UnderBottom())//菜单是否在最底层
    {
        return;//不进行选项位置的操作
    }
    if(selectedMenu->selectIndex==selectedMenu->itemCount-1)//如果选中的是最后一个菜单条目
    {
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child-selectedMenu->selectIndex);//指向对应的子菜单
        selectedMenu->selectIndex = 0;//选中第一个菜单条目
    }
    else
    {
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child+1);//指向对应的子菜单
        selectedMenu->selectIndex++;//选中下一个菜单条目
    }
    selectMenuID &= ~0x0f;//将表示当前层级菜单条目ID的数据清零
    selectMenuID |= selectedMenu->selectIndex + 1;//标记菜单条目ID
    if(updateDis)
    {
        My_Menu_Display();//显示当前菜单
    }
}
/************************************************
进入下一级菜单，如果没有下一级（NULL）就直接返回
************************************************/
bool My_Menu_Into(bool updateDis)
{
    if(My_Menu_UnderBottom())//菜单是否在最底层
    {
        return false;
    }
    if(updateDis)
    {
        My_Menu_Clear();//清除当前菜单的显示
    }
    if(selectedMenu==NULL)//如果当前选中的菜单为空（还未进入菜单）
    {
        selectedMenu = &menu_root;//选中根菜单
    }
    else if(selectedMenu->child == NULL || ((_my_menu_obj *)(selectedMenu->child))->itemCount==0)//如果没有子菜单或子菜单的条目数为0
    {
        selectMenuID |= 0x8000;//标记为在菜单的最底层
        if(updateDis)
        {
            My_Menu_Display();//显示当前菜单
        }
        return false;
    }
    else
    {
        selectedMenu = (_my_menu_obj *)(selectedMenu->child);//选中当前菜单的子菜单
    }
    selectMenuID <<= 4;//菜单ID左移4位
    selectMenuID |= selectedMenu->selectIndex + 1;//标记当前菜单ID
    if(updateDis)
    {
        My_Menu_Display();//显示当前菜单
    }
    return true;
}
/************************************************
返回上一级菜单，如果没有上一级（NULL）就直接返回
************************************************/
bool My_Menu_Back(bool updateDis)
{
    if(updateDis)
    {
        My_Menu_Clear();//清除当前菜单的显示
    }
    if(selectedMenu==NULL)//如果当前选中的菜单为空（已经退出菜单）
    {
        return false;
    }
    if(My_Menu_UnderBottom())//菜单是否在最底层
    {
        selectMenuID &= ~0x8000;//清除菜单在最底层的标记
        if(updateDis)
        {
            My_Menu_Display();//显示当前菜单
        }
        return true;
    }
    selectedMenu = (_my_menu_obj *)(selectedMenu->parent);//选中当前菜单的父菜单
    selectMenuID >>= 4;
    if(updateDis)
    {
        My_Menu_Display();//显示当前菜单
    }
    return true;
}
/************************************************
直接退出菜单
************************************************/
void My_Menu_Exit(void)
{
    selectedMenu = NULL;
    selectMenuID = 0;//清除菜单ID
    My_Menu_Display();//显示当前菜单
}
/************************************************
获取当前选中的菜单条目的ID号
************************************************/
u32 My_Menu_GetSelectID(void)
{
    if(selectedMenu==NULL)//未进入菜单
    {
        return 0;
    }
    return selectMenuID;//返回菜单ID
}
/************************************************
判断当前是否在菜单的最底层
************************************************/
bool My_Menu_UnderBottom(void)
{
    return (selectMenuID&0x8000)?true:false;
}
/************************************************
菜单的显示，当显示内容有变化时调用，显示当前的菜单
************************************************/
void __weak My_Menu_Display(void)
{
    //弱定义菜单显示函数，在实际项目中根据实际情况定义
}
/************************************************
菜单的清除显示，当显示内容有变化时调用，清除当前菜单的显示
************************************************/
void __weak My_Menu_Clear(void)
{
    //弱定义菜单显示清除函数，在实际项目中根据实际情况定义
}
/************************************************
控制菜单进入、返回、退出等操作
************************************************/
void __weak My_Menu_Contrl(void)
{
    
}
u16 My_Menu_GetVersion(void)
{
    return VERSION;
}
