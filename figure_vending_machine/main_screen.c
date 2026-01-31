#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"


#include "tools/tools.h"
#include "figure_vending_machine/login_screen.h"
#include "figure_vending_machine/main_screen.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include<stdio.h>

void main_screen()
{
    //创建主窗口
    main_screen_page=lv_obj_create(lv_scr_act());

    //设置menu2的大小
    lv_obj_set_size(main_screen_page,800,480);

     //创建标签
    lv_obj_t *lb1=lv_label_create(main_screen_page);
    lv_obj_set_align(lb1,LV_ALIGN_TOP_MID);
    //标签设置文字
    lv_obj_add_style(lb1,&def_text_style,0);
    lv_label_set_text(lb1,"粤嵌自助贩卖机主界面");

    //新建3个小窗
    lv_obj_t * item1_window = lv_obj_create(main_screen_page),
        * item2_window = lv_obj_create(main_screen_page),
        * item3_window = lv_obj_create(main_screen_page);
    
    lv_obj_set_size(item1_window,250,350);
    lv_obj_set_size(item2_window,250,350);
    lv_obj_set_size(item3_window,250,350);

    lv_obj_align_to(item1_window, main_screen_page, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_align_to(item2_window, main_screen_page, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_align_to(item3_window, main_screen_page, LV_ALIGN_TOP_RIGHT, 0, 50);
    //新建3个图片对象
    lv_obj_t * item1_img = lv_img_create(item1_window),
        *item2_img = lv_img_create(item2_window),
        *item3_img = lv_img_create(item3_window);
    
    lv_img_set_src(item1_img, "S:/IOT/projects/26-1-30/resources/item1.jpg"); // 设置图片路径名
    lv_img_set_src(item2_img, "S:/IOT/projects/26-1-30/resources/item2.jpg"); // 设置图片路径名
    lv_img_set_src(item3_img, "S:/IOT/projects/26-1-30/resources/item3.jpg"); // 设置图片路径名

}

