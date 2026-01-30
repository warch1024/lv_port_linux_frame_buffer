#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"


#include "tools/tools.h"
#include "26-1-29task/login_screen.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

 static lv_obj_t * login_page, * cn_kb,* user_ta, * psw_ta, *window1, *main_screen_page;
 static lv_style_t def_text_style;

void hidden_kb_cb(lv_event_cb_t * e){   //隐藏键盘回调
    lv_obj_add_flag(cn_kb, LV_OBJ_FLAG_HIDDEN);//显示键盘
}
void user_ta_cb(lv_event_cb_t *e ){
    lv_obj_clear_flag(cn_kb, LV_OBJ_FLAG_HIDDEN);//显示键盘
    lv_keyboard_set_textarea(cn_kb, user_ta);
    
}
void psw_ta_cb(lv_event_cb_t *e ){
    lv_obj_clear_flag(cn_kb, LV_OBJ_FLAG_HIDDEN);//显示键盘
    lv_keyboard_set_textarea(cn_kb, psw_ta);
    
}
void login_btn_cb(lv_event_cb_t *e){
//跳转界面
         //获取输入框输入的用户名和密码
    char *name=lv_textarea_get_text(user_ta);
    char *passwd=lv_textarea_get_text(psw_ta);

    //判断用户名和密码是否正确
    if(strcmp(name,"gec")==0 && strcmp(passwd,"123456")==0)
    {
        //跳到主界面
        main_screen();
        //关闭释放登录界面
        lv_obj_del(window1);
    }
}

void login_screen(){
    login_page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(login_page,800,480);
    //添加隐键盘回调函数
    lv_obj_add_event_cb(login_page, hidden_kb_cb, LV_EVENT_CLICKED, NULL);   

    create_font_style(&def_text_style,"/fonts/MSYH.TTC", 20);   //设置提示字体
    //添加小窗
    window1 = lv_obj_create(login_page);
    lv_obj_set_size(window1, 400, 200);
    //lv_obj_set_pos(window1, 200, 0);
    lv_obj_align(window1,LV_ALIGN_TOP_MID,0, 0); //设置对齐

    //lv_obj_set_pos(window1, 200, 140);
    //添加键盘
    cn_kb = tools_create_pinyin_ime(login_page, 500, 200);   //屏幕上添加键盘,默认隐藏
    //用户名输入框
    user_ta = lv_textarea_create(window1);    //添加用户名输入框
    //lv_obj_set_width(user_ta, 300);
    lv_obj_set_size(user_ta, 200, 50);  //设置尺寸
    lv_obj_align_to(user_ta,window1,LV_ALIGN_TOP_MID,0,-10);    //对齐到小窗
    lv_textarea_set_placeholder_text(user_ta, "输入用户名");
    lv_obj_add_style(user_ta, &def_text_style,0);
    lv_textarea_set_one_line(user_ta,true);  //限定在一行,框满不会换行,会一直向右增长行
    //lv_textarea_set_accepted_chars(user_ta, "0123456789");   //设置限定输入字符
    //lv_textarea_set_password_mode(user_ta, true); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(user_ta, user_ta_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数

    //密码输入框
    psw_ta = lv_textarea_create(window1);  //添加密码窗口
    lv_obj_set_size(psw_ta, 200, 50);
    lv_obj_align_to(psw_ta,user_ta,LV_ALIGN_TOP_MID,0,50);
    lv_textarea_set_placeholder_text(psw_ta, "输入密码");
    lv_obj_add_style(psw_ta, &def_text_style,0);
    lv_textarea_set_one_line(psw_ta,true);  //限定在一行,框满不会换行,会一直向右增长行
    lv_textarea_set_accepted_chars(psw_ta, "0123456789");   //设置限定输入字符
    lv_textarea_set_password_mode(psw_ta, true); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(psw_ta, psw_ta_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数
 
    //添加登录按钮
    lv_obj_t * login_bt = lv_btn_create(window1);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(login_bt, 80, 40);
    lv_obj_align_to(login_bt,psw_ta,LV_ALIGN_TOP_MID,0,50);
    lv_obj_add_event_cb(login_bt, login_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * login_bt_lb = lv_label_create(login_bt); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(login_bt_lb,login_bt ,LV_ALIGN_CENTER,-5,-2); //设置对齐
    lv_obj_add_style(login_bt_lb, &def_text_style, 0);   //给组件添加样式
    lv_label_set_text(login_bt_lb, "登录");   //给标签添加文字

    //软键盘跟文本框关联-->关联之后软键盘输入的字符才可以在文本框显示
    //lv_keyboard_set_textarea(kb,user_ta);

    
}

void main_screen()
{
    //创建小窗口
    main_screen_page=lv_obj_create(lv_scr_act());

    //设置menu2的大小
    lv_obj_set_size(main_screen_page,800,480);

     //创建标签
    lv_obj_t *lb1=lv_label_create(main_screen_page);
    lv_obj_set_align(lb1,LV_ALIGN_TOP_MID);
    //标签设置文字
    lv_obj_add_style(lb1,&def_text_style,0);
    lv_label_set_text(lb1,"粤嵌自助贩卖机主界面");
}
