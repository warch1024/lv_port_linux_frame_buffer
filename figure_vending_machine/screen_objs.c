#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include "figure_vending_machine/screen_objs.h"
#include "tools/tools.h"
#include "figure_vending_machine/login_screen.h"
#include "figure_vending_machine/main_screen.h"
//#include "figure_vending_machine/figure_vending_machine.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include<stdio.h>

lv_obj_t                //管理所有页面对象
    * login_screen_page = NULL,
    * login_window = NULL, 
    * main_screen_page = NULL;
    
lv_style_t 
    * def_text_style = NULL;  //默认提示文字的style
    * 

