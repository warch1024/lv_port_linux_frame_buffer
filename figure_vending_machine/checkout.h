#ifndef _CHECKOUT_H
#define _CHECKOUT_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"
#include "figure_vending_machine/goods_mangm.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

//购物车加购列表
extern tools_dll_t * shopping_cart_added_list;

//加购清单
typedef struct Wish_list_t{
    goods_info_t * goods; //加购的商品
    uint32_t num;    //加购个数

}wish_list_t;

lv_obj_t * shopping_cart(lv_obj_t * parent_o);
void shopping_cart_btn_cb(lv_event_t * e);
void refresh_shopping_cart_list(tools_dll_t * added_list, lv_obj_t * shopping_cart_list_o);
void add_wish_list_goods(tools_dll_t * added_list, goods_info_t * goods);
void minus_wish_list_goods(tools_dll_t * added_list, goods_info_t * goods);
void init_shopping_cart_added_list();

void set_shopping_cart_btn_goods_counter(lv_obj_t * shopping_cart_btn_lb, uint32_t count);
lv_obj_t * settlement(lv_obj_t * parent_o, tools_dll_t * added_list);
lv_obj_t * create_settlement_qr_card(lv_obj_t * parent_o);
uint32_t count_added_total_goods(tools_dll_t * added_list);
uint64_t caculate_added_total_price(tools_dll_t * added_list);
static void refresh_settlement_title_lb(lv_obj_t * settlement_title_lb, tools_dll_t* added_list);
#endif