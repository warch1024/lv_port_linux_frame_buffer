#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include "figure_vending_machine/login_screen.h"
#include "tools/tools.h"
#include "figure_vending_machine/main_screen.h"
#include "figure_vending_machine/screen_objs.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include<stdio.h>

 static lv_obj_t * user_ta, * psw_ta, * cf_psw_ta,
    * login_bt, * logup_bt, * sm_logup_bt;

void hidden_kb_cb(lv_event_t * e){   //隐藏键盘回调
    tools_hidden_pinyin_kb(tools_cn_kb, tools_cand_pannel);
    
}
void ta_kb_associate_cb(lv_event_t * e){
    tools_show_pinyin_kb(tools_cn_kb, tools_cand_pannel);
    if(e->target == user_ta){
        lv_keyboard_set_textarea(tools_cn_kb, user_ta);
    }
    else if(e->target == psw_ta){
        lv_keyboard_set_textarea(tools_cn_kb, psw_ta);
    }
    else if(e->target == cf_psw_ta){
        lv_keyboard_set_textarea(tools_cn_kb, cf_psw_ta);
    }
}

void login_btn_cb(lv_event_t *e){
//跳转界面
         //获取输入框输入的用户名和密码
    char *name=lv_textarea_get_text(user_ta);
    char *passwd=lv_textarea_get_text(psw_ta);

    //判断用户名和密码是否正确
    if(1 || strcmp(name,"gec")==0 && strcmp(passwd,"123456")==0)
    {
        //跳到主界面
        main_screen();
        //关闭释放登录界面
        lv_obj_del(login_window_o);
    }
}

void login_screen(){        //登陆界面
    
    login_screen_page_o = lv_obj_create(lv_scr_act());
    lv_obj_set_size(login_screen_page_o,800,480);
     //添加键盘
    tools_create_pinyin_ime(login_screen_page_o, 500, 200);   //屏幕上添加键盘,默认隐藏
    //添加隐键盘回调函数
    lv_obj_add_event_cb(login_screen_page_o, hidden_kb_cb, LV_EVENT_CLICKED, NULL);   

    def_text_style = (lv_style_t * )malloc(sizeof(lv_style_t)); //申请字体style
    if(def_text_style != NULL){
        tools_create_font_style(def_text_style,"/fonts/MSYH.TTC", 20);   //设置提示字体
    }
    else{
        perror("内存申请失败");
    }

     //添加小窗
    login_window_o = lv_obj_create(login_screen_page_o);
    lv_obj_set_size(login_window_o, 400, 200);
    //lv_obj_set_pos(window1, 200, 0);
    lv_obj_align(login_window_o,LV_ALIGN_TOP_MID,0, 0); //设置对齐

   

    login_window(); //显示登录框
}


void login_window(){    //登录界面
    
    //lv_obj_set_pos(window1, 200, 140);
   
    //用户名输入框
    user_ta = lv_textarea_create(login_window_o);    //添加用户名输入框
    //lv_obj_set_width(user_ta, 300);
    lv_obj_set_size(user_ta, 200, 50);  //设置尺寸
    lv_obj_align_to(user_ta,login_window_o,LV_ALIGN_TOP_MID,0,-10);    //对齐到小窗
    lv_textarea_set_placeholder_text(user_ta, "输入用户名");
    lv_obj_add_style(user_ta, def_text_style,0);
    lv_textarea_set_one_line(user_ta,true);  //限定在一行,框满不会换行,会一直向右增长行
    lv_textarea_set_max_length(user_ta ,8);  //限制用户名长度
    //lv_textarea_set_accepted_chars(user_ta, "0123456789");   //设置限定输入字符
    //lv_textarea_set_password_mode(user_ta, true); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(user_ta, ta_kb_associate_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数

    //密码输入框
    psw_ta = lv_textarea_create(login_window_o);  //添加密码窗口
    lv_obj_set_size(psw_ta, 200, 50);
    lv_obj_align_to(psw_ta,user_ta,LV_ALIGN_TOP_MID,0,50);
    lv_textarea_set_placeholder_text(psw_ta, "输入密码");
    lv_obj_add_style(psw_ta, def_text_style,0);
    lv_textarea_set_one_line(psw_ta,true);  //限定在一行,框满不会换行,会一直向右增长行
    lv_textarea_set_accepted_chars(psw_ta, "0123456789");   //设置限定输入字符
    lv_textarea_set_max_length(psw_ta ,8);  //限制密码长度
    lv_textarea_set_password_mode(psw_ta, true); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(psw_ta, ta_kb_associate_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数
    
 
    //添加登录按钮
    login_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(login_bt, 80, 40);
    lv_obj_align_to(login_bt,psw_ta,LV_ALIGN_TOP_RIGHT,0,50);
    lv_obj_add_event_cb(login_bt, login_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * login_bt_lb = lv_label_create(login_bt); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(login_bt_lb,login_bt ,LV_ALIGN_CENTER,-5,-2); //设置对齐
    lv_obj_add_style(login_bt_lb, def_text_style, 0);   //给组件添加样式
    lv_label_set_text(login_bt_lb, "登录");   //给标签添加文字

     //添加注册按钮
    logup_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(logup_bt, 80, 40);
    lv_obj_align_to(logup_bt,login_bt,LV_ALIGN_RIGHT_MID,20,0); //对齐到登录按钮
    lv_obj_add_event_cb(logup_bt, logup_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * logup_bt_lb = lv_label_create(logup_bt); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(logup_bt_lb,logup_bt ,LV_ALIGN_CENTER,-5,-2); //设置对齐
    lv_obj_add_style(logup_bt_lb, def_text_style, 0);   //给组件添加样式
    lv_label_set_text(logup_bt_lb, "注册");   //给标签添加文字

    //软键盘跟文本框关联-->关联之后软键盘输入的字符才可以在文本框显示
    //lv_keyboard_set_textarea(kb,user_ta);
}



void logup_btn_cb(lv_event_t * e){
    logup_window();
    if(user_ta && psw_ta && cf_psw_ta){
        char *logup_name=lv_textarea_get_text(user_ta);
        char *logup_passwd=lv_textarea_get_text(psw_ta);
        char *logup_cf_passwd=lv_textarea_get_text(cf_psw_ta);
        if(strcmp(logup_passwd, logup_cf_passwd) == 0){
            //注册成功，返回登陆界面
            back_to_login_screen();
        }
    }
}

void back_to_login_screen(){
    printf("返回等克鲁");
}

void logup_window(){    //注册界面
    //隐藏按钮
    if(login_bt && logup_bt){
        lv_obj_add_flag(login_bt, LV_OBJ_FLAG_HIDDEN);//隐藏按钮
        lv_obj_add_flag(logup_bt, LV_OBJ_FLAG_HIDDEN);//隐藏按钮
    }
    
    //lv_obj_set_size(login_window_o, 400, 250);  //调整登录窗口
    lv_textarea_set_text(user_ta, "");  //清空用户名输入框
    lv_textarea_set_placeholder_text(user_ta, "");  //清空提示
    lv_textarea_set_placeholder_text(user_ta, "设置用户名");
 
    //密码输入框
    lv_textarea_set_text(psw_ta, "");  //清空用户名输入框
    lv_textarea_set_placeholder_text(psw_ta, "");  //清空提示
    lv_textarea_set_placeholder_text(psw_ta, "设置密码(小于9位)");
    lv_textarea_set_password_mode(psw_ta, false); //关闭密码模式

    //确认密码框
    cf_psw_ta = lv_textarea_create(login_window_o);  //添加密码窗口
    lv_obj_set_size(cf_psw_ta, 200, 50);
    lv_obj_align_to(cf_psw_ta,psw_ta,LV_ALIGN_TOP_MID,0,50);
    lv_textarea_set_placeholder_text(cf_psw_ta, "重复设置密码");
    lv_obj_add_style(cf_psw_ta, def_text_style,0);
    lv_textarea_set_one_line(cf_psw_ta,true);  //限定在一行,框满不会换行,会一直向右增长行
    lv_textarea_set_accepted_chars(cf_psw_ta, "0123456789");   //设置限定输入字符
    lv_textarea_set_max_length(cf_psw_ta ,8);  //限制密码长度
    lv_textarea_set_password_mode(cf_psw_ta, false); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(cf_psw_ta, ta_kb_associate_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数
 
     //添加确认注册按钮
    sm_logup_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(sm_logup_bt, 80, 40);
    lv_obj_align_to(sm_logup_bt,cf_psw_ta,LV_ALIGN_TOP_MID, 0, 50); //对齐到登录按钮
    lv_obj_add_event_cb(sm_logup_bt, logup_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * sm_logup_bt_lb = lv_label_create(logup_bt); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(sm_logup_bt_lb,logup_bt ,LV_ALIGN_CENTER,-5,-2); //设置对齐
    lv_obj_add_style(sm_logup_bt_lb, def_text_style, 0);   //给组件添加样式
    lv_label_set_text(sm_logup_bt_lb, "提交");   //给标签添加文字

}
