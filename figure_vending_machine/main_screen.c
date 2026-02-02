
#include"figure_vending_machine/main_screen.h"

static lv_style_t * main_screen_item_bg_style = NULL;

static lv_obj_t * item1_window = NULL,
    * item2_window = NULL,
    * item3_window = NULL,
    * item4_window = NULL;

static int items_price[4] = {100, 0, 0, 0};
static lv_obj_t * myspinbox;


static void lv_spinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(myspinbox);
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(myspinbox);
    }
}

void main_screen()
{
    
    //创建主窗口
    main_screen_o=lv_obj_create(NULL);
    //设置大小
    lv_obj_set_size(main_screen_o,800,480);
    //加载主界面删除旧窗口
    lv_scr_load_anim(main_screen_o, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, true);

     //创建标签
    lv_obj_t *main_screen_title=lv_label_create(main_screen_o);
    lv_obj_set_align(main_screen_title,LV_ALIGN_TOP_MID);
    //标签设置文字
    lv_obj_add_style(main_screen_title,def_text_style,0);
    lv_label_set_text(main_screen_title,"2233扭蛋自主贩卖机");

    //新建4个小窗
    item1_window = lv_obj_create(main_screen_o);
    item2_window = lv_obj_create(main_screen_o);
    item3_window = lv_obj_create(main_screen_o);
    item4_window = lv_obj_create(main_screen_o);
        
    //设置窗口大小
    lv_obj_set_size(item1_window,200,300);
    lv_obj_set_size(item2_window,200,300);
    lv_obj_set_size(item3_window,200,300);
    lv_obj_set_size(item4_window,200,300);

    //对齐窗口
    lv_obj_align_to(item1_window, main_screen_o, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_align_to(item2_window, item1_window, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align_to(item3_window, item2_window, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align_to(item4_window, item3_window, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    //直接设置每个对象的样式
    lv_obj_set_style_bg_img_src(item1_window, "S:/IOT/projects/26-1-30/resources/item1.jpg", 0);
    lv_obj_set_style_bg_img_src(item2_window, "S:/IOT/projects/26-1-30/resources/item2.jpg", 0);
    lv_obj_set_style_bg_img_src(item3_window, "S:/IOT/projects/26-1-30/resources/item3.jpg", 0);
    lv_obj_set_style_bg_img_src(item4_window, "S:/IOT/projects/26-1-30/resources/item1.jpg", 0);

    // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_t * item1_btn_lb = lv_label_create(item1_window); 
    lv_obj_align_to(item1_btn_lb, item1_window, LV_ALIGN_TOP_MID, 0, 0);   //对齐

    // 给标签设置文字内容
    lv_label_set_text(item1_btn_lb, "2233");   //给标签添加文字
    lv_style_set_text_color(def_text_style, lv_color_make(0x00, 0x00, 0x00));  // 设置为红色
    lv_obj_add_style(item1_btn_lb, def_text_style, 0);   //标签支持中文
    
    //设置滚轮
    myspinbox = lv_spinbox_create(item1_window);
    lv_spinbox_set_range(myspinbox, 0, 99999);           // 设置范围
    lv_spinbox_set_digit_format(myspinbox, 5, 0);    // 5位数字，0位小数
    
    lv_spinbox_set_cursor_pos(myspinbox, 0);         // 设置选中个位（最低位）
    
    lv_spinbox_set_value(myspinbox, 0);              // 初始值为0
    
    lv_obj_set_width(myspinbox, 65);
    lv_obj_align_to(myspinbox, item1_window, LV_ALIGN_BOTTOM_MID, 0, 5);

    lv_coord_t h = lv_obj_get_height(myspinbox);

    lv_obj_t * btn = lv_btn_create(item1_window);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, myspinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  NULL);

    btn = lv_btn_create(item1_window);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, myspinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);
    //价签
    lv_obj_t *item_price_title_lb=lv_label_create(main_screen_o);
    lv_obj_align_to(item_price_title_lb, item1_window, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 2);

    //标签设置文字
    char item_price_str[50] = {0};   //转换价格数字到文本
    sprintf(item_price_str,"售价: %d CNY", items_price[0]);
    lv_label_set_text(item_price_title_lb, item_price_str);
    lv_obj_add_style(item_price_title_lb, def_text_style,0);
    //价格


}








void create_main_screen_item(lv_obj_t * parent_o, char * item_title, int price, char * bg_pic_path){

    //goods title
    lv_obj_t * item_window = lv_obj_create(parent_o);
        
    //set window size
    lv_obj_set_size(item_window,200,300);   //固定尺寸

    // //对齐窗口
    // lv_obj_align_to(item_window, main_screen_o, LV_ALIGN_TOP_LEFT, 0, 40);

    //直接设置每个对象的样式
    lv_obj_set_style_bg_img_src(item_window, bg_pic_path, 0);

    // 设置商品标签
    lv_obj_t * item_title_lb = lv_label_create(item_window);
    lv_obj_align_to(item_title_lb, item_window, LV_ALIGN_TOP_MID, 0, 0);   //对齐
    // 给标签设置文字内容
    lv_label_set_text(item_title_lb, "二二三三");   //给标签添加文字
    lv_style_set_text_color(def_text_style, lv_color_make(0x00, 0x00, 0x00));  // 全局共享样式
    lv_obj_add_style(item_title_lb, def_text_style, 0);   //标签支持中文
    
    //设置滚轮
    lv_obj_t * item_spinbox = lv_spinbox_create(item_window);
    lv_spinbox_set_range(item_spinbox, 0, 99999);           // 设置范围
    lv_spinbox_set_digit_format(item_spinbox, 5, 0);    // 5位数字，0位小数
    
    lv_spinbox_set_cursor_pos(item_spinbox, 0);         // 设置选中个位（最低位）
    
    lv_spinbox_set_value(item_spinbox, 0);              // 初始值为0
    
    lv_obj_set_width(item_spinbox, 65);
    lv_obj_align_to(item_spinbox, item_window, LV_ALIGN_BOTTOM_MID, 0, 5);

    lv_coord_t h = lv_obj_get_height(item_spinbox);

    lv_obj_t * btn = lv_btn_create(item_window);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, myspinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  NULL);

    btn = lv_btn_create(item_window);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, myspinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);
    //价签
    lv_obj_t *item_price_title_lb=lv_label_create(main_screen_o);
    lv_obj_align_to(item_price_title_lb, item_window, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 2);

    //标签设置文字
    char item_price_str[50] = {0};   //转换价格数字到文本
    sprintf(item_price_str,"售价: %d CNY", items_price[0]);
    lv_label_set_text(item_price_title_lb, item_price_str);
    lv_obj_add_style(item_price_title_lb, def_text_style,0);
    //价格

}
