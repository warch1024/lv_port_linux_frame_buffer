#ifndef _MAIN_SCREEN_H
#define _MAIN_SCREEN_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"
#include"figure_vending_machine/checkout.h" 
// #include "figure_vending_machine/goods_mangm.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

/*商品卡片创建返回值结构体
*/
typedef struct Item_card_ros{
    lv_obj_t * item_window;
    lv_obj_t * item_title_lb;
    lv_obj_t * item_sb;
    lv_obj_t * iten_price_lb;
}item_card_ros;




void main_screen();
/* 创建商品卡片 
@param parent_o 父对象
@param item_title 商品标题
@param price 商品价格
@param bg_pic_path 商品背景图片路径
@return 商品卡片对象
*/
item_card_ros create_main_screen_item_card(lv_obj_t * parent_o, char * item_title_text, int price, char * bg_pic_path);

tools_dll_t* add_page_k_item_card(lv_obj_t * parent_o, tools_dll_t * listed_goods);
#endif