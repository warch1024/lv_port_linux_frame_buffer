#include "figure_vending_machine/login_screen.h"    

// #include"figure_vending_machine/screen_objs.h"

 static lv_obj_t * user_ta, * psw_ta, * cf_psw_ta,
    * login_bt, * logup_bt, * sm_logup_bt, * bk_logup_bt;

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

void login_btn_cb(lv_event_t *e){   //登录按钮回调函数
//跳转界面
         //获取输入框输入的用户名和密码
    char *name=lv_textarea_get_text(user_ta);
    char *passwd=lv_textarea_get_text(psw_ta);

    //判断用户名和密码是否正确
    if(1 || strcmp(name,"gec")==0 && strcmp(passwd,"123456")==0)
    {
        //关闭释放登录界面
        lv_obj_del(login_window_o);
        login_window_o = NULL;
        lv_obj_del(login_screen_page_o);
        login_screen_page_o = NULL;
        //跳到主界面
        main_screen();
    }
}

void login_screen(){        //登陆界面

    if(!login_screen_page_o){   //避免内存溢出
        login_screen_page_o = lv_obj_create(lv_scr_act());
    }
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

    login_window(); //显示登录框
}


void login_window(){    //登录界面
    
    //添加小窗
    if(!login_window_o){    //避免内存溢出

        login_window_o = lv_obj_create(login_screen_page_o);
    }
    lv_obj_set_size(login_window_o, 300, 200);
    lv_obj_align(login_window_o,LV_ALIGN_TOP_MID,0, -10); //设置对齐

    //用户名输入框
    user_ta = tools_create_login_ta(login_window_o, 200, 40, "输入用户名");
    lv_obj_align_to(user_ta,login_window_o,LV_ALIGN_TOP_MID,0,-10);    //对齐到窗口
    lv_obj_add_event_cb(user_ta, ta_kb_associate_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数

    //密码输入框
    psw_ta = tools_create_login_ta(login_window_o, 200, 40, "输入密码");  //添加密码窗口
    lv_obj_align_to(psw_ta,user_ta,LV_ALIGN_OUT_BOTTOM_MID,0,5);

    lv_textarea_set_accepted_chars(psw_ta, "0123456789");   //设置限定输入字符
    lv_textarea_set_password_mode(psw_ta, true); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(psw_ta, ta_kb_associate_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数
    
 
    //添加登录按钮
    login_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(login_bt, 80, 40);
    lv_obj_align_to(login_bt,psw_ta,LV_ALIGN_OUT_BOTTOM_LEFT,0,4);
    lv_obj_add_event_cb(login_bt, login_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * login_bt_lb = tools_create_login_bt_lb(login_bt, "登录");

     //添加注册按钮
    logup_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(logup_bt, 80, 40);
    lv_obj_align_to(logup_bt,psw_ta,LV_ALIGN_OUT_BOTTOM_RIGHT,0,4); //对齐到登录按钮
    lv_obj_add_event_cb(logup_bt, logup_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * logup_bt_lb = tools_create_login_bt_lb(logup_bt, "注册"); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上

}

void logup_btn_cb(lv_event_t * e){  //注册回调函数
    logup_window(); //打开注册窗口

}
void logup_window(){    //注册界面
    //删除按钮
    if(login_bt && logup_bt){   
        lv_obj_del(login_bt);
        login_bt = NULL;
        lv_obj_del(logup_bt);
        logup_bt = NULL;
    }
    
    //调整登录窗口
    lv_textarea_set_text(user_ta, "");  //清空用户名输入框
    lv_textarea_set_placeholder_text(user_ta, "");  //清空提示
    lv_textarea_set_placeholder_text(user_ta, "设置用户名");
 
    //密码输入框
    lv_textarea_set_text(psw_ta, "");  //清空用户名输入框
    lv_textarea_set_placeholder_text(psw_ta, "");  //清空提示
    lv_textarea_set_placeholder_text(psw_ta, "设置密码(小于9位)");
    lv_textarea_set_password_mode(psw_ta, false); //关闭密码模式

    //确认密码框
    cf_psw_ta = tools_create_login_ta(login_window_o, 200, 40, "重复设置密码");
    lv_obj_align_to(cf_psw_ta,psw_ta,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_textarea_set_accepted_chars(cf_psw_ta, "0123456789");   //设置限定输入字符
    lv_textarea_set_password_mode(cf_psw_ta, false); //密码输入使用*代替,开启密码模式
    lv_obj_add_event_cb(cf_psw_ta, ta_kb_associate_cb,LV_EVENT_FOCUSED,NULL);   //添加输入框回调函数
 
     //添加确认注册按钮
    sm_logup_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(sm_logup_bt, 80, 40);
    lv_obj_align_to(sm_logup_bt,cf_psw_ta,LV_ALIGN_OUT_BOTTOM_LEFT, 0, 4); //对齐到登录按钮
    lv_obj_add_event_cb(sm_logup_bt, sm_logup_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * sm_logup_bt_lb = tools_create_login_bt_lb(sm_logup_bt, "提交");

     //添加返回登陆界面按钮
    bk_logup_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(bk_logup_bt, 80, 40);
    lv_obj_align_to(bk_logup_bt,cf_psw_ta,LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 4); //对齐到登录按钮
    lv_obj_add_event_cb(bk_logup_bt, bk_logup_btn_cb, LV_EVENT_CLICKED, NULL);   //添加按钮点击回调函数

    //给按钮添加标签
    lv_obj_t * bk_logup_bt_lb = tools_create_login_bt_lb(bk_logup_bt, "返回"); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上

}


void sm_logup_btn_cb(lv_event_t * e){  //注册提交按钮点击回调服务
    user_register("/IOT/projects/26-1-30/user_info.txt");
    if(user_ta && psw_ta && cf_psw_ta){
        char *logup_name=lv_textarea_get_text(user_ta);
        char *logup_passwd=lv_textarea_get_text(psw_ta);
        char *logup_cf_passwd=lv_textarea_get_text(cf_psw_ta);
        if(strcmp(logup_passwd, logup_cf_passwd) == 0){
            //注册成功，返回登陆界面
            lv_obj_del(login_window_o); // 删除登录窗口，重新注册登录窗口
            login_window_o = NULL;
            login_window();
        }
    }
}



void bk_logup_btn_cb(lv_event_t * e){    //返回主界面
    lv_obj_del(login_window_o);  //登录窗口与全子
    login_window_o = NULL;
    login_window();
}

void save_logup_info(){ //保存用户信息
    
}