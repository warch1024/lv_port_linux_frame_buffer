#ifndef _CHECKOUT_H
#define _CHECKOUT_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"
#include "figure_vending_machine/goods_mangm.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

//单个商品信息


//加购清单
typedef struct Wish_list_t{
    goods_info_t * goods; //加购的商品
    int num;    //商品数量
    struct Wish_list_t * next;
    struct Wish_list_t * prev;
    struct Wish_list_t * tail;
}wish_list_t;


void shopping_cart(lv_obj_t * parent_o);
void shopping_cart_btn_cb(lv_event_t * e);
#endif