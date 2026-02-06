#include "figure_vending_machine/login_screen.h"    


#define USER_RECORD_FILE "/IOT/projects/26-1-30/user_info_recorder.txt"

// #include"figure_vending_machine/screen_objs.h"
//登录屏幕的所有组件
 static lv_obj_t * login_window_o = NULL,     //登录窗口
    * user_ta= NULL, * psw_ta= NULL, * cf_psw_ta= NULL, //输入框
    * login_bt= NULL, * logup_bt= NULL, * sm_logup_bt= NULL, * bk_logup_bt= NULL,   //按钮
    * login_check_box1= NULL, * login_check_box2= NULL; //复选框

//登陆屏幕的所有样式
static lv_style_t * login_bg_style = NULL,  //登录屏幕背景设置样式
    * login_window_opa_style = NULL;     //登录窗口透明度设置样式

//键盘和中文支持
static cn_kb_cp_ros login_cn_kb_pair = {.cn_kb = NULL, .cn_kb_cp = NULL};
//默认主题图片路径
static char * def_login_theme_path = "S:/IOT/projects/26-1-30/resources/login_bg2.jpg"; 

//初始化登录屏幕基础组件
void login_screen_base_wigdet_init(lv_obj_t * screen){
   
    
    
}

void hidden_kb_cb(lv_event_t * e){   //隐藏键盘回调
    tools_hidden_pinyin_kb(login_cn_kb_pair.cn_kb, login_cn_kb_pair.cn_kb_cp);
    
}
void ta_kb_associate_cb(lv_event_t * e){
    //显示键盘
    tools_show_pinyin_kb(login_cn_kb_pair.cn_kb, login_cn_kb_pair.cn_kb_cp);
    //连接键盘和输入框
    tools_ta_kb_associate(e->target, login_cn_kb_pair.cn_kb);
}


void verify_user(){

}

void login_btn_cb(lv_event_t *e){   //登录按钮回调函数
//跳转界面
         //获取输入框输入的用户名和密码
    char *name=lv_textarea_get_text(user_ta);
    char *passwd=lv_textarea_get_text(psw_ta);

    //判断用户名和密码是否正确
    user_info_t user_info_tmp = {.user_name = {0}, .user_psw = {0}};
    strcpy(user_info_tmp.user_name, name);
    strcpy(user_info_tmp.user_psw, passwd);
    //debug
    printf("deng原%s\n%s\n",name, passwd);
    printf("deng堆%s\n%s\n",user_info_tmp.user_name, user_info_tmp.user_psw);
     printf("指针%p\n%p\n",&user_info_tmp, user_info_list);
     //管理员登录
     if(is_admin(&user_info_tmp)){
        printf("###################管理员#################\n");
        admin_management();
     }

    if(verify_user_name_psw(&user_info_tmp, user_info_list)){   //验证用户
        //写入用户信息到文件
        write_user_info_list_to_file(user_info_list, USER_RECORD_FILE);
        //跳到主界面
        main_screen();
        
    }
    else{
        printf("用户名或密码错误\n");
    }
}
void login_change_theme_checkbox_cb(lv_event_t * e){  //复选框点击回调函数
    lv_obj_remove_style(login_screen_o, login_bg_style, LV_PART_ANY | LV_STATE_ANY);  //移除背景
    //给主屏幕添加图片
    if(login_bg_style){
        if(e->target == login_check_box1){
            lv_obj_clear_state(login_check_box2, LV_STATE_CHECKED);//取消2选择
            // 设置背景图片
            lv_style_set_bg_img_src(login_bg_style, "S:/IOT/projects/26-1-30/resources/login_bg1.jpg");
            // 设置背景图片的裁剪（可选）
            lv_style_set_bg_img_opa(login_bg_style, LV_OPA_COVER);
            // 应用样式到屏幕
            lv_obj_add_style(login_screen_o, login_bg_style, 0);
            lv_obj_set_style_text_color(login_check_box1, lv_color_hex(0xffffff), LV_PART_MAIN);  // 设置为白色
            lv_obj_set_style_text_color(login_check_box2, lv_color_hex(0xffffff), LV_PART_MAIN);  // 设置为白色
        }

        else if(e->target == login_check_box2){
            lv_obj_clear_state(login_check_box1, LV_STATE_CHECKED);//取消1选择
            
            // 设置背景图片
            lv_style_set_bg_img_src(login_bg_style, "S:/IOT/projects/26-1-30/resources/login_bg2.jpg");
            // 设置背景图片的裁剪（可选）
            lv_style_set_bg_img_opa(login_bg_style, LV_OPA_COVER);
            
            // 应用样式到屏幕
            lv_obj_add_style(login_screen_o, login_bg_style, 0);
            lv_obj_set_style_text_color(login_check_box1, lv_color_hex(0x000000), LV_PART_MAIN);  // 设置为白色
            lv_obj_set_style_text_color(login_check_box2, lv_color_hex(0x000000), LV_PART_MAIN);  // 设置为白色
        }
    }
}
void load_def_login_theme(lv_obj_t * screen_o, char * def_theme_path){  //登陆界面的默认主题
    if(!login_bg_style){    //只初始化一次，存放style

        login_bg_style = (lv_style_t * )malloc(sizeof(lv_style_t));
    }
    lv_style_init(login_bg_style);
    lv_obj_add_state(login_check_box2, LV_STATE_CHECKED);//默认主题2
    //lv_obj_remove_style(login_screen_page_o, login_bg_style, LV_PART_ANY | LV_STATE_ANY);  //移除背景
    
    // 设置背景图片
    lv_style_set_bg_img_src(login_bg_style, def_theme_path);
    // 设置背景图片的裁剪（可选）
    lv_style_set_bg_img_opa(login_bg_style, LV_OPA_COVER);
    
    // 应用样式到屏幕
    lv_obj_add_style(login_screen_o, login_bg_style, 0);
}


void add_switch_full_screen_theme_checkbox(lv_obj_t * full_screen_o){    //给屏幕添加复选框，切换主题

    login_check_box1 = tools_create_login_checkbox(full_screen_o, -1, -1, "CYBER"); //新建复选框
    lv_obj_align_to(login_check_box1, full_screen_o, LV_ALIGN_TOP_LEFT, 0, 0);     //对齐位置
    //添加点击复选框切换主题回调函数
    lv_obj_add_event_cb(login_check_box1, login_change_theme_checkbox_cb, LV_EVENT_VALUE_CHANGED, NULL);    //添加选取回调函数
    
    login_check_box2 = tools_create_login_checkbox(full_screen_o, -1, -1, "默认主题"); //复选框2
    lv_obj_align_to(login_check_box2, login_check_box1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);    //对齐到第一个复选框
    //添加点击复选框切换主题回调函数
    lv_obj_add_event_cb(login_check_box2, login_change_theme_checkbox_cb, LV_EVENT_VALUE_CHANGED, NULL);    //添加选取回调函数
}

void login_screen(){        //主登录界面
    //初始化用户信息链表
    init_user_info_list(USER_RECORD_FILE);
     //创建登录屏幕
    if(!login_screen_o){   //避免重复申请内存
        login_screen_o = lv_obj_create(NULL);  
    }
    //设置尺寸
    lv_obj_set_size(login_screen_o,800,480);
     //添加键盘
    login_cn_kb_pair =  tools_create_pinyin_ime(login_screen_o, 500, 200);   //屏幕上添加键盘,默认隐藏
    //添加隐键盘回调函数
    lv_obj_add_event_cb(login_screen_o, hidden_kb_cb, LV_EVENT_CLICKED, NULL); //点击对象隐藏键盘

    //添加主题切换复选框
    add_switch_full_screen_theme_checkbox(login_screen_o);
    //加载默认主题
    load_def_login_theme(login_screen_o, def_login_theme_path);

    //加载登录窗口
    login_window(); 
    //加载新屏幕动画
    lv_scr_load(login_screen_o);

}


void login_window(){    //登录界面
    
    //添加小窗
    if(!login_window_o){    //避免内存溢出

        login_window_o = lv_obj_create(login_screen_o);
    }
    //设置小窗大小和对齐
    lv_obj_set_size(login_window_o, 300, 200);
    lv_obj_align(login_window_o,LV_ALIGN_TOP_MID,0, 0); //设置对齐
    if(!login_window_opa_style){ //登录窗口的透明度主题
        login_window_opa_style = (lv_style_t *)malloc(sizeof(lv_style_t));
    }
    tools_set_opa_style(login_window_opa_style, login_window_o, LV_OPA_50);

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
    //添加按钮点击回调函数
    lv_obj_add_event_cb(login_bt, login_btn_cb, LV_EVENT_CLICKED, NULL);   

    //给按钮添加标签
    lv_obj_t * login_bt_lb = tools_create_login_bt_lb(login_bt, "登录");

     //添加注册按钮
    logup_bt = lv_btn_create(login_window_o);   
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(logup_bt, 80, 40);
    lv_obj_align_to(logup_bt,psw_ta,LV_ALIGN_OUT_BOTTOM_RIGHT,0,4); //对齐到登录按钮
    //添加按钮点击回调函数
    lv_obj_add_event_cb(logup_bt, logup_btn_cb, LV_EVENT_CLICKED, NULL);   

    //给按钮添加标签
    lv_obj_t * logup_bt_lb = tools_create_login_bt_lb(logup_bt, "注册"); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    
    //设置动画
    //lv_obj_fade_in(login_screen_o, 800, 0);
}

void logup_btn_cb(lv_event_t * e){  //注册回调函数
    logup_window(); //打开注册窗口

}
void logup_window(){    //注册界面
    //删除登录和注册按钮
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
    //设置动画
    //lv_obj_fade_in(login_screen_o, 800, 0);
}

//点击提交注册按钮回调函数
void sm_logup_btn_cb(lv_event_t * e){  //注册提交按钮点击回调服务
    
    if(user_ta && psw_ta && cf_psw_ta){
        //获取注册信息
        char *logup_name=lv_textarea_get_text(user_ta);
        char *logup_passwd=lv_textarea_get_text(psw_ta);
        char *logup_cf_passwd=lv_textarea_get_text(cf_psw_ta);
        if(strcmp(logup_passwd, logup_cf_passwd) == 0){ //密码设置正确
            user_info_t *user_info = (user_info_t *)malloc(sizeof(user_info_t));    //保存用户信息节点
            strcpy(user_info->user_name, logup_name);
            strcpy(user_info->user_psw, logup_passwd);
            add_user_info_to_list(user_info_list, user_info);
             //注册成功，返回登陆界面
            lv_obj_del(login_window_o); // 删除登录窗口，重新注册登录窗口
            login_window_o = NULL;
            //debug
            printf("zhu原%s\n%s\n",logup_name, logup_passwd);
            printf("zhu堆%s\n%s\n",user_info->user_name, user_info->user_psw);
            login_window();
        }
        else{
            printf("密码设置错误\n");
            //lv_textarea_set_text(user_ta, "");
            lv_textarea_set_text(psw_ta, "");
            lv_textarea_set_text(cf_psw_ta, "");
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


void admin_management(){    //管理员管理程序
    // 在主菜单或其他适当位置

        //初始化全局listed_list
    init_def_goods_to_list();
    //初始化已加购列表
    init_shopping_cart_added_list();
    //创建标签
    lv_obj_t *main_screen_admin_lb=lv_label_create(main_screen_o);
    lv_obj_set_align(main_screen_admin_lb,LV_ALIGN_TOP_RIGHT);
    //标签设置文字
    lv_label_set_text(main_screen_admin_lb,"管理员");
    lv_obj_add_style(main_screen_admin_lb, def_text_style, 1);
    
    // lv_obj_invalidate(login_screen_o); //刷新
    // lv_obj_invalidate(main_screen_admin_lb); //刷新
    show_goods_management_menu();   //显示管理面板

    main_screen();  //重新初始化
}

