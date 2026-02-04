#ifndef GOODS_MANGM_H
#define GOODS_MANGM_H

#include "tools/tools.h"

#include"figure_vending_machine/screen_objs.h"  //标准库


typedef struct Goods_info_t{    //商品信息结构
    char title[40];
    uint64_t price;
    char goods_pic_path[100];
    uint32_t num;
    struct Goods_info_t * next;
    struct Goods_info_t * prev;
    struct Goods_info_t * tail;
}goods_info_t;

extern goods_info_t * listed_goods; //全局已上架商品列表


void init_def_goods_to_list();
goods_info_t *init_listed_goods_list();
goods_info_t * add_listed_goods_list(goods_info_t *listed_goods_list, goods_info_t goods);
goods_info_t *delete_listed_goods_list(goods_info_t *listed_goods_list, char * goods);
goods_info_t * find_listed_goods_list(goods_info_t *listed_goods_list, char * goods_name);
int modify_listed_goods_list(goods_info_t *listed_goods_list, char * goods_name, uint32_t num, uint32_t price);
int settle_modify_listed_goods_list(goods_info_t *listed_goods_list, char * goods_name, uint32_t num);
void modify_head_listed_goods_list(goods_info_t *listed_goods_list);


#endif