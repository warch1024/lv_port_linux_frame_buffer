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


extern lv_obj_t                //管理所有屏幕
    * login_screen_o,   //登录界面
    * main_screen_o;   //主界面

extern lv_style_t * def_text_style; //默认提示文字的style



//对象键值对node
typedef struct Lv_obj_pn{       //存储lv对象
    char name[50]; //对象名
    lv_obj_t * obj; //对象地址
    struct Lv_obj_pn * prev;
    struct Lv_obj_pn * next;
    struct Lv_obj_pn * tail;
}lv_obj_pn;


/*贩卖机主控制程序
*/
int figure_vending_machine();


#endif