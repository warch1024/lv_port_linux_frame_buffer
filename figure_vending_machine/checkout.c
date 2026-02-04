#include"figure_vending_machine/checkout.h"

//管理页面对象
static lv_obj_t * shopping_cart_list = NULL,
    * shopping_cart_window = NULL;


void shopping_cart(lv_obj_t * parent_o){
    //添加购物车按钮
    lv_obj_t * shopping_cart_btn = lv_btn_create(parent_o);
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(shopping_cart_btn, 150, 50);
    lv_obj_align_to(shopping_cart_btn, parent_o, LV_ALIGN_BOTTOM_LEFT, 20, -10);
    //添加按钮点击回调函数，点击按钮弹出窗口
    lv_obj_add_event_cb(shopping_cart_btn, shopping_cart_btn_cb, LV_EVENT_CLICKED, NULL);

    //给按钮添加标签
    lv_obj_t * shopping_cart_btn_lb = lv_label_create(shopping_cart_btn);
    //显示购物车商品数量
    static uint32_t shopping_cart_num = 0;  //显示购物车商品数量
    char shopping_cart_btn_lb_text[50] = {"\0"};
    sprintf(shopping_cart_btn_lb_text, "购物车(%d)", shopping_cart_num);
    //按钮标签
    lv_label_set_text(shopping_cart_btn_lb, shopping_cart_btn_lb_text); //设置标签文字
    lv_obj_align_to(shopping_cart_btn_lb, shopping_cart_btn, LV_ALIGN_CENTER, -2, -4);//对齐标签
    lv_obj_add_style(shopping_cart_btn_lb, def_text_style, 0);   //标签支持中文
    
    //购物车小窗
    shopping_cart_window = lv_obj_create(parent_o);
    lv_obj_align(shopping_cart_window, LV_ALIGN_TOP_MID, 0, 0); //对齐小窗

    lv_obj_add_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN);  //隐藏小窗
    lv_obj_set_size(shopping_cart_window, 400, 450);    //设置大小
    lv_obj_set_style_bg_color(shopping_cart_window, lv_color_make(255, 201, 14), 0);

    //购物车标签
    lv_obj_t * shopping_cart_title = lv_label_create(shopping_cart_window); 
    lv_obj_align_to(shopping_cart_title,shopping_cart_window, LV_ALIGN_TOP_MID, -15, -12);  //对齐标题
    lv_label_set_text(shopping_cart_title, "已加购项目"); //设置标签文字
    lv_obj_add_style(shopping_cart_title, def_text_style, 0);   //标签支持中文

    //创建列表在屏幕
    shopping_cart_list = lv_list_create(parent_o);
    lv_obj_add_flag(shopping_cart_list, LV_OBJ_FLAG_HIDDEN);  //隐藏小窗
    
    lv_obj_set_size(shopping_cart_list, 390, 405);
    //列表对齐
    lv_obj_align(shopping_cart_list, LV_ALIGN_TOP_MID, 0, 40);
    
    lv_obj_add_style(shopping_cart_list, def_text_style, 0);   //列表支持中文

}

void shopping_cart_btn_cb(lv_event_t * e){
    if(lv_obj_has_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN)){

        lv_obj_clear_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(shopping_cart_list, LV_OBJ_FLAG_HIDDEN);
    }
    else{
        lv_obj_add_flag(shopping_cart_window, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(shopping_cart_list, LV_OBJ_FLAG_HIDDEN);
    }
} 

//给购物车列表添加已加购项目
void refresh_shopping_cart_list(lv_obj_t * list, wish_list_t * wish_list){  //刷新购物车加购列表
    if(wish_list) {                                       // 遍历链表添加到加购列表
        for(wish_list = wish_list->next; wish_list != NULL; wish_list = wish_list->next) {

            // 添加列表按钮项目
            char goods_title_item[50] = {"\0"};
            sprintf(goods_title_item, "%s( %d × %d )",wish_list->goods->title, wish_list->num, wish_list->goods->price); //
            lv_list_add_btn(list, NULL, goods_title_item);
        }
    }
}

//增加商品到加购列表
int add_wish_list_goods(wish_list_t * wish_list, goods_info_t * goods){    //保存新建对象到链表
    if(wish_list){
        for(wish_list = wish_list->next; wish_list != NULL; wish_list = wish_list->next) {
            if(goods == wish_list->goods){
                wish_list->num ++;
                return 1;
            }
        }
    }
    return 0;
}
void init_goods_list(){

}
