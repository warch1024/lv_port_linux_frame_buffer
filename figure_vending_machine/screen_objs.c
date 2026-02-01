#include"figure_vending_machine/screen_objs.h"  //标准库

lv_obj_t                //管理所有页面对象
    * login_screen_page_o = NULL,   //登录界面
    * login_window_o = NULL,     //登录窗口
    * main_screen_page_o = NULL,   //主界面
    * logup_window_o = NULL;    //注册窗口
    
lv_style_t * def_text_style = NULL;  //默认提示文字的style

