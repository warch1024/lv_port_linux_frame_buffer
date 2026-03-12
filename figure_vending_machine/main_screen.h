#ifndef _MAIN_SCREEN_H
#define _MAIN_SCREEN_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"
#include"figure_vending_machine/checkout.h" 
#include "open_chat_client.h"  //open chat client头文件
// #include "figure_vending_machine/goods_mangm.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

/*商品卡片创建返回值结构体
*/
typedef struct Item_card_t{
    lv_obj_t * item_window;
    lv_obj_t * item_title_lb;
    lv_obj_t * item_sb;
    lv_obj_t * iten_price_lb;
}item_card_t;


void main_screen_init();
const char* get_goods_card_title(lv_obj_t* item_spinbox);
void main_screen();
/* 创建商品卡片 
@param parent_o 父对象
@param item_title 商品标题
@param price 商品价格
@param bg_pic_path 商品背景图片路径
@return 商品卡片对象
*/
item_card_t create_main_screen_item_card(lv_obj_t * parent_o, char * item_title_text, int price, char * bg_pic_path);

tools_dll_t* add_page_k_item_card(lv_obj_t * parent_o, tools_dll_t * listed_goods);

void main_screen_guest_cb(lv_event_t * e);
lv_obj_t * add_page_4_card_(lv_obj_t * parent_o, tools_dll_t * listed_goods);
////////
void load_open_chat_main_interface(lv_obj_t * main_screen_o);
void add_open_chat_ol_client_to_list(lv_obj_t * open_chat_ol_list, struct clientmsg * online_clients);
#endif