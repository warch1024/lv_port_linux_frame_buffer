
#include"figure_vending_machine/main_screen.h"

#define ONE_PAGE_ITERMS_NUM 4
#define GOODS_ITEM_CARD_LB 65533    //设置商品卡片的标签对象的用户标识
static item_card_t one_screen_item_card[ONE_PAGE_ITERMS_NUM];


const char* get_goods_card_title(lv_obj_t* item_spinbox){
    lv_obj_t* item_window = lv_obj_get_parent(item_spinbox);    //获取父对象
    if(item_window == NULL) return NULL;
    
    for(int i = 0; i < lv_obj_get_child_cnt(item_window); i++) {
        lv_obj_t* child = lv_obj_get_child(item_window, i);
        if(lv_obj_get_user_data(child) == (void*)GOODS_ITEM_CARD_LB){
            
            return lv_label_get_text(child);    //返回商品标题
        }
    }
    return NULL;
}
//增加按钮回调
static void item_sb_increment_event_cb(lv_event_t * e)  //直接传入要调节的滚轮的对象地址
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment((lv_obj_t *)e->user_data);
        goods_info_t * goods =NULL;
        //查找触发回调的按钮所在商品卡片的商品title，根据此title查找已上架列表，返回商品信息对象
        char goods_title[40] = {0};
        //获取卡片标签的文字
        strcpy(goods_title,
             lv_label_get_text(tools_get_peer_obj_via_user_label(e->user_data, (void*)GOODS_ITEM_CARD_LB)));
        
        //根据此title查找已上架列表，返回商品信息对象
        if((goods = find_listed_goods_list(listed_goods, goods_title)) != NULL){

            //将此商品加入加购列表
            add_wish_list_goods(shopping_cart_added_list, goods);
        }
    }
}

static void item_sb_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement((lv_obj_t *)e->user_data);
        goods_info_t * goods =NULL;
        //查找触发回调的按钮所在商品卡片的商品title，根据此title查找已上架列表，返回商品信息对象
        char goods_title[40] = {0};
        //获取卡片标签的文字
        strcpy(goods_title,
             lv_label_get_text(tools_get_peer_obj_via_user_label(e->user_data, (void*)GOODS_ITEM_CARD_LB)));
        
        //根据此title查找已上架列表，返回商品信息对象
        if((goods = find_listed_goods_list(listed_goods, goods_title)) != NULL){
            //将此商品加入加购列表
            minus_wish_list_goods(shopping_cart_added_list, goods);
        }
    }
}


void main_screen(){

    //初始化全局listed_list
    init_def_goods_to_list();
    //初始化已加购列表
    init_shopping_cart_added_list();

    //创建主窗口
    main_screen_o=lv_obj_create(NULL);
    //设置大小
    lv_obj_set_size(main_screen_o,800,480);
    // 设置背景颜色为白色
    lv_obj_set_style_bg_color(main_screen_o, lv_color_hex(0xd4e9f7), LV_PART_MAIN);  

    //给主屏幕添加屏幕滑动
    //lv_obj_add_event_cb(main_screen_o, main_screen_guest_cb, LV_EVENT_GESTURE, NULL);

    //加载主界面删除旧窗口
    lv_scr_load_anim(main_screen_o, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, true);

    add_page_4_card_(main_screen_o, listed_goods);

    // 标签背景
    lv_obj_t *main_screen_title_bg = lv_obj_create(main_screen_o);
    lv_obj_set_size(main_screen_title_bg, 150,25);
    lv_obj_set_align(main_screen_title_bg,LV_ALIGN_TOP_MID);
    //lv_obj_set_style_radius(main_screen_title_bg, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(main_screen_title_bg, 0, LV_PART_MAIN);//去除边框
    // lv_obj_set_style_bg_opa(main_screen_title_bg, 50,LV_PART_MAIN);
    //创建标签
   lv_obj_t *main_screen_title=lv_label_create(main_screen_o);
   lv_obj_set_align(main_screen_title,LV_ALIGN_TOP_MID);
   //标签设置文字
   lv_label_set_text(main_screen_title,"2233扭蛋贩卖机");
   lv_obj_add_style(main_screen_title,def_text_style,0);


    //添加购物车相关功能
    lv_obj_t * shopping_cart_btn_lb = shopping_cart(main_screen_o);
    //添加结算按钮相关功能
    settlement(main_screen_o, shopping_cart_added_list);
}


item_card_t create_main_screen_item_card(lv_obj_t * parent_o, char * item_title_text, int price, char * bg_pic_path){
    

    //print_goods_info(listed_goods);
    //goods title
    lv_obj_t * item_window = lv_obj_create(parent_o);
        
    //set window size
    lv_obj_set_size(item_window, 200, 300);   //固定尺寸

    //lv_obj_set_style_opa(item_window, LV_OPA_50, LV_PART_MAIN);  // 完全透明
    //直接设置每个对象的样式
    lv_obj_set_style_bg_img_src(item_window, bg_pic_path, 0);   //设置背景图片

    // 设置商品标签
    lv_obj_t * item_title_lb = lv_label_create(item_window);
    //设置标签对象的用户标识，用于查找商品卡片的title
    lv_obj_set_user_data(item_title_lb, (void*)GOODS_ITEM_CARD_LB); 
    
    lv_obj_align_to(item_title_lb, item_window, LV_ALIGN_TOP_MID, 0, 0);   //对齐
    // 给标签设置文字内容
    lv_label_set_text(item_title_lb, item_title_text);   //给标签添加文字
    lv_style_set_text_color(def_text_style, lv_color_make(0x00, 0x00, 0x00));  // 全局共享样式
    lv_obj_add_style(item_title_lb, def_text_style, 0);   //标签支持中文
    
    //设置滚轮
    lv_obj_t * item_spinbox = lv_spinbox_create(item_window);
    lv_spinbox_set_range(item_spinbox, 0, 99999);           // 设置范围
    lv_spinbox_set_digit_format(item_spinbox, 4, 0);    // 5位数字，0位小数
    lv_spinbox_set_cursor_pos(item_spinbox, 0);         // 设置选中个位（最低位）
    lv_spinbox_set_value(item_spinbox, 0);              // 初始值为0
    lv_obj_set_width(item_spinbox, 65); //宽度
    lv_obj_align_to(item_spinbox, item_window, LV_ALIGN_BOTTOM_MID, 0, 0);  //对齐
    lv_obj_add_style(item_spinbox, def_text_style, 0);  //添加中文支持
    //设置滚轮不能交互
    lv_textarea_set_cursor_click_pos(item_spinbox, false);//禁止点击聚焦
    // lv_obj_clear_flag(item_spinbox, LV_OBJ_FLAG_CLICKABLE); //不可点击

    lv_coord_t sb_h = lv_obj_get_height(item_spinbox); //获取滚轮高度

    lv_obj_t * mp_btn = lv_btn_create(item_window); //两边的按钮
    lv_obj_set_size(mp_btn, sb_h, sb_h);
    lv_obj_align_to(mp_btn, item_spinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_set_style_bg_img_src(mp_btn, LV_SYMBOL_PLUS, 0);
    //传入滚轮的地址
    lv_obj_add_event_cb(mp_btn, item_sb_increment_event_cb, LV_EVENT_ALL,  item_spinbox);

    mp_btn = lv_btn_create(item_window);    //复用按钮
    lv_obj_set_size(mp_btn, sb_h, sb_h);    //设置尺寸
    lv_obj_align_to(mp_btn, item_spinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);    //对齐到右侧
    lv_obj_set_style_bg_img_src(mp_btn, LV_SYMBOL_MINUS, 0);    //设置背景
    //添加回调函数，传入滚轮地址
    lv_obj_add_event_cb(mp_btn, item_sb_decrement_event_cb, LV_EVENT_ALL, item_spinbox);
    //添加价格标签
    lv_obj_t *item_price_title_lb=lv_label_create(parent_o);    //价格标签放在屏幕上
    

    //标签设置文字
    char item_price_str[50] = {0};   //转换价格数字到文本
    sprintf(item_price_str,"售价: %d CNY\n\n", price);
    lv_label_set_text(item_price_title_lb, item_price_str); //设置文字
    lv_obj_add_style(item_price_title_lb, def_text_style,0);    //添加样式支持


    //lv_obj_set_style_bg_img_src(item_window, bg_pic_path, 0);   //设置背景图片
    //返回值
    item_card_t ret_val = {.item_window   = item_window,
                             .item_title_lb = item_title_lb,
                             .item_sb       = item_spinbox,
                             .iten_price_lb = item_price_title_lb};
    return ret_val;
}
//一个屏幕创建n个相同样式卡片

//给一个page添加k个商品item,返回lv卡片对象链表
tools_dll_t* add_page_k_item_card(lv_obj_t * parent_o, tools_dll_t * listed_goods){
    //保存所有创建的商品小窗对象
    tools_dll_t * page_k_item_card = tools_init_dll_list();
    //debug
    // print_goods_info(listed_goods);
    //创建k个卡片
    for(tools_dll_t * tmp_node = listed_goods->next; tmp_node != NULL; tmp_node = tmp_node->next){
        item_card_t * item_ret_o = (item_card_t *)malloc(sizeof(item_card_t));
        //创建单个商品小窗
        if(item_ret_o){
            *item_ret_o = create_main_screen_item_card(parent_o, 
                ((goods_info_t* )(tmp_node->data))->title, 
                ((goods_info_t* )(tmp_node->data))->price, 
                ((goods_info_t* )(tmp_node->data))->goods_pic_path);
        }
        //添加商品小窗对象保存链表
        tools_add_dll_list_node(page_k_item_card, item_ret_o);
    }

    //卡片窗口对齐
    lv_obj_align_to(((item_card_t*)(page_k_item_card->next->data))->item_window,
         parent_o, LV_ALIGN_TOP_LEFT, 0, 30);   //第一张卡片单独对齐
    //其余循环对齐
    for(tools_dll_t * tmp_node = page_k_item_card->next->next; tmp_node != NULL; tmp_node = tmp_node->next){
        lv_obj_align_to(((item_card_t*)(tmp_node->data))->item_window,
         ((item_card_t*)(tmp_node->prev->data))->item_window, LV_ALIGN_OUT_RIGHT_MID, 5, 0);   
    }
    //价签对齐窗口
    for(tools_dll_t * tmp_node = page_k_item_card->next; tmp_node != NULL; tmp_node = tmp_node->next){
        lv_obj_align_to(((item_card_t*)(tmp_node->data))->iten_price_lb,
         ((item_card_t*)(tmp_node->data))->item_window, LV_ALIGN_OUT_BOTTOM_MID, 5, 0); 
    }
    return page_k_item_card;
}


////////////////
#define MAX_CARD_PER_ROW 4
//当前页面
static lv_obj_t * main_screen_current_page_ = NULL;   //保存当前page所在节点的地址

static tools_dll_t * all_main_screen_goods_cards_ = NULL;   //管理所有商品展示卡片对象

//一个页面创建4个card
lv_obj_t * add_page_4_card_(lv_obj_t * parent_o, tools_dll_t * listed_goods){
    //初始化所有卡片和页面对象管理链表
    all_main_screen_goods_cards_ = tools_init_dll_list();//管理所有卡片

    int listed_goods_count = listed_goods->num; //获取总已上架商品数量
    //创建新页面
    lv_obj_t * main_screen_page1 = lv_obj_create(parent_o);
    lv_obj_set_size(main_screen_page1, lv_obj_get_width(parent_o), lv_obj_get_height(parent_o));//设置尺寸
    lv_obj_align_to(main_screen_page1, parent_o, LV_ALIGN_BOTTOM_MID,0,0);  //对齐


    //遍历已上架列表
    for(tools_dll_t * tmp_node = listed_goods->next; tmp_node != NULL; tmp_node = tmp_node->next){
        //创建小窗创建函数返回值结构
        item_card_t * item_ret_o = (item_card_t *)malloc(sizeof(item_card_t));
        //创建单个商品小窗
        if(item_ret_o){
            *item_ret_o = create_main_screen_item_card(main_screen_page1, 
                ((goods_info_t* )(tmp_node->data))->title, 
                ((goods_info_t* )(tmp_node->data))->price, 
                ((goods_info_t* )(tmp_node->data))->goods_pic_path);
        }
        //添加商品小窗对象保存链表尾
        tools_add_dll_list_node(all_main_screen_goods_cards_, item_ret_o);

        //卡片窗口对齐，第一张对齐父窗口new_page
        if(all_main_screen_goods_cards_->num == 1){ //全局第一张
            lv_obj_align_to(((item_card_t*)(all_main_screen_goods_cards_->tail->data))->item_window,
                main_screen_page1, LV_ALIGN_TOP_LEFT, 0, 20);   //第一张卡片单独对齐
        }
        else if(all_main_screen_goods_cards_->num % MAX_CARD_PER_ROW == 1){    //第二行第一张对齐到上一张
            lv_obj_align_to(((item_card_t*)(all_main_screen_goods_cards_->tail->data))->item_window,
                ((item_card_t*)(all_main_screen_goods_cards_->tail->prev->prev->prev->prev->data))->item_window,
                LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
        }
        else{   //其余循环对齐前一张
            lv_obj_align_to(((item_card_t*)(all_main_screen_goods_cards_->tail->data))->item_window,
                ((item_card_t*)(all_main_screen_goods_cards_->tail->prev->data))->item_window,
                LV_ALIGN_OUT_RIGHT_MID, 5, 0);
        }
        //所有的价签对齐卡片
        lv_obj_align_to(((item_card_t*)(all_main_screen_goods_cards_->tail->data))->iten_price_lb,
            ((item_card_t*)(all_main_screen_goods_cards_->tail->data))->item_window,
            LV_ALIGN_OUT_BOTTOM_MID, 5, 0);
        //完成之后将page对象保存到链表all_main_screen_pages里面
    }
}
///////////////
