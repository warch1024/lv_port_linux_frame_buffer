#ifndef __CHAT_FUNC_H__
#define __CHAT_FUNC_H__

// #include "tools/tools.h"
// #include"figure_vending_machine/checkout.h" 
#include "open_chat_client.h"  //open chat client头文件
// #include "figure_vending_machine/main_screen.h"

#include"openChat/screen_objs.h"  //标准库


struct file_send_info{
    char file_name[64];
    lv_obj_t * oc_main_interface_o;
    lv_obj_t * oc_input_interface_o;
    struct clientmsg * oc_current_chat_client;
    struct sockaddr_in * oc_self_client_addr;
    int * oc_self_client_sock;
};
extern lv_obj_t * oc_main_interface_to_client;  //指向主聊天界面容器对象指针，用于定位新气泡

void oc_display_recv_send_message(const char * message, lv_obj_t * main_interface_o, char* msg_type, char* img_path);//将接收到的消息显示在主屏幕
void move_input_interface_above_kb(lv_obj_t * input_interface_o, lv_obj_t * kb_o);//将输入框所在容器移动到键盘上方
void move_input_interface_default(lv_obj_t * input_interface_o, lv_obj_t * main_interface_o);//将输入框所在容器移动到默认位置
void oc_send_msg_btn_handler(struct clientmsg * oc_current_chat_client, struct sockaddr_in * oc_self_client_addr, 
    int *oc_self_client_sock, struct clientmsg * oc_online_clients_ll,  lv_obj_t * oc_input_ta, lv_obj_t * oc_main_interface_o);
//发送文件按钮点击处理函数
void oc_send_file_btn_handler(struct clientmsg * oc_current_chat_client, struct sockaddr_in * oc_self_client_addr,
    int * oc_self_client_sock, lv_obj_t * oc_main_interface_o, lv_obj_t * oc_input_interface_o);

void oc_send_to_file_btn_cb(lv_event_t * e);
//发送表情包按钮点击处理函数
void oc_send_emotion_btn_handler(struct clientmsg * oc_current_chat_client, struct sockaddr_in * oc_self_client_addr,
    int * oc_self_client_sock, lv_obj_t * oc_main_interface_o, lv_obj_t * oc_input_interface_o, lv_obj_t * oc_main_screen_o);
//遍历表情包所在文件夹，将所有表情包图片添加到列表框的预览区域
void add_emotion_to_preview_flex_cont(lv_obj_t * oc_flex_cont, char * emotion_dir, struct file_send_info * file_send_info);
void oc_send_emotion_click_cb(lv_event_t * e);
#endif
