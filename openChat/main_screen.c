
#include"openChat/main_screen.h"


//主屏幕的对象
lv_obj_t * open_chat_main_interface_o = NULL;//聊天界面的主容器对象
static lv_obj_t * main_screen_title_bg = NULL,//主屏幕的标题背景对象
                * main_screen_title_lb = NULL,//主屏幕的标题标签对象
                * open_chat_ol_list = NULL,//聊天界面的在线用户列表框对象
                
                * open_chat_input_interface_o = NULL,//聊天界面的输入框对象
                * open_chat_input_ta = NULL,//聊天界面的输入框文本对象
                * open_chat_send_msg_btn = NULL,//聊天界面的发送消息按钮对象
                * open_chat_send_file_btn = NULL,//聊天界面的发送文件按钮对象
                * open_chat_send_emoti_btn = NULL;//聊天界面的发送表情按钮对象
static cn_kb_cp_ros open_chat_screen_cn_kb_pair = {.cn_kb = NULL, .cn_kb_cp = NULL};//主屏幕的中文键盘对象
static struct clientmsg * oc_current_chat_client = NULL;//当前聊天的客户端对象
//主屏幕入口函数
void main_screen_init(){

    //加载主屏幕的功能
    //删除登录屏幕
    // lv_obj_del(login_screen_o);
    // login_screen_o = NULL;
    // printf("debug: 登录屏幕已删除\n");
    main_screen();
}
void oc_hidden_kb_cb(lv_event_t * e){   //隐藏键盘回调
    tools_hidden_pinyin_kb(open_chat_screen_cn_kb_pair.cn_kb, open_chat_screen_cn_kb_pair.cn_kb_cp);
    //将输入框所在容器移动到键盘下方
    move_input_interface_default(open_chat_input_interface_o, open_chat_main_interface_o);
}
void oc_ta_kb_associate_cb(lv_event_t * e){
    //显示键盘
    tools_show_pinyin_kb(open_chat_screen_cn_kb_pair.cn_kb, open_chat_screen_cn_kb_pair.cn_kb_cp);
    //连接键盘和输入框
    tools_ta_kb_associate(lv_event_get_target(e), open_chat_screen_cn_kb_pair.cn_kb);
    //将输入框所在容器移动到键盘上方
    move_input_interface_above_kb(open_chat_input_interface_o, open_chat_screen_cn_kb_pair.cn_kb);
}



//阶段2 主屏幕
void main_screen(){
   

    // //初始化全局listed_list
    // init_def_goods_to_list();   //初始化商品列表
    // //初始化已加购列表
    // init_shopping_cart_added_list();     //初始化加购列表

    //创建主屏幕screen
    main_screen_o=lv_obj_create(NULL);
    //设置大小
    lv_obj_set_size(main_screen_o,800,480);
    // 设置背景颜色为白色
    lv_obj_set_style_bg_color(main_screen_o, lv_color_hex(0xd4e9f7), LV_PART_MAIN);  
    // // 添加商品卡片到主屏幕
    // add_page_4_card_(main_screen_o, listed_goods);
    //添加键盘到主屏幕
    
    //debug
    //debug 显示键盘
    // lv_obj_add_event_cb(main_screen_o, oc_hidden_kb_cb, LV_EVENT_CLICKED, NULL); //添加点击屏幕隐藏键盘事件
    
    //添加openchat主界面到屏幕
    load_open_chat_main_interface(main_screen_o);
    //添加键盘到主屏幕
    printf("主屏幕完成添加openchat主界面\n");
    printf("键盘对对象大小：%d, kb地址%p, cp地址%p\n", sizeof(open_chat_screen_cn_kb_pair), open_chat_screen_cn_kb_pair.cn_kb, open_chat_screen_cn_kb_pair.cn_kb_cp);
    open_chat_screen_cn_kb_pair =  tools_create_pinyin_ime(main_screen_o, 500, 200);   //屏幕上添加键盘,默认隐藏
 //    tools_show_pinyin_kb(open_chat_screen_cn_kb_pair.cn_kb, open_chat_screen_cn_kb_pair.cn_kb_cp);
    printf("debug: 键盘加载完成%p\n", open_chat_screen_cn_kb_pair.cn_kb);
    
    // 添加主屏幕标题
    // 创建标签背景
    main_screen_title_bg = lv_obj_create(main_screen_o);
    lv_obj_set_size(main_screen_title_bg, 150,25);
    lv_obj_set_align(main_screen_title_bg,LV_ALIGN_TOP_MID);
    //lv_obj_set_style_radius(main_screen_title_bg, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(main_screen_title_bg, 0, LV_PART_MAIN);//去除边框
    // lv_obj_set_style_bg_opa(main_screen_title_bg, 50,LV_PART_MAIN);
    //创建标签
    main_screen_title_lb = lv_label_create(main_screen_o);
   lv_obj_set_align(main_screen_title_lb,LV_ALIGN_TOP_MID);
   //标签设置文字
   lv_label_set_text(main_screen_title_lb,"openChat");
   lv_obj_add_style(main_screen_title_lb,def_text_style,0);
   
   // //添加购物车相关功能
   // lv_obj_t * shopping_cart_btn_lb = shopping_cart(main_screen_o);
   // //添加结算按钮相关功能
   // settlement(main_screen_o, shopping_cart_added_list);
   //
   //加载主界面删除旧窗口（login）使用动画转场
   lv_scr_load_anim(main_screen_o, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, false);
   //删除登录屏幕
//    lv_obj_del(login_screen_o);
//    login_screen_o = NULL;
//    printf("debug: 登录屏幕已删除\n");
}

 




///////////////openchat主界面
void load_open_chat_main_interface(lv_obj_t * main_screen_o){   //传入主屏对象
    //debug
    printf("debug: 加载主界面\n");
    lv_coord_t main_screen_h = lv_obj_get_height(main_screen_o);    //获取主界面高度
    lv_coord_t main_screen_w = lv_obj_get_width(main_screen_o); //获取主界面宽度
    //创建在线客户端列表对象放置容器
    lv_obj_t * open_chat_ol_list_o = lv_obj_create(main_screen_o); //创建在线客户端列表对象放置容器
    lv_obj_set_size(open_chat_ol_list_o, 255, main_screen_h-2);//设置尺寸
    lv_obj_align_to(open_chat_ol_list_o, main_screen_o, LV_ALIGN_TOP_LEFT, 0, 0);  //对齐
    lv_obj_clear_flag(open_chat_ol_list_o, LV_OBJ_FLAG_SCROLLABLE);// 禁用输入框的滚动条
    lv_coord_t ol_list_h = lv_obj_get_height(open_chat_ol_list_o);//获取在线客户端列表高度
    lv_coord_t ol_list_w = lv_obj_get_width(open_chat_ol_list_o);//获取在线客户端列表宽度
    //创建在线客户端列表对象
    open_chat_ol_list = lv_list_create(open_chat_ol_list_o); //创建在线客户端列表对象
    lv_obj_set_size(open_chat_ol_list, ol_list_w, ol_list_h);//设置尺寸
    lv_obj_align_to(open_chat_ol_list, open_chat_ol_list_o, LV_ALIGN_CENTER, 0, 0);  //对齐
    //添加点击列表框隐藏键盘事件回调
    lv_obj_add_event_cb(open_chat_ol_list, oc_hidden_kb_cb, LV_EVENT_CLICKED, NULL); //添加点击列表框隐藏键盘

    //创建openchat主聊天界面容器
    open_chat_main_interface_o = lv_obj_create(main_screen_o);
    oc_main_interface_to_client = open_chat_main_interface_o;
    lv_obj_set_size(open_chat_main_interface_o, main_screen_w - ol_list_w, main_screen_h - 120);//设置尺寸,剩100px给输入框容器
    lv_obj_align_to(open_chat_main_interface_o, main_screen_o, LV_ALIGN_TOP_RIGHT, 0, 0);  //对齐
    lv_coord_t main_interface_h = lv_obj_get_height(open_chat_main_interface_o);//获取主聊天界面高度
    lv_coord_t main_interface_w = lv_obj_get_width(open_chat_main_interface_o);//获取主聊天界面宽度
    // 取消open_chat_main_interface_o的x轴滚动效果
    lv_obj_set_scroll_dir(open_chat_main_interface_o, LV_DIR_VER);
    //添加点击主聊天界面隐藏键盘事件回调
    lv_obj_add_event_cb(open_chat_main_interface_o, oc_hidden_kb_cb, LV_EVENT_CLICKED, NULL); //添加点击主聊天界面隐藏键盘

    //创建输入框和下方按钮放置容器
    open_chat_input_interface_o = lv_obj_create(main_screen_o);
    lv_obj_set_size(open_chat_input_interface_o, main_interface_w, main_screen_h - main_interface_h);//设置尺寸
    lv_obj_align_to(open_chat_input_interface_o, open_chat_main_interface_o, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);  //对齐
    lv_obj_set_style_pad_all(open_chat_input_interface_o, 0, LV_PART_MAIN); //设置内边距为0
    lv_obj_clear_flag(open_chat_input_interface_o, LV_OBJ_FLAG_SCROLLABLE);// 禁用输入框的滚动条
    lv_coord_t input_interface_h = lv_obj_get_height(open_chat_input_interface_o);//获取输入框容器高度
    lv_coord_t input_interface_w = lv_obj_get_width(open_chat_input_interface_o);//获取输入框容器宽度

    //在输入框容器里面创建输入框textarea 创建id = 0
    open_chat_input_ta = tools_create_common_ta(open_chat_input_interface_o, input_interface_w, input_interface_h, "请输入消息");
    lv_obj_align_to(open_chat_input_ta, open_chat_input_interface_o, LV_ALIGN_TOP_MID, 0, 0);  //对齐
    //添加点击输入框显示键盘事件回调
    lv_obj_add_event_cb(open_chat_input_ta, oc_ta_kb_associate_cb, LV_EVENT_FOCUSED, NULL); //添加点击输入框显示键盘事件
    
    //创建发送表情包，文件，发送消息按钮对象
    //创建发送表情包按钮 创建id = 1
    lv_coord_t btn_w = 110, btn_h = 43;//按钮宽度和高度
    open_chat_send_emoti_btn = tools_create_lbed_bt(open_chat_input_interface_o, "(●'◡'●)", btn_w, btn_h, lv_color_hex(0x4CAF50));//创建发送表情包按钮
    lv_obj_add_state(open_chat_send_emoti_btn, LV_STATE_DISABLED);//添加禁用状态
    lv_obj_align_to(open_chat_send_emoti_btn, open_chat_input_interface_o, LV_ALIGN_BOTTOM_LEFT, 5, 1);  //对齐
    //添加点击发送表情包按钮事件回调
    lv_obj_add_event_cb(open_chat_send_emoti_btn, oc_send_emotion_btn_cb, LV_EVENT_CLICKED, NULL); //添加点击发送表情包按钮事件回调
    //创建发送文件按钮 创建id = 2
    open_chat_send_file_btn = tools_create_lbed_bt(open_chat_input_interface_o, "发送文件", btn_w, btn_h, lv_color_hex(0xFF8C00));//创建发送文件按钮
    lv_obj_add_state(open_chat_send_file_btn, LV_STATE_DISABLED);//添加禁用状态
    lv_obj_align_to(open_chat_send_file_btn, open_chat_send_emoti_btn, LV_ALIGN_OUT_RIGHT_MID, 5, 1);  //对齐
    //添加点击发送文件按钮事件回调
    lv_obj_add_event_cb(open_chat_send_file_btn, oc_send_file_btn_cb, LV_EVENT_CLICKED, NULL); //添加点击发送文件按钮事件回调
    //创建发送消息按钮 创建id = 3
    open_chat_send_msg_btn = tools_create_lbed_bt(open_chat_input_interface_o, "发送消息", btn_w, btn_h, lv_color_hex(0x2196F3));//创建发送消息按钮
    lv_obj_add_state(open_chat_send_msg_btn, LV_STATE_DISABLED);//添加禁用状态
    lv_obj_align_to(open_chat_send_msg_btn, open_chat_input_interface_o, LV_ALIGN_BOTTOM_RIGHT, -5, 1);  //对齐
    //添加点击发送消息按钮事件回调
    lv_obj_add_event_cb(open_chat_send_msg_btn, oc_send_msg_btn_cb, LV_EVENT_CLICKED, NULL); //添加点击发送消息按钮事件回调

    //从服务器加载所有在线用户
    // recv_all_online_clients_handler(open_chat_client_sock);
    //将服务器在线用户列表显示到在线用户列表对象
    add_open_chat_ol_client_to_list(open_chat_ol_list, oc_online_client_ll);
    printf("debug: 同步在线用户列表完成\n");
}
//将服务器在线用户列表显示到在线用户列表对象，返回按钮点击回调函数链表
void add_open_chat_ol_client_to_list(lv_obj_t * open_chat_ol_list, struct clientmsg * online_clients){
    //debug
    printf("debug: 同步在线用户列表\n");
    //接收所有在线客户端信息
    //send_get_all_online_clients_cmd(&oc_client_sock, &oc_client_addr);//发送获取所有在线客户端命令
    //清空在线用户列表
    lv_obj_clean(open_chat_ol_list);
    //遍历在线用户链表，将每个用户添加到在线用户列表
    struct clientmsg * current = online_clients->next;
    if(current == NULL){    //等待同步完成
        //sleep(1);
    }
    while(current != NULL){
        //创建在线用户按钮
        char ol_client_info[30] = {0};
        bzero(ol_client_info, sizeof(ol_client_info));
        sprintf(ol_client_info, "%s@%hu", current->ip, current->port);  //将用户IP地址和端口号格式化到ol_client_info

        lv_obj_t * ol_client_btn = lv_list_add_btn(open_chat_ol_list, NULL, ol_client_info);    //添加在线用户信息到在线用户列表
        lv_obj_set_user_data(ol_client_btn, current);  //给按钮添加用户数据，存储用户IP地址和端口号
        lv_obj_add_style(ol_client_btn, def_text_style, 0);   //给按钮的文字添加文字style
        //添加列表框按钮回调
        lv_obj_add_event_cb(ol_client_btn, open_chat_ol_client_btn_cb, LV_EVENT_CLICKED, current);//传入当前用户信息
        current = current->next;
    }
}
//在线用户列表按钮点击回调函数
void open_chat_ol_client_btn_cb(lv_event_t * e){    //在线用户列表按钮点击回调函数
    lv_obj_t * btn = lv_event_get_target(e);    //获取点击的按钮对象
    struct clientmsg * client = (struct clientmsg *)lv_event_get_user_data(e);//获取按钮用户数据，即当前用户信息
    //debug
    printf("debug: 点击了在线用户 %s@%hu\n", client->ip, client->port);
    //将点击的用户显示在主界面的标题
    char ol_client_info[30] = {0};
    bzero(ol_client_info, sizeof(ol_client_info));
    sprintf(ol_client_info, "%s@%hu", client->ip, client->port);  //将用户IP地址和端口号格式化到ol_client_info
    //将用户信息添加到主界面的标题
    //重新调整标题标签背景的宽度和高度，以适应用户信息
    lv_obj_set_size(main_screen_title_bg, 250, 28);
    lv_label_set_text(main_screen_title_lb, ol_client_info);
    //将点击的用户信息存储到当前聊天的客户端对象
    oc_current_chat_client = client;
    //解除发送文件，发送表情，发送消息按钮点击限制
    lv_obj_clear_state(open_chat_send_file_btn, LV_STATE_DISABLED);
    lv_obj_clear_state(open_chat_send_emoti_btn, LV_STATE_DISABLED);
    lv_obj_clear_state(open_chat_send_msg_btn, LV_STATE_DISABLED);
}


//发送消息按钮点击回调函数
void oc_send_msg_btn_cb(lv_event_t * e){
    lv_obj_t * btn = lv_event_get_target(e);    //获取点击的按钮对象
    //获取输入框中的文本
    //发送消息
    oc_send_msg_btn_handler(oc_current_chat_client, &oc_client_addr, &oc_client_sock, oc_online_client_ll,
         open_chat_input_ta, open_chat_main_interface_o);
}
//发送文件按钮点击回调函数
void oc_send_file_btn_cb(lv_event_t * e){
    lv_obj_t * btn = lv_event_get_target(e);    //获取点击的按钮对象
    //发送文件
    oc_send_file_btn_handler(oc_current_chat_client, &oc_client_addr, &oc_client_sock, open_chat_main_interface_o,
         open_chat_input_interface_o);
}
//发送表情按钮点击回调函数
void oc_send_emotion_btn_cb(lv_event_t * e){
    lv_obj_t * btn = lv_event_get_target(e);    //获取点击的按钮对象
    //发送表情
    oc_send_emotion_btn_handler(oc_current_chat_client, &oc_client_addr, &oc_client_sock, open_chat_main_interface_o,
         open_chat_input_interface_o, main_screen_o);
}

