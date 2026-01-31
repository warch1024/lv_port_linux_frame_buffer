#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
//贩卖机自定义头文件
//figure_vending_machine/figure_vending_machine.c
#include "figure_vending_machine/figure_vending_machine.h"      
#include "tools/tools.h"
#include "figure_vending_machine/main_screen.h"
#include "figure_vending_machine/login_screen.h"
#include "figure_vending_machine/screen_objs.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include<stdio.h>




int figure_vending_machine(){
    login_screen(); //显示登陆界面
}
