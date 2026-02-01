#ifndef _SCREEN_OBJS_H
#define _SCREEN_OBJS_H

//lvgl头文件
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

//引入C语言标准库
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/input.h>  //跟输入子系统模型有关的头文件
#include <sys/mman.h>  //mmap的头文件
#include <errno.h>  //perror的头文件
#include<dirent.h>


extern lv_obj_t 
    * login_screen_page_o,
    * login_window_o, 
    * main_screen_page_o,
    * logup_window_o;

extern lv_style_t * def_text_style;
/*贩卖机主控制程序
*/
int figure_vending_machine();


#endif