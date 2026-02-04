
#include"figure_vending_machine/main_screen.h"

#define ONE_PAGE_ITERMS_NUM 4

static item_card_ros one_screen_item_card[ONE_PAGE_ITERMS_NUM];

static void item_sb_increment_event_cb(lv_event_t * e)  //直接传入要调节的滚轮的对象地址
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment((lv_obj_t *)e->user_data);
    }
}

static void item_sb_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement((lv_obj_t *)e->user_data);
    }
}


void main_screen()
{
    //初始化全局listed_list
    init_def_goods_to_list();
    //创建主窗口
    main_screen_o=lv_obj_create(NULL);
    //设置大小
    lv_obj_set_size(main_screen_o,800,480);
    //创建商品展示页面1，商品显示在此窗口上
    lv_obj_t * main_screen_page1_o=lv_obj_create(main_screen_o);
    //设置商品展示页面1大小
    lv_obj_set_size(main_screen_page1_o,800,480);

    //加载主界面删除旧窗口
    lv_scr_load_anim(main_screen_o, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, true);

     //创建标签
    lv_obj_t *main_screen_title=lv_label_create(main_screen_o);
    lv_obj_set_align(main_screen_title,LV_ALIGN_TOP_MID);
    //标签设置文字
    lv_label_set_text(main_screen_title,"2233扭蛋自主贩卖机");
    lv_obj_add_style(main_screen_title,def_text_style,0);

    //
    //int num = 4;
    char items_title[][40] = {"2", "22", "223", "2233"};
    int items_prices[ONE_PAGE_ITERMS_NUM] = {10,21,33,22};
    //图片的地址必须使用内存申请
    char * items_bg_pic_path = (char*)(malloc(ONE_PAGE_ITERMS_NUM * 100));
    strcpy(items_bg_pic_path, "S:/IOT/projects/26-1-30/resources/item1.jpg\0");
    strcpy(items_bg_pic_path + 1*100, "S:/IOT/projects/26-1-30/resources/item2.jpg\0");
    strcpy(items_bg_pic_path + 2*100, "S:/IOT/projects/26-1-30/resources/item3.jpg\0");
    strcpy(items_bg_pic_path + 3*100, "S:/IOT/projects/26-1-30/resources/item1.jpg\0");

    //将商品添加到页面1上
    create_main_screen_k_item_card(main_screen_page1_o, ONE_PAGE_ITERMS_NUM, items_title,items_prices, items_bg_pic_path);
    
    //添加购物车相关
    shopping_cart(main_screen_o);
}


item_card_ros create_main_screen_item_card(lv_obj_t * parent_o, char * item_title_text, int price, char * bg_pic_path){
    
    //goods title
    lv_obj_t * item_window = lv_obj_create(parent_o);
        
    //set window size
    lv_obj_set_size(item_window, 200, 300);   //固定尺寸

    //直接设置每个对象的样式
    lv_obj_set_style_bg_img_src(item_window, bg_pic_path, 0);   //设置背景图片

    // 设置商品标签
    lv_obj_t * item_title_lb = lv_label_create(item_window);
    lv_obj_align_to(item_title_lb, item_window, LV_ALIGN_TOP_MID, 0, 0);   //对齐
    // 给标签设置文字内容
    lv_label_set_text(item_title_lb, item_title_text);   //给标签添加文字
    lv_style_set_text_color(def_text_style, lv_color_make(0x00, 0x00, 0x00));  // 全局共享样式
    lv_obj_add_style(item_title_lb, def_text_style, 0);   //标签支持中文
    
    //设置滚轮
    lv_obj_t * item_spinbox = lv_spinbox_create(item_window);
    lv_spinbox_set_range(item_spinbox, 0, 99999);           // 设置范围
    lv_spinbox_set_digit_format(item_spinbox, 5, 0);    // 5位数字，0位小数
    
    lv_spinbox_set_cursor_pos(item_spinbox, 0);         // 设置选中个位（最低位）
    
    lv_spinbox_set_value(item_spinbox, 0);              // 初始值为0
    
    lv_obj_set_width(item_spinbox, 65); //宽度
    lv_obj_align_to(item_spinbox, item_window, LV_ALIGN_BOTTOM_MID, 0, 0);  //对齐

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
    sprintf(item_price_str,"售价: %d CNY", price);
    lv_label_set_text(item_price_title_lb, item_price_str); //设置文字
    lv_obj_add_style(item_price_title_lb, def_text_style,0);    //添加样式支持


    //lv_obj_set_style_bg_img_src(item_window, bg_pic_path, 0);   //设置背景图片
    //返回值
    item_card_ros ret_val = {.item_window   = item_window,
                             .item_title_lb = item_title_lb,
                             .item_sb       = item_spinbox,
                             .iten_price_lb = item_price_title_lb};
    return ret_val;
}
//一个屏幕创建n个相同样式卡片
void create_main_screen_k_item_card(
    lv_obj_t * parent_o, int num, char (* items_title_text)[40], int price[], char * bg_pic_path){

    // item_card_ros one_screen_item_card[num];
    //创建4个卡片
    for(int i=0; i<num; i++){
        one_screen_item_card[i] =
        create_main_screen_item_card(parent_o, items_title_text[i], price[i], (bg_pic_path + i*100));
    }

    //卡片窗口对齐
    lv_obj_align_to(one_screen_item_card[0].item_window, parent_o, LV_ALIGN_TOP_LEFT, 0, 40);   //第一张卡片单独对齐
    for(int i=1; i< num; i++){

        lv_obj_align_to(one_screen_item_card[i].item_window, one_screen_item_card[i-1].item_window, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    }
    //价签对齐窗口
    for(int i=0; i< num; i++){

        lv_obj_align_to(one_screen_item_card[i].iten_price_lb, one_screen_item_card[i].item_window, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 2);
    }
}

//给一个page添加k个商品item
void add_page_k_item_card(lv_obj_t * parent_o, int num, goods_info_t * listed_goods){

    //创建k个卡片
    for(int i=0; i<num; i++){
        one_screen_item_card[i] =
        create_main_screen_item_card(parent_o, items_title_text[i], price[i], (bg_pic_path + i*100));
    }

    //卡片窗口对齐
    lv_obj_align_to(one_screen_item_card[0].item_window, parent_o, LV_ALIGN_TOP_LEFT, 0, 40);   //第一张卡片单独对齐
    for(int i=1; i< num; i++){

        lv_obj_align_to(one_screen_item_card[i].item_window, one_screen_item_card[i-1].item_window, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    }
    //价签对齐窗口
    for(int i=0; i< num; i++){

        lv_obj_align_to(one_screen_item_card[i].iten_price_lb, one_screen_item_card[i].item_window, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 2);
    }
}
