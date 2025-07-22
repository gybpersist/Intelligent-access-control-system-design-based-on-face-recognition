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
#include "my_menu.h"

#define VERSION         11

static u16 selectMenuID = 0;//ѡ�еĲ˵���Ŀ��ID��ʹ��My_Menu_GetSelectID������ȡʱ���λ��bit31��ʼ��Ϊ1���θ�λ��bit30�������ǲ����ڲ˵�����ײ㡣(16λ�����֧��3���˵�)
static _my_menu_obj menu_root;//���˵�
#if MAX_LEVEL_MENU>1
static _my_menu_obj menu_level_1[ArrayCount(MenuItemStr_root)];//�����˵������ݸ��˵���Ŀ�������壩
#endif
const static _my_menu_obj menu_null[MAX_ITEM_BOTTOM];//�ղ˵�,��Ϊ��ײ�˵����Ӳ˵�������Ԫ�ظ���Ҫ��С�������˵���������Ŀ����
static _my_menu_obj *menu_level_bottom[MAX_ITEM_BOTTOM];//��Ϊ��ײ�˵����Ӳ˵���ָ�루����Ԫ�ظ���Ҫ��С�������˵���������Ŀ����
_my_menu_obj *selectedMenu = NULL;//��ǰѡ�еģ�������ʾ�ģ��˵�


void My_Menu_Init(void)
{
    u8 i,j;
    for(i=0;i<MAX_ITEM_BOTTOM;i++)
    {
        menu_level_bottom[i] = (_my_menu_obj *)&menu_null[0];
    }
    menu_root.itemCount = MenuItemObj_root.itemCount;//���˵���Ŀ����
    menu_root.title = TITLE_MENU_ROOT;//���˵�����
    menu_root.contentPtrArray = MenuItemObj_root.contentPtrArray;//���˵���һ����Ŀ�����ݣ�ָ��ָ���ָ�룩
    menu_root.parent = NULL;//���˵�û�и��˵�
    #if MAX_LEVEL_MENU>1
    menu_root.child = (struct _my_menu_obj *)&menu_level_1[0];//���˵�Ĭ�ϵ��Ӳ˵�(menu_level_1�� �����Ӳ˵� ������)
    #else
    menu_root.child = (struct _my_menu_obj *)menu_level_bottom[0];//���˵�Ĭ�ϵ��Ӳ˵�(menu_level_1�� �����Ӳ˵� ������)
    #endif
    menu_root.selectIndex = 0;//���˵�Ĭ��ѡ���һ��
    
#if MAX_LEVEL_MENU>1
    for(i=0;i<ArrayCount(menu_level_1);i++)//�������˵���ÿһ���Ӳ˵�
    {
        menu_level_1[i].itemCount = MenuItemObj_level_1[i].itemCount;//�����Ӳ˵�����Ŀ��
        if(menu_level_1[i].itemCount)
        {
            menu_level_1[i].title = *(menu_root.contentPtrArray + i);//�����Ӳ˵��ı���
            menu_level_1[i].contentPtrArray = MenuItemObj_level_1[i].contentPtrArray;//�����Ӳ˵���һ����Ŀ�����ݣ�ָ��ָ���ָ�룩
            menu_level_1[i].parent = (struct _my_menu_obj *)&menu_root;//�����Ӳ˵��ĸ��˵�
        #if MAX_LEVEL_MENU>2
            menu_level_1[i].child = (struct _my_menu_obj *)menu_level_2[i];//�����Ӳ˵����Ӳ˵�(menu_level_2�� �����Ӳ˵� ������)
        #else
            menu_level_1[i].child = (struct _my_menu_obj *)menu_level_bottom[0];//�����Ӳ˵����Ӳ˵�(menu_level_2�� �����Ӳ˵� ������)
        #endif
            menu_level_1[i].selectIndex = 0;//�˵�Ĭ��ѡ���һ��
        }
    }
#endif
#if MAX_LEVEL_MENU>2
    for(i=0;i<ArrayCount(MenuItemObjPtr_level_2);i++)//����ÿһ������˵�
    {
        for(j=0;j<MenuItemObj_level_1[i].itemCount;j++)//����ÿһ�������˵�
        {
            (menu_level_2[i]+j)->itemCount = (MenuItemObjPtr_level_2[i]+j)->itemCount;//���ò˵�����Ŀ��
            (menu_level_2[i]+j)->title = *(menu_level_1[i].contentPtrArray+j);//���ò˵��ı���
            (menu_level_2[i]+j)->contentPtrArray = (MenuItemObjPtr_level_2[i]+j)->contentPtrArray;//���ò˵���һ����Ŀ�����ݣ�ָ��ָ���ָ�룩
            (menu_level_2[i]+j)->parent = (struct _my_menu_obj *)&menu_level_1[i];//���ò˵��ĸ��˵�
            (menu_level_2[i]+j)->child = (struct _my_menu_obj *)menu_level_bottom[0];//�����Ӳ˵����Ӳ˵�(��ΪNULL)
            (menu_level_2[i]+j)->selectIndex = 0;//�˵�Ĭ��ѡ���һ��
        }
    }
#endif
}
/************************************************
ѡ��˵�����һ��������ǵ�һ����ѡ�����һ��
************************************************/
void My_Menu_SelectUp(bool updateDis)
{
    if(My_Menu_UnderBottom())//�˵��Ƿ�����ײ�
    {
        return;//������ѡ��λ�õĲ���
    }
    if(selectedMenu->selectIndex==0)//��ǰѡ�е��ǵ�һ���˵���Ŀ
    {
        selectedMenu->selectIndex = selectedMenu->itemCount-1;//ѡ�����һ���˵���Ŀ
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child+selectedMenu->selectIndex);//ָ���Ӧ���Ӳ˵�
    }
    else
    {
        selectedMenu->selectIndex--;//ָ����һ���˵���Ŀ
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child-1);//ָ���Ӧ���Ӳ˵�
    }
    selectMenuID &= ~0x0f;//����ʾ��ǰ�㼶�˵���ĿID����������
    selectMenuID |= selectedMenu->selectIndex + 1;//��ǲ˵���ĿID
    if(updateDis)
    {
        My_Menu_Display();//��ʾ��ǰ�˵�
    }
}
/************************************************
ѡ��˵�����һ������������һ����ѡ�е�һ��
************************************************/
void My_Menu_SelectDown(bool updateDis)
{
    if(My_Menu_UnderBottom())//�˵��Ƿ�����ײ�
    {
        return;//������ѡ��λ�õĲ���
    }
    if(selectedMenu->selectIndex==selectedMenu->itemCount-1)//���ѡ�е������һ���˵���Ŀ
    {
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child-selectedMenu->selectIndex);//ָ���Ӧ���Ӳ˵�
        selectedMenu->selectIndex = 0;//ѡ�е�һ���˵���Ŀ
    }
    else
    {
        selectedMenu->child = (struct _my_menu_obj *)((_my_menu_obj *)selectedMenu->child+1);//ָ���Ӧ���Ӳ˵�
        selectedMenu->selectIndex++;//ѡ����һ���˵���Ŀ
    }
    selectMenuID &= ~0x0f;//����ʾ��ǰ�㼶�˵���ĿID����������
    selectMenuID |= selectedMenu->selectIndex + 1;//��ǲ˵���ĿID
    if(updateDis)
    {
        My_Menu_Display();//��ʾ��ǰ�˵�
    }
}
/************************************************
������һ���˵������û����һ����NULL����ֱ�ӷ���
************************************************/
bool My_Menu_Into(bool updateDis)
{
    if(My_Menu_UnderBottom())//�˵��Ƿ�����ײ�
    {
        return false;
    }
    if(updateDis)
    {
        My_Menu_Clear();//�����ǰ�˵�����ʾ
    }
    if(selectedMenu==NULL)//�����ǰѡ�еĲ˵�Ϊ�գ���δ����˵���
    {
        selectedMenu = &menu_root;//ѡ�и��˵�
    }
    else if(selectedMenu->child == NULL || ((_my_menu_obj *)(selectedMenu->child))->itemCount==0)//���û���Ӳ˵����Ӳ˵�����Ŀ��Ϊ0
    {
        selectMenuID |= 0x8000;//���Ϊ�ڲ˵�����ײ�
        if(updateDis)
        {
            My_Menu_Display();//��ʾ��ǰ�˵�
        }
        return false;
    }
    else
    {
        selectedMenu = (_my_menu_obj *)(selectedMenu->child);//ѡ�е�ǰ�˵����Ӳ˵�
    }
    selectMenuID <<= 4;//�˵�ID����4λ
    selectMenuID |= selectedMenu->selectIndex + 1;//��ǵ�ǰ�˵�ID
    if(updateDis)
    {
        My_Menu_Display();//��ʾ��ǰ�˵�
    }
    return true;
}
/************************************************
������һ���˵������û����һ����NULL����ֱ�ӷ���
************************************************/
bool My_Menu_Back(bool updateDis)
{
    if(updateDis)
    {
        My_Menu_Clear();//�����ǰ�˵�����ʾ
    }
    if(selectedMenu==NULL)//�����ǰѡ�еĲ˵�Ϊ�գ��Ѿ��˳��˵���
    {
        return false;
    }
    if(My_Menu_UnderBottom())//�˵��Ƿ�����ײ�
    {
        selectMenuID &= ~0x8000;//����˵�����ײ�ı��
        if(updateDis)
        {
            My_Menu_Display();//��ʾ��ǰ�˵�
        }
        return true;
    }
    selectedMenu = (_my_menu_obj *)(selectedMenu->parent);//ѡ�е�ǰ�˵��ĸ��˵�
    selectMenuID >>= 4;
    if(updateDis)
    {
        My_Menu_Display();//��ʾ��ǰ�˵�
    }
    return true;
}
/************************************************
ֱ���˳��˵�
************************************************/
void My_Menu_Exit(void)
{
    selectedMenu = NULL;
    selectMenuID = 0;//����˵�ID
    My_Menu_Display();//��ʾ��ǰ�˵�
}
/************************************************
��ȡ��ǰѡ�еĲ˵���Ŀ��ID��
************************************************/
u32 My_Menu_GetSelectID(void)
{
    if(selectedMenu==NULL)//δ����˵�
    {
        return 0;
    }
    return selectMenuID;//���ز˵�ID
}
/************************************************
�жϵ�ǰ�Ƿ��ڲ˵�����ײ�
************************************************/
bool My_Menu_UnderBottom(void)
{
    return (selectMenuID&0x8000)?true:false;
}
/************************************************
�˵�����ʾ������ʾ�����б仯ʱ���ã���ʾ��ǰ�Ĳ˵�
************************************************/
void __weak My_Menu_Display(void)
{
    //������˵���ʾ��������ʵ����Ŀ�и���ʵ���������
}
/************************************************
�˵��������ʾ������ʾ�����б仯ʱ���ã������ǰ�˵�����ʾ
************************************************/
void __weak My_Menu_Clear(void)
{
    //������˵���ʾ�����������ʵ����Ŀ�и���ʵ���������
}
/************************************************
���Ʋ˵����롢���ء��˳��Ȳ���
************************************************/
void __weak My_Menu_Contrl(void)
{
    
}
u16 My_Menu_GetVersion(void)
{
    return VERSION;
}
