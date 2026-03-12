#include"figure_vending_machine/checkout.h"

//管理页面对象
static lv_obj_t * shopping_cart_list = NULL,
    * shopping_cart_window = NULL,
    * shopping_cart_btn_lb = NULL,
    * shopping_cart_btn = NULL,
    * settlement_title_lb = NULL,
    * settlement_qr_code = NULL;
//购物车已加购列表
tools_dll_t * shopping_cart_added_list = NULL;

uint32_t shopping_cart_num = 0;  //显示购物车商品数量

//购物车主函数，返回标签对象
lv_obj_t * shopping_cart(lv_obj_t * parent_o){
    //添加购物车按钮
    shopping_cart_btn = lv_btn_create(parent_o);
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(shopping_cart_btn, 150, 50);
    lv_obj_align_to(shopping_cart_btn, parent_o, LV_ALIGN_BOTTOM_LEFT, 20, -10);
    //添加按钮点击回调函数，点击按钮弹出窗口
    lv_obj_add_event_cb(shopping_cart_btn, shopping_cart_btn_cb, LV_EVENT_CLICKED, NULL);

    //给按钮添加标签
    shopping_cart_btn_lb = lv_label_create(shopping_cart_btn);
    //显示购物车商品数量
    
    char shopping_cart_btn_lb_text[50] = {"\0"};
    sprintf(shopping_cart_btn_lb_text, "购物车(%d)", shopping_cart_num);
    //按钮标签
    lv_label_set_text(shopping_cart_btn_lb, shopping_cart_btn_lb_text); //设置标签文字
    lv_obj_align(shopping_cart_btn_lb,LV_ALIGN_CENTER, 0,0);
    //lv_obj_align_to(shopping_cart_btn_lb, shopping_cart_btn, LV_ALIGN_CENTER, -2, -4);//对齐标签
    lv_obj_add_style(shopping_cart_btn_lb, def_text_style, 0);   //标签支持中文
    
    //购物车小窗
    shopping_cart_window = lv_obj_create(parent_o);
    lv_obj_align(shopping_cart_window, LV_ALIGN_TOP_MID, 0, 0); //对齐小窗

    lv_obj_add_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN);  //隐藏小窗
    lv_obj_set_size(shopping_cart_window, 400, 450);    //设置大小
    lv_obj_set_style_bg_color(shopping_cart_window, lv_color_make(255, 201, 14), 0);

    //购物车标签
    lv_obj_t * shopping_cart_title = lv_label_create(shopping_cart_window); 
    lv_obj_align_to(shopping_cart_title,shopping_cart_window, LV_ALIGN_TOP_MID, -25, -12);  //对齐标题
    lv_label_set_text(shopping_cart_title, "已加购项目"); //设置标签文字
    lv_obj_add_style(shopping_cart_title, def_text_style, 0);   //标签支持中文

    //创建列表在屏幕
    shopping_cart_list = lv_list_create(parent_o);
    lv_obj_add_flag(shopping_cart_list, LV_OBJ_FLAG_HIDDEN);  //隐藏小窗
    
    lv_obj_set_size(shopping_cart_list, 390, 405);
    //列表对齐
    lv_obj_align(shopping_cart_list, LV_ALIGN_TOP_MID, 0, 40);
    
    lv_obj_add_style(shopping_cart_list, def_text_style, 0);   //列表支持中文

return shopping_cart_btn_lb;

}

//点击购物车按钮回调函数
void shopping_cart_btn_cb(lv_event_t * e){
    if(lv_obj_has_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN)){

        lv_obj_clear_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(shopping_cart_list, LV_OBJ_FLAG_HIDDEN);
        //点击购物车时刷新购物车列表
        refresh_shopping_cart_list(shopping_cart_added_list, shopping_cart_list);
    }
    else{
        lv_obj_add_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(shopping_cart_list, LV_OBJ_FLAG_HIDDEN);
    }
} 

//点击购物车按钮时，遍历加购链表更新购物车列表
void refresh_shopping_cart_list(tools_dll_t * added_list, lv_obj_t * shopping_cart_list_o){  //刷新购物车加购列表
    if(added_list && shopping_cart_list_o) {                                       // 遍历链表添加到加购列表
        lv_obj_clean(shopping_cart_list_o); //先清空列表
        //添加总价显示按钮
        // uint64_t total_price = 0;   //购物车商品总价
        // for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
        //     uint32_t num = ((wish_list_t*)(tmp_node->data))->num, 
        //         price = ((wish_list_t*)(tmp_node->data))->goods->price;
        //         total_price += num*price;
        // }
        char total_price_list_btn[50] = {0};
        sprintf(total_price_list_btn, "——合计: %d元——",caculate_added_total_price(added_list));
        lv_list_add_btn(shopping_cart_list_o, NULL, total_price_list_btn);
  
        //显示价签
        for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            // 添加列表按钮项目
            char goods_title_item[50] = {0};
            //商品标题（数量×价格）
            uint32_t num = ((wish_list_t*)(tmp_node->data))->num, 
                price = ((wish_list_t*)(tmp_node->data))->goods->price;
            sprintf(goods_title_item, "%s    %d元(%d×%d元)", 
                ((wish_list_t*)(tmp_node->data))->goods->title, 
                (num*price),
                ((wish_list_t*)(tmp_node->data))->num,
                ((wish_list_t*)(tmp_node->data))->goods->price);
            lv_obj_t * tmp_btn = lv_list_add_btn(shopping_cart_list_o, NULL, goods_title_item);
            //lv_obj_add_style(tmp_btn, def_text_style,0);    //添加中文支持  
        }

    }
}

//增加商品到加购列表，商品页面加减按钮触发增加商品到已加购列表
void add_wish_list_goods(tools_dll_t * added_list, goods_info_t * goods){//每次添加一个商品到加购
       
    if(added_list && goods){
        //调整已加购数量计数器，并更新按钮表标签显示
        shopping_cart_num ++;   //已购商品计数器+1
        set_shopping_cart_btn_goods_counter(shopping_cart_btn_lb, shopping_cart_num);//刷新加购按钮计数器

        //查找是否已经加购
        for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(((wish_list_t*)(tmp_node->data))->goods == goods){   //已经加入购物车
                ((wish_list_t*)(tmp_node->data))->num ++;
                
                return ;
            }
        }
        //购物车没有，则加入
        wish_list_t * new_shopping_cart_goods = (wish_list_t *)malloc(sizeof(wish_list_t));
        if(new_shopping_cart_goods){
            new_shopping_cart_goods->goods = goods;
            new_shopping_cart_goods->num = 1;
            //将新的加购链表对象加入已加购链表
            tools_add_dll_list_node(added_list, new_shopping_cart_goods);
        }
    }
}
void minus_wish_list_goods(tools_dll_t * added_list, goods_info_t * goods){
     if(added_list && goods){
        //查找是否已经加购
        for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(((wish_list_t*)(tmp_node->data))->goods == goods){   //已经加入购物车
                if(((wish_list_t*)(tmp_node->data))->num <= 0){ //已经减到0，删除此节点
                    tools_delete_dll_list_node(added_list, tmp_node);
                }
                else{
                    ((wish_list_t*)(tmp_node->data))->num --;   //更新加购列表结构对象的计数
                    if(((wish_list_t*)(tmp_node->data))->num == 0){//商品减到0，删除节点
                        tools_delete_dll_list_node(added_list, tmp_node);
                    }
                    //已购商品计数器-1,只有在此商品数大于1时才减
                    shopping_cart_num --;   
                    set_shopping_cart_btn_goods_counter(shopping_cart_btn_lb, shopping_cart_num);
                }
            }
        }
    }
}

//调整加购链表成员对象，未启用
void adjust_wish_list_goods(tools_dll_t * added_list, goods_info_t * goods, uint32_t count){
       
    if(added_list && goods){
        //调整已加购数量计数器，并更新按钮表标签显示
        shopping_cart_num ++;   //已购商品计数器+1
        set_shopping_cart_btn_goods_counter(shopping_cart_btn_lb, shopping_cart_num);//刷新加购按钮计数器

        //查找是否已经加购
        for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(((wish_list_t*)(tmp_node->data))->goods == goods){   //已经加入购物车
                ((wish_list_t*)(tmp_node->data))->num ++;
                
                return ;
            }
        }
        //购物车没有，则加入
        wish_list_t * new_shopping_cart_goods = (wish_list_t *)malloc(sizeof(wish_list_t));
        if(new_shopping_cart_goods){
            new_shopping_cart_goods->goods = goods;
            new_shopping_cart_goods->num = 1;
            //将新的加购链表对象加入已加购链表
            tools_add_dll_list_node(added_list, new_shopping_cart_goods);
        }
    }
}


void init_shopping_cart_added_list(){
    if(shopping_cart_added_list == NULL){
    
        shopping_cart_added_list = tools_init_dll_list();   //初始化头节点

    }
}


//调节购物车按钮加购计数器
void set_shopping_cart_btn_goods_counter(lv_obj_t * shopping_cart_btn_lb, uint32_t count){
    char shopping_cart_btn_lb_text[50] = {"\0"};
    sprintf(shopping_cart_btn_lb_text, "购物车(%d)", count);
    //按钮标签
    lv_label_set_text(shopping_cart_btn_lb, shopping_cart_btn_lb_text); //设置标签文字
    lv_obj_invalidate(shopping_cart_btn_lb);//刷新按钮标签
    lv_obj_invalidate(lv_obj_get_parent(shopping_cart_btn_lb));//刷新按钮
}

//统计总价
uint64_t caculate_added_total_price(tools_dll_t * added_list){    //计算总价
    uint64_t total_price = 0;
    for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            uint32_t num = ((wish_list_t*)(tmp_node->data))->num, 
                price = ((wish_list_t*)(tmp_node->data))->goods->price;
            total_price += num*price;
        }
        return total_price;
}

//统计加购数量
uint32_t count_added_total_goods(tools_dll_t * added_list){
    uint32_t total_count = 0;
    for(tools_dll_t * tmp_node = added_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            uint32_t num = ((wish_list_t*)(tmp_node->data))->num;
            total_count += num;
        }
        return total_count;
}

/////////////////////////////结算功能/////////////
static void refresh_settlement_title_lb(lv_obj_t * settlement_title_lb, tools_dll_t* added_list){
    char total_price_settlement_lb_text[50] = {0};
    sprintf(total_price_settlement_lb_text, "合计: %d元\n请扫码支付",caculate_added_total_price(added_list));
    lv_label_set_text(settlement_title_lb, total_price_settlement_lb_text);
    //更新二维码内容
    char settlement_qr_code_data[200] = {0};
    int total_price = caculate_added_total_price(added_list);
    sprintf(settlement_qr_code_data, 
        "=== 欢迎使用 ===\n请添加商品到购物车\n然后点击结算按钮\n共加购商品%d件\n需要支付: %d元",
        count_added_total_goods(added_list),
        total_price);
    lv_qrcode_update(settlement_qr_code, settlement_qr_code_data, strlen(settlement_qr_code_data));
}


static void settlement_btn_btn_cb(lv_event_t * e){
    if(lv_obj_has_flag(e->user_data, LV_OBJ_FLAG_HIDDEN)){

        lv_obj_clear_flag(e->user_data, LV_OBJ_FLAG_HIDDEN);    //显示二维码
        lv_obj_clear_flag(shopping_cart_btn, LV_OBJ_FLAG_CLICKABLE);      //禁用购物车
        refresh_settlement_title_lb(settlement_title_lb, shopping_cart_added_list);
    }
    else{
        lv_obj_add_flag(e->user_data, LV_OBJ_FLAG_HIDDEN);  //隐藏二维码
        lv_obj_add_flag(shopping_cart_btn, LV_OBJ_FLAG_CLICKABLE);  //打开购物车
    }

}


//结算服务函数
lv_obj_t * settlement(lv_obj_t * parent_o, tools_dll_t * added_list){
    //购物车小窗
    lv_obj_t * settlement_window = lv_obj_create(parent_o);
    lv_obj_align(settlement_window, LV_ALIGN_TOP_MID, 0, 25); //对齐小窗

    lv_obj_add_flag(settlement_window, LV_OBJ_FLAG_HIDDEN);  //隐藏小窗
    lv_obj_set_size(settlement_window, 300, 350);    //设置大小
    lv_obj_set_style_bg_color(settlement_window, lv_color_make(255, 201, 14), 0);

    //添加二维码
    create_settlement_qr_card(settlement_window);

     //添加结算按钮
     lv_obj_t * settlement_btn = lv_btn_create(parent_o);
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(settlement_btn, 150, 50);
    lv_obj_align_to(settlement_btn, parent_o, LV_ALIGN_BOTTOM_RIGHT, -20, -10);
    //添加按钮点击回调函数，点击按钮弹出窗口
    lv_obj_add_event_cb(settlement_btn, settlement_btn_btn_cb, LV_EVENT_CLICKED, settlement_window);

    //给按钮添加标签
    lv_obj_t * settlement_btn_lb = lv_label_create(settlement_btn);
    //显示购物车商品数量
    
    //按钮标签
    lv_label_set_text(settlement_btn_lb, "结算"); //设置标签文字
    lv_obj_align(settlement_btn_lb,LV_ALIGN_CENTER, 0,0);
    //lv_obj_align_to(shopping_cart_btn_lb, shopping_cart_btn, LV_ALIGN_CENTER, -2, -4);//对齐标签
    lv_obj_add_style(settlement_btn_lb, def_text_style, 0);   //标签支持中文
    

    //购物车标签
    settlement_title_lb = lv_label_create(settlement_window); 
    lv_obj_align_to(settlement_title_lb,settlement_window, LV_ALIGN_TOP_MID, -25, -12);  //对齐标题

    lv_label_set_text(settlement_title_lb, "合计: 0元\n\n扫码支付"); //设置标签文字
    lv_obj_add_style(settlement_title_lb, def_text_style, 0);   //标签支持中文
    return  settlement_title_lb;
}

lv_obj_t * create_settlement_qr_card(lv_obj_t * parent_o){
    // 创建一个150x150像素的二维码
    settlement_qr_code = lv_qrcode_create(parent_o, 250, lv_color_hex(0x000000), lv_color_hex(0xFFFFFF)); // 背景色(白色)
    lv_obj_align(settlement_qr_code, LV_ALIGN_BOTTOM_MID, 0, 0);
    const char * initial_data = "=== 欢迎使用 ===\n请添加商品到购物车\n然后点击结算按钮";
    lv_qrcode_update(settlement_qr_code, initial_data, strlen(initial_data));
    printf("QR code created with initial content: \n%s\n", initial_data);
    return settlement_qr_code;
}