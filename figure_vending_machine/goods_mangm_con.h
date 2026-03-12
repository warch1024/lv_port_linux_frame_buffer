#include "figure_vending_machine/goods_mangm.h"

#include "tools/tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 交互式商品管理函数
void goods_management_console(void);
void print_goods_info_interactive(void);
void add_new_goods_interactive(void);
void modify_existing_goods_interactive(void);
void delete_goods_interactive(void);
void search_goods_interactive(void);
void search_goods_by_name_interactive(const char * keyword);
void search_goods_by_price_range_interactive(void);
void search_goods_by_price_range(int min_price, int max_price);
void modify_goods_all_fields_interactive(goods_info_t * goods, char * new_title,uint32_t * new_price, uint32_t * new_num, char * new_pic_path);
void show_goods_management_menu(void);