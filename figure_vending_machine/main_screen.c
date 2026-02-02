
#include"figure_vending_machine/main_screen.h"

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

    //新建3个小窗

    lv_obj_t * item1_window = lv_obj_create(main_screen_o),
        * item2_window = lv_obj_create(main_screen_o),
        * item3_window = lv_obj_create(main_screen_o),
        * item4_window = lv_obj_create(main_screen_o);
    
    lv_obj_set_size(item1_window,250,350);
    lv_obj_set_size(item2_window,250,350);
    lv_obj_set_size(item3_window,250,350);
    lv_obj_set_size(item4_window,250,350);

    lv_obj_align_to(item1_window, main_screen_o, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_align_to(item2_window, item1_window, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(item3_window, item2_window, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(item4_window, item3_window, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    //新建3个图片对象
    lv_obj_t * item1_img = lv_img_create(item1_window),
        *item2_img = lv_img_create(item2_window),
        *item3_img = lv_img_create(item3_window);
        
    
    lv_img_set_src(item1_img, "S:/IOT/projects/26-1-30/resources/item1.jpg"); // 设置图片路径名
    lv_img_set_src(item2_img, "S:/IOT/projects/26-1-30/resources/item2.jpg"); // 设置图片路径名
    lv_img_set_src(item3_img, "S:/IOT/projects/26-1-30/resources/item3.jpg"); // 设置图片路径名

}

