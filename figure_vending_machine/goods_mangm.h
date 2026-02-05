#ifndef GOODS_MANGM_H
#define GOODS_MANGM_H

#include "tools/tools.h"

#include"figure_vending_machine/screen_objs.h"  //标准库


typedef struct Goods_info_t{    //商品信息结构
    char title[40];
    uint32_t price;
    char goods_pic_path[100];
    uint32_t num;
}goods_info_t;

extern tools_dll_t * listed_goods; //全局已上架商品列表


void init_def_goods_to_list();
goods_info_t * list_listed_goods_list(tools_dll_t *listed_goods_list, goods_info_t new_goods);
goods_info_t *delete_listed_goods_list(tools_dll_t *listed_goods_list, goods_info_t * del_goods);
goods_info_t * find_listed_goods_list(tools_dll_t *listed_goods_list, char * goods_name);
int modify_listed_goods_list(tools_dll_t *listed_goods_list, char * goods_name, uint32_t num, uint32_t price);
int settle_modify_listed_goods_list(tools_dll_t *listed_goods_list, char * goods_name, uint32_t num);
void print_goods_info(tools_dll_t * listed_goods_list);
#endif