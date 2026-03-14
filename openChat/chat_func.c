#include "chat_func.h"
lv_obj_t * oc_flex_cont = NULL, *oc_main_interface_to_client = NULL;  //旧的消息气泡容器对象指针，用于定位新气泡
//将接收到的消息显示在主屏幕
void oc_display_recv_send_message(const char * message, lv_obj_t * main_interface_o, char* msg_type, char* img_path){
    if(message == NULL || main_interface_o == NULL){
        printf("显示消息失败，消息或主界面对象为空\n");
        return;
    }

    // 创建一个消息气泡容器
    lv_obj_t * bubble_bar_o = lv_obj_create(main_interface_o);    //创建一个消息气泡容器
    lv_obj_set_size(bubble_bar_o, lv_obj_get_width(main_interface_o), LV_SIZE_CONTENT);   //设置气泡容器的大小为与主界面宽度相同，内容高度
    lv_obj_set_style_bg_opa(bubble_bar_o, LV_OPA_TRANSP, LV_PART_MAIN);//设置气泡容器背景透明度为完全透明
    lv_obj_set_style_border_width(bubble_bar_o, 0, LV_PART_MAIN); // 取消气泡的边框
    lv_obj_align_to(bubble_bar_o, main_interface_o, LV_ALIGN_BOTTOM_MID, 0, 0);//将气泡容器对齐到主界面的底部中间
    

    //创建消息气泡子容器
    lv_obj_t * bubble = lv_obj_create(bubble_bar_o);    //创建一个消息气泡容器
    lv_obj_set_size(bubble, LV_SIZE_CONTENT, LV_SIZE_CONTENT);   //设置气泡容器的大小为内容宽度，内容高度
    // lv_obj_set_style_max_width(bubble, lv_pct(50), LV_PART_MAIN);//设置气泡容器最大宽度为父对象的50%
    lv_obj_set_style_radius(bubble, 25, LV_PART_MAIN);//设置气泡容器的圆角半径为20
    lv_obj_set_style_pad_all(bubble, 12, LV_PART_MAIN);//设置气泡容器的所有内边距为10
    lv_obj_set_style_border_width(bubble, 0, LV_PART_MAIN); // 取消气泡的边框
    if(strcmp(msg_type, "recv") == 0){
        lv_obj_set_style_bg_color(bubble, lv_color_hex(0xFFFACD), LV_PART_MAIN);//设置气泡容器的背景颜色为浅黄色
        lv_obj_align(bubble, LV_ALIGN_BOTTOM_LEFT, 0, 0);//将气泡容器对齐到气泡容器的底部左侧
    }
    else if(strcmp(msg_type, "send") == 0){
        lv_obj_set_style_bg_color(bubble, lv_color_hex(0x90EE90), LV_PART_MAIN);//设置气泡容器的背景颜色为浅绿色
        lv_obj_align(bubble, LV_ALIGN_BOTTOM_RIGHT, 0, 0);//将气泡容器对齐到气泡容器的底部右侧
    }
    //创建图片显示子容器
    lv_obj_t * img_cont = NULL;
    if(img_path != NULL){
        img_cont = lv_obj_create(bubble);    //创建一个图片显示子容器
        lv_obj_set_size(img_cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);   //设置图片显示子容器的大小为内容宽度，内容高度
        lv_obj_set_style_bg_opa(img_cont, LV_OPA_TRANSP, LV_PART_MAIN);//设置图片显示子容器背景透明度为完全透明
        lv_obj_set_style_border_width(img_cont, 0, LV_PART_MAIN); // 取消图片显示子容器的边框
        lv_obj_set_style_pad_all(img_cont, 0, LV_PART_MAIN); //设置图片显示子容器的所有内边距为0
        lv_obj_align(img_cont, LV_ALIGN_TOP_LEFT, 0, 0);//将图片显示子容器对齐到气泡容器的中心
        //加载图片到图片显示子容器
        //debug
        printf("debug 显示在界面的图片路径： %s\n", img_path);
        lv_obj_t * img = lv_img_create(img_cont);    //创建一个图片对象
        lv_obj_set_style_bg_opa(img_cont, LV_OPA_TRANSP, LV_PART_MAIN);//设置图片显示子容器背景透明度为完全透明
        char img_path_format[256] = {0};
        bzero(img_path_format, sizeof(img_path_format));
        sprintf(img_path_format, "S:%s", img_path); // 格式化图片路径为 LVGL 要求的格式
        lv_img_set_src(img, img_path_format);    //设置图片对象的图片路径
        lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);   //设置图片对象的大小为内容宽度，内容高度
        lv_obj_set_style_border_width(img, 0, LV_PART_MAIN);    // 取消图片对象的边框
        lv_obj_set_style_pad_all(img, 0, LV_PART_MAIN); //设置图片对象的所有内边距为0
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);//将图片对象对齐到图片显示子容器的中心
    }

    // 在气泡内创建标签用于显示文本，最长显示43123ewrqwerqw123412
    lv_obj_t * message_lb = lv_label_create(bubble);    //创建一个标签对象用于显示消息文本
    lv_obj_add_style(message_lb, def_text_style, 0);   //给组件添加样式,支持中文
    if(strlen(message) > 20){
        lv_obj_set_width(message_lb, lv_obj_get_width(main_interface_o)/2);    //必须固定宽度才能换行显示
    }
    else{
        lv_obj_set_width(message_lb, LV_SIZE_CONTENT);    //必须固定宽度才能换行显示
    }
    // lv_obj_set_style_max_width(message_lb, lv_pct(100), LV_PART_MAIN);    //设置标签最大宽度为气泡的100%
    lv_label_set_long_mode(message_lb, LV_LABEL_LONG_WRAP);    //设置标签的长文本模式为换行显示
    lv_label_set_text(message_lb, message);    //设置标签的文本内容为传入的消息
    lv_obj_align(message_lb, LV_ALIGN_TOP_LEFT, 0, 0);    //将标签对齐到气泡容器的中心
    lv_obj_set_style_text_align(message_lb, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN); // 设置标签文本左对齐
    //将消息lb放在表情包下面
    if(img_cont != NULL){
        lv_obj_align_to(message_lb, img_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0); // 设置标签文本居中对齐
    }
    //设置气泡间的虚拟隔挡，用于分隔不同的气泡
    lv_obj_t * bubble_gap = lv_obj_create(main_interface_o);
    lv_obj_set_size(bubble_gap, lv_obj_get_width(bubble_bar_o), 5);   //设置气泡容器的大小为与主界面宽度相同，内容高度
    lv_obj_set_style_bg_opa(bubble_gap, LV_OPA_TRANSP, LV_PART_MAIN);//设置气泡容器背景透明度为完全透明
    lv_obj_set_style_border_width(bubble_gap, 0, LV_PART_MAIN); // 取消气泡的边框
    lv_obj_align_to(bubble_gap, bubble_bar_o, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);//将隔挡放在气泡容器下面

    lv_obj_scroll_to_y(main_interface_o, LV_COORD_MAX, LV_ANIM_ON); //滚动到新气泡的位置，确保新气泡可见


}
//移动输入框所在容器到键盘上方
void move_input_interface_above_kb(lv_obj_t * input_interface_o, lv_obj_t * kb_o){
    lv_obj_align_to(input_interface_o, kb_o, LV_ALIGN_OUT_TOP_MID, 0, 0);  //对齐
}
//移动输入框所在容器到键盘下方
void move_input_interface_default(lv_obj_t * input_interface_o, lv_obj_t * main_interface_o){
    lv_obj_align_to(input_interface_o, main_interface_o, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);  //对齐
}

//发送消息按钮事件处理函数
void oc_send_msg_btn_handler(struct clientmsg * oc_current_chat_client, struct sockaddr_in * oc_self_client_addr, 
    int *oc_self_client_sock, struct clientmsg * oc_online_clients_ll,  lv_obj_t * oc_input_ta, lv_obj_t * oc_main_interface_o){
    //获取输入框中的文本
    char msg[1024] = {0};
    bzero(msg, sizeof(msg));
    if(oc_input_ta == NULL || !lv_obj_is_valid(oc_input_ta) || oc_main_interface_o == NULL){
        perror("oc_input_ta or oc_main_interface_o is NULL or invalid");
        return;
    }
    if(oc_self_client_addr == NULL || oc_online_clients_ll == NULL){
        printf("oc_self_client_addr or oc_online_clients_ll为NULL\n");
        return;
    }
    strcpy(msg, lv_textarea_get_text(oc_input_ta));
    //debug
    printf("debug: 点击了发送消息按钮\n");
    if(oc_current_chat_client == NULL){ //如果当前聊天的客户端对象为空, 则将消息发给自己
        char source_ip[INET_ADDRSTRLEN];
        oc_current_chat_client = find_client(inet_ntop(AF_INET, &oc_self_client_addr->sin_addr, source_ip, sizeof(source_ip)),
         ntohs(oc_self_client_addr->sin_port), oc_online_clients_ll);
    }
    //将消息发送给当前聊天的客户端
    send_c2c_chat_msg(oc_self_client_sock, oc_self_client_addr, oc_current_chat_client, msg);
    oc_display_recv_send_message(msg, oc_main_interface_o, "send", NULL);//将发送的消息显示在主屏幕
    //清空输入框
    lv_textarea_set_text(oc_input_ta, "");
    //debug
    printf("debug: 发送消息 %s 给 %s@%hu\n", msg, oc_current_chat_client->ip, oc_current_chat_client->port);
}


//发送文件功能函数，由点击发送文件按钮事件处理函数触发
void oc_send_file_btn_handler(struct clientmsg * oc_current_chat_client, struct sockaddr_in * oc_self_client_addr,
     int * oc_self_client_sock, lv_obj_t * oc_main_interface_o, lv_obj_t * oc_input_interface_o){
         
        if(oc_input_interface_o == NULL || !lv_obj_is_valid(oc_input_interface_o)){
            printf("发送文件失败，输入框所在容器对象为空\n");
            return;
        }
    lv_obj_t * oc_input_ta = lv_obj_get_child(oc_input_interface_o, 0);
    lv_obj_t * oc_send_msg_btn = lv_obj_get_child(oc_input_interface_o, 3);
    
    if(oc_input_ta == NULL || !lv_obj_is_valid(oc_input_ta) || oc_send_msg_btn == NULL || !lv_obj_is_valid(oc_send_msg_btn)){
        printf("发送文件失败，输入框或发送消息按钮为空\n");
        return;
    }
    //禁用并隐藏发送消息按钮
    lv_obj_add_state(oc_send_msg_btn, LV_STATE_DISABLED);
    lv_obj_add_flag(oc_send_msg_btn, LV_OBJ_FLAG_HIDDEN);
    //新建发出文件按钮 创建id = 4
    lv_obj_t * oc_send_to_file_btn = tools_create_lbed_bt(oc_input_interface_o, "确认发送", 110, 43, lv_color_hex(0xFF8C00));//创建发送文件按钮
    //将发出文件按钮覆盖发送消息按钮
    lv_obj_align_to(oc_send_to_file_btn, oc_send_msg_btn, LV_ALIGN_CENTER, 0, 0);
    
    //将输入框ta提示文字设置为"请输入文件名"
    lv_textarea_set_placeholder_text(oc_input_ta, "请输入文件名");  //设置提示词

    static struct file_send_info file_send_info = {0};
    bzero(&file_send_info, sizeof(file_send_info));

    //传递参数给点击发出文件按钮事件处理函数
    // strcpy(file_send_info.file_name, "000");//
    file_send_info.oc_main_interface_o = oc_main_interface_o;
    file_send_info.oc_input_interface_o = oc_input_interface_o;
    file_send_info.oc_current_chat_client = oc_current_chat_client;
    file_send_info.oc_self_client_addr = oc_self_client_addr;
    file_send_info.oc_self_client_sock = oc_self_client_sock;
    //添加点击发出文件按钮事件回调
    lv_obj_add_event_cb(oc_send_to_file_btn, oc_send_to_file_btn_cb, LV_EVENT_CLICKED, &file_send_info); //添加点击发出文件按钮事件回调
}

//点击发出文件按钮事件处理函数
void oc_send_to_file_btn_cb(lv_event_t * e){
    lv_obj_t * btn = lv_event_get_target(e);
    struct file_send_info * file_send_info = (struct file_send_info *)lv_event_get_user_data(e);//获取点击发出文件按钮事件处理函数中传递的文件路径
    if(btn == NULL || !lv_obj_is_valid(btn) || file_send_info == NULL){
        printf("点击发出文件按钮事件处理函数失败，按钮或文件发送信息为空\n");
        return;
    }
    //获取oc_input_interface_o中的输入框和发送消息按钮  id = 0，发送消息按钮 id = 3， 发出文件按钮 id = 4
    lv_obj_t * oc_input_ta = lv_obj_get_child(file_send_info->oc_input_interface_o, 0);
    lv_obj_t * oc_send_msg_btn = lv_obj_get_child(file_send_info->oc_input_interface_o, 3);
    lv_obj_t * oc_send_to_file_btn = lv_obj_get_child(file_send_info->oc_input_interface_o, 4);

    if(oc_input_ta == NULL || !lv_obj_is_valid(oc_input_ta) || oc_send_msg_btn == NULL || !lv_obj_is_valid(oc_send_msg_btn) ||
     oc_send_to_file_btn == NULL || !lv_obj_is_valid(oc_send_to_file_btn)){
        printf("点击发出文件按钮事件处理函数失败，输入框或发送消息按钮为空\n");
        return;
    }
    char file_name[64] = {0};
    bzero(file_name, sizeof(file_name));
    strcpy(file_name, lv_textarea_get_text(oc_input_ta));//获取输入框中的文件名
    if(strlen(file_name) <= 0){
        printf("点击发出文件按钮事件处理函数失败，文件名为空\n");
        return;
    }
    // 发送文件
    // 检查文件名是否包含非法字符
    if(strpbrk(file_name, "/\\?%*:|\"<>") != NULL){
        printf("发送文件失败，文件名包含非法字符\n");
        return;
    }
    char abs_file_path[256] = {0}, current_dir[256] = {0};
    bzero(abs_file_path, sizeof(abs_file_path));
    bzero(current_dir, sizeof(current_dir));
    getcwd(current_dir, sizeof(current_dir));   //获取当前工作目录
    sprintf(abs_file_path, "%s/%s", current_dir, file_name);    //拼接绝对路径
    printf("debug 发送的文件路径： %s\n", abs_file_path);
    // 检查文件是否存在
    if(access(abs_file_path, F_OK) == -1){
        printf("发送文件失败，文件不存在\n");
        return;
    }
    send_file_cmd_content(file_send_info->oc_self_client_sock, file_send_info->oc_self_client_addr,
         file_send_info->oc_current_chat_client, abs_file_path);
    //debug
    printf("发送本地文件 %s文件%s 到 %s完成\n", abs_file_path, file_name, file_send_info->oc_current_chat_client->ip);
    // 启用并显示发送消息按钮
    if(oc_send_msg_btn != NULL && lv_obj_is_valid(oc_send_msg_btn)){
        lv_obj_clear_state(oc_send_msg_btn, LV_STATE_DISABLED);
        lv_obj_clear_flag(oc_send_msg_btn, LV_OBJ_FLAG_HIDDEN);
    }
    // 删除发出文件按钮
    if(oc_send_to_file_btn != NULL && lv_obj_is_valid(oc_send_to_file_btn)){
        lv_obj_del(oc_send_to_file_btn);
    }
    //将发送成功的提示信息显示在聊天界面
    char send_success_msg[128] = {0};
    bzero(send_success_msg, sizeof(send_success_msg));
    sprintf(send_success_msg, "发送文件%s成功！", file_name);
    printf("debug 发送成功的提示信息： %s\n", send_success_msg);
    if(file_send_info != NULL && lv_obj_is_valid(file_send_info->oc_main_interface_o)){
        oc_display_recv_send_message(send_success_msg, file_send_info->oc_main_interface_o, "send", NULL);
    }
}


////发送表情包功能函数，由点击(●'◡'●)按钮事件处理函数触发
//点击(●'◡'●)按钮，在输入框父对象容器上面弹出flex，显示所有表情包图片
//选中的列表框按钮，将按钮代表的表情包文件名添加到输入框中
//点击确认发送按钮，将输入框中的表情包文件发送给对方
void oc_send_emotion_btn_handler(struct clientmsg * oc_current_chat_client, struct sockaddr_in * oc_self_client_addr,
    int * oc_self_client_sock, lv_obj_t * oc_main_interface_o, lv_obj_t * oc_input_interface_o, lv_obj_t * oc_main_screen_o){
        //判断输入框所在容器对象是否为空
    if(oc_input_interface_o == NULL || !lv_obj_is_valid(oc_input_interface_o)){
        printf("发送表情包失败，输入框所在容器对象为空\n");
        return;
    }
    //获取oc_input_interface_o中的输入框  id = 0，发送消息按钮 id = 3， 发出文件按钮 id = 4
   lv_obj_t * oc_input_ta = lv_obj_get_child(oc_input_interface_o, 0);
   lv_obj_t * oc_send_file_btn = lv_obj_get_child(oc_input_interface_o, 2);
   lv_obj_t * oc_send_msg_btn = lv_obj_get_child(oc_input_interface_o, 3);
   //判断输入框和发送消息按钮是否为空
   if(oc_input_ta == NULL || !lv_obj_is_valid(oc_input_ta) || oc_send_msg_btn == NULL || !lv_obj_is_valid(oc_send_msg_btn) ||
     oc_send_file_btn == NULL || !lv_obj_is_valid(oc_send_file_btn)){
       printf("发送表情包失败，输入框或发送消息按钮或发出文件按钮为空\n");
       return;
   }
   //禁用并隐藏发送消息按钮
   lv_obj_add_state(oc_input_ta, LV_STATE_DISABLED); //禁用输入框，阻止点击弹出输入法
   lv_obj_add_state(oc_send_file_btn, LV_STATE_DISABLED); //禁用发出文件按钮，阻止点击发出文件
   lv_obj_add_state(oc_send_msg_btn, LV_STATE_DISABLED); //禁用发送消息按钮，阻止点击发送消息

   //遍历表情包所在文件夹，将所有表情包名字和图片添加到flex容器
   char emotion_path[128] = {0}, current_path[64] = {0};
   bzero(current_path, sizeof(current_path));
   getcwd(current_path, sizeof(current_path)); //获取当前路径
   bzero(emotion_path, sizeof(emotion_path));
   sprintf(emotion_path, "%s/emotion", current_path); //表情包文件夹路径
   //新建一个flex容器，显示所有表情包图片
   oc_flex_cont = lv_obj_create(oc_main_screen_o);
   lv_obj_set_size(oc_flex_cont, 400, 250);      // 设置容器大小
   lv_obj_align_to(oc_flex_cont, oc_input_interface_o, LV_ALIGN_OUT_TOP_MID, 0, 0); //将oc_flex_cont的顶部外边对齐到oc_input_interface_o的顶部外边
   //发送表情包的一些参数传递给添加表情包到flex容器的函数
   static struct file_send_info file_send_info = {0};
   bzero(&file_send_info, sizeof(file_send_info));
   file_send_info.oc_main_interface_o = oc_main_interface_o;    //主界面对象
   file_send_info.oc_input_interface_o = oc_input_interface_o;  //输入框所在容器对象
   file_send_info.oc_current_chat_client = oc_current_chat_client;  //当前聊天对象
   file_send_info.oc_self_client_addr = oc_self_client_addr;        //客户端地址
   file_send_info.oc_self_client_sock = oc_self_client_sock;        //客户端套接字
   //将所有表情包图片添加到flex容器
   add_emotion_to_preview_flex_cont(oc_flex_cont, emotion_path, &file_send_info); //添加表情包到flex容器

}

//遍历表情包所在文件夹，将所有表情包图片添加到列表框的预览区域
void add_emotion_to_preview_flex_cont(lv_obj_t * oc_flex_cont, char * emotion_dir, struct file_send_info * file_send_info){
    DIR * dir;  //打开表情包文件夹
    struct dirent * ent;    //目录项指针预览
    if((dir = opendir(emotion_dir)) == NULL){   //打开表情包文件夹失败
        printf("打开表情包文件夹失败\n");
        return;
    }
    //传入的容器设置为Flex布局
    lv_obj_set_layout(oc_flex_cont, LV_LAYOUT_FLEX);          // 启用Flex布局
    lv_obj_set_flex_flow(oc_flex_cont, LV_FLEX_FLOW_ROW_WRAP); // 横向排列 + 排满换行
    // 设置所有方向的内边距（容器与子对象之间的距离）
    lv_obj_set_style_pad_all(oc_flex_cont, 0, LV_PART_MAIN);

    // 设置主轴方向的间距（子对象之间的距离）
    // lv_obj_set_style_flex_gap(oc_flex_cont, 0, LV_PART_MAIN);
  
    // 遍历表情包文件夹中的所有文件
    while((ent = readdir(dir)) != NULL){    //遍历表情包文件夹中的所有文件
        if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){    //跳过当前目录和上一级目录
            continue;
        }
        char emotion_path[128] = {0};   //表情包文件路径
        bzero(emotion_path, sizeof(emotion_path));  //清空字符串
        sprintf(emotion_path, "%s/%s", emotion_dir, ent->d_name);   //将表情包文件夹路径和文件名拼接起来
        printf("debug：表情包文件路径：%s\n",emotion_path);
        // 创建图片预览子对象
        lv_obj_t * img_preview_o = lv_obj_create(oc_flex_cont); //放置图片的容器
        lv_obj_set_size(img_preview_o, LV_SIZE_CONTENT, LV_SIZE_CONTENT);      // 设置容器大小
        // 创建一个标签，保存图片路径
        lv_obj_t * img_preview_label = lv_label_create(img_preview_o);
        lv_obj_set_size(img_preview_label, 1, 1);      // 设置标签大小
        lv_label_set_text(img_preview_label, emotion_path); // 设置标签文本为图片路径
        lv_obj_add_flag(img_preview_label, LV_OBJ_FLAG_HIDDEN); // 隐藏图片路径标签

        lv_obj_t * img_preview = lv_img_create(img_preview_o);  //将图片预览子对象添加到容器中
        char img_path[128] = {0};
        bzero(img_path, sizeof(img_path));
        sprintf(img_path, "S:%s", emotion_path);   //格式化图片路径
        lv_img_set_src(img_preview, img_path);  // 设置图片源
        lv_obj_set_size(img_preview, LV_SIZE_CONTENT, LV_SIZE_CONTENT);      // 设置预览图大小
        // 缩放图片到 100×100
        // lv_img_set_zoom(img_preview, LV_IMG_ZOOM_NONE * 100 / lv_img_get_zoom(img_preview));
        // lv_obj_set_size(img_preview, 100, 100);
        lv_obj_set_style_radius(img_preview, 20, LV_PART_MAIN);  // 圆角
        lv_obj_set_style_clip_corner(img_preview, true, LV_PART_MAIN);  // 裁剪圆角
        lv_obj_set_style_border_width(img_preview, 0, LV_PART_MAIN);
        // lv_obj_set_style_border_color(img_preview, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
        lv_obj_set_style_pad_all(img_preview, 0, LV_PART_MAIN);  // 内边距
        // lv_obj_scroll_to_y(oc_flex_cont, LV_COORD_MAX, LV_ANIM_ON); //滚动到新气泡的位置，确保新气泡可见
        // 添加点击事件，点击后直接发送该图片文件
        lv_obj_add_event_cb(img_preview_o, oc_send_emotion_click_cb, LV_EVENT_CLICKED, file_send_info);
    }
    closedir(dir);
}

//点击表情包图片回调函数，发送这个表情包给当前聊天对象
void oc_send_emotion_click_cb(lv_event_t * e){
    lv_obj_t * img_preview_o = lv_event_get_target(e);
    if(img_preview_o == NULL || !lv_obj_is_valid(img_preview_o)){
        printf("点击表情包图片预览子对象事件处理函数失败，图片预览子对象为空\n");
        return;
    }
    // // 获取图片路径标签
    // lv_obj_t * img_preview_o = lv_obj_get_parent(img_preview_o);
    // if(img_preview_o == NULL || !lv_obj_is_valid(img_preview_o)){
    //     printf("点击表情包图片预览子对象事件处理函数失败，图片预览子对象为空\n");
    //     return;
    // }
    lv_obj_t * img_preview_label = lv_obj_get_child(img_preview_o, 0);
    if(img_preview_label == NULL || !lv_obj_is_valid(img_preview_label)){
        printf("点击表情包图片预览子对象事件处理函数失败，图片路径标签为空\n");
        return;
    }
    char * emotion_img_path = (char *)lv_label_get_text(img_preview_label);
    if(emotion_img_path == NULL){
        printf("点击表情包图片预览子对象事件处理函数失败，图片路径为空\n");
        return;
    }
    //debug
    printf("点击表情包图片预览子对象事件处理函数，图片路径为：%s\n", emotion_img_path);
    //获取发送表情包文件的回调函数触发时传入的参数
    struct file_send_info * file_send_info = (struct file_send_info *)lv_event_get_user_data(e);
    if(file_send_info == NULL){
        printf("点击表情包图片预览子对象事件处理函数失败，文件发送信息为空\n");
        return;
    }
    //获取发送表情包文件的客户端套接字和地址
    int *oc_self_client_sock = file_send_info->oc_self_client_sock;
    struct sockaddr_in *oc_self_client_addr = file_send_info->oc_self_client_addr;
    struct clientmsg * current_chat_client = file_send_info->oc_current_chat_client;
    if(oc_self_client_addr == NULL || current_chat_client == NULL){ 
        printf("点击表情包图片预览子对象事件处理函数失败，当前聊天对象或客户端地址为空\n");
        return;
    }
    //将图片发送给当前聊天对象
    if(access(emotion_img_path, F_OK) == -1){
        printf("点击表情包图片预览子对象事件处理函数失败，图片路径不存在\n");
        return;
    }
    send_file_cmd_content( oc_self_client_sock, oc_self_client_addr, current_chat_client, emotion_img_path);
    //debug
    printf("发送成功本地文件 %s 到 %s\n", emotion_img_path, current_chat_client->ip);
    //解除发送消息按钮的禁用状态和隐藏标志，使按钮可点击
    lv_obj_t * oc_input_interface_o = file_send_info->oc_input_interface_o;  //获取输入接口对象
    lv_obj_t * oc_input_ta = lv_obj_get_child(oc_input_interface_o, 0);  //获取输入框对象
    lv_obj_t * oc_send_file_btn = lv_obj_get_child(oc_input_interface_o, 2);  //获取发送文件按钮对象
    lv_obj_t * oc_send_msg_btn = lv_obj_get_child(oc_input_interface_o, 3);  //获取发送消息按钮对象

    if(oc_input_interface_o == NULL || !lv_obj_is_valid(oc_input_interface_o) ||
         oc_send_msg_btn == NULL || !lv_obj_is_valid(oc_send_msg_btn) ||
         oc_input_ta == NULL || !lv_obj_is_valid(oc_input_ta) ||
         oc_send_file_btn == NULL || !lv_obj_is_valid(oc_send_file_btn)){
        printf("无法解除发送消息按钮的禁用状态和隐藏标志，发送消息按钮指针为空\n");
        return;
    }
    else{   //解除发送消息按钮的禁用状态和隐藏标志，使按钮可点击
        lv_obj_clear_state(oc_send_msg_btn, LV_STATE_DISABLED); //解除禁用状态
        lv_obj_clear_state(oc_input_ta, LV_STATE_DISABLED);//解除禁用状态
        lv_obj_clear_state(oc_send_file_btn, LV_STATE_DISABLED);//解除禁用状态
    }

    //将发送成功的提示信息显示在聊天界面
    char send_success_msg[128] = {0};
    bzero(send_success_msg, sizeof(send_success_msg));
    sprintf(send_success_msg, "发送表情%s成功！", emotion_img_path);
    printf("debug 发送成功的提示信息： %s\n", send_success_msg);
    if(file_send_info != NULL && lv_obj_is_valid(file_send_info->oc_main_interface_o)){
    sprintf(send_success_msg, "发送表情%s成功！", emotion_img_path);
        oc_display_recv_send_message(send_success_msg, file_send_info->oc_main_interface_o, "send",emotion_img_path);
    }

    lv_obj_del(oc_flex_cont);   //删除表情包图片预览容器
}

