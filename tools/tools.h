#ifndef _TOOLS_H
#define _TOOLS_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/main_screen.h"
// #include "figure_vending_machine/user_info_mangm.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

//extern lv_obj_t * tools_cn_kb, *tools_cand_pannel;

typedef struct CN_kb_cp_ros{
    lv_obj_t * cn_kb;
    lv_obj_t * cn_kb_cp;
}cn_kb_cp_ros;

//通用双向带尾节点指针链表结构
typedef struct Tools_dll_t{
    void * data;
    uint32_t num;
    struct Tools_dll_t * next;
    struct Tools_dll_t * prev;
    struct Tools_dll_t * tail;
}tools_dll_t;

/*创建字体样式函数
参数：
    style：样式指针
    font_path:字体存储路径，指定使用的字体的存储路径
    font_size:字体大小
*/
void tools_create_font_style(lv_style_t *style, char* font_path, int font_size);

/*内部-输入法添加拼音插件
参数：
    kb：要添加插件的键盘
    parent_obj:插件放置对象
    kb_width: 键盘宽度，候选栏绑定此宽度
*/
//static lv_obj_t * add_pinyin_plugin(lv_obj_t * kb, lv_obj_t * parent_obj, int kb_width);

/*创建拼音输入法
参数：
    parent_obj：键盘父对象
    weight:键盘宽度
    height: 键盘高度
*/
cn_kb_cp_ros tools_create_pinyin_ime(lv_obj_t * parent_obj, int weight, int height);

/*显示拼音键盘
参数：
    kb：要显示的键盘
    cp: 要显示的候选栏
*/
void tools_show_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp);

/*隐藏拼音键盘
参数：
    kb：要隐藏的键盘
    cp: 要隐藏的候选栏
*/
void tools_hidden_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp);

/*创建登录窗口的输入框，并返回输入框对象，添加中文支持，限定一行，限定8长度
* @param parent_o 父对象
* @param width 宽度
* @param height 高度
* @param pht 提示
*/
lv_obj_t* tools_create_login_ta(lv_obj_t* parent_o, int width, int height, char* pht);

/*创建登录窗口的按钮标签，并返回按钮对象，设置内部对齐，支持中文
* @param parent_o 按钮对象
* @param width 宽度
* @param height 高度
* @param text 按钮文字
*/
lv_obj_t* tools_create_login_bt_lb(lv_obj_t* parent_o,char* text);
/**
 * @brief 创建按钮标签，并返回按钮对象，设置内部对齐，支持中文
 * @param parent_o 按钮对象
 * @param width 宽度
 * @param height 高度
 * @param text 按钮文字
 * @return lv_obj_t* 按钮对象
 */
lv_obj_t* tools_add_bt_lb(lv_obj_t* parent_o,char* text);
/**
 * @brief 创建按钮带标签，并返回按钮对象，设置标签内部对齐，支持中文
 * @param parent_o 按钮对象
 * @param width 宽度
 * @param height 高度
 * @param text 按钮文字
 * @param color 按钮颜色
 * @return lv_obj_t* 按钮对象
 */
lv_obj_t* tools_create_lbed_bt(lv_obj_t* parent_o,char* text, int width, int height, lv_color_t color);
/*创建登录窗口的复选框，并返回复选框对象，设置内部对齐，支持中文
* @param parent_o 父对象
* @param width 宽度
* @param height 高度
* @param text 按钮提示文字
*/
lv_obj_t * tools_create_login_checkbox(lv_obj_t * parent_o, int width, int height, char * text);

/*将键盘和输入框连接
* @param ta 输入框对象
* @param kb 键盘对象
*/
void tools_ta_kb_associate(lv_obj_t * ta, lv_obj_t * kb);

/*设置透明度样式
* @param opa_style 透明度样式
* @param obj 透明度对象
* @param opa_value 透明度值
*/
void tools_set_opa_style(lv_style_t * opa_style, lv_obj_t * obj, lv_opa_t opa_value);

/*设置背景样式
* @param bg_style 背景样式
* @param obj 背景对象
* @param opa_value 透明度值
* @param fig_path 背景图片路径
*/
void tools_set_bg_style(lv_style_t * bg_style, lv_obj_t * obj, lv_opa_t opa_value, char * fig_path);

/*链表初始化
* @brief 初始化链表头节点
* @return 返回头节点指针
*/
tools_dll_t *tools_init_dll_list();

/*链表添加节点
* @brief 添加节点
* @param list 链表头节点指针
* @param data 节点数据
* @return 返回链表
*/
tools_dll_t * tools_add_dll_list_node(tools_dll_t * dll_list, void* data);

/*链表删除节点
* @brief 删除节点
* @param list 链表头节点指针
* @param data 数据域
* @return 链表头节点指针
*/
tools_dll_t *tools_delete_dll_list_node(tools_dll_t * dll_list, tools_dll_t * node);

/*链表查找节点
* @brief 查找节点
* @param list 链表头节点指针
* @param data 数据域
* @return 数据所在节点指针
*/
tools_dll_t * tools_find_dll_list_node(tools_dll_t *dll_list, void * data);

/*链表修改
* @brief 遍历链表
* @param list 链表头节点指针
* @param old_data 旧数据域
* @param new_data 新数据域
*/
int tools_modify_dll_list_node(tools_dll_t * dll_list, void * old_data, void * new_data);

/*根据同级对象和用户标签查找并返回同级对象
* @parram peer_obj 同级对象
* @param user_label 用户标签
*/
lv_obj_t* tools_get_peer_obj_via_user_label(lv_obj_t* peer_obj, void * user_label);

#endif