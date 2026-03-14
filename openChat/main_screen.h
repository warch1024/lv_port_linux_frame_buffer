#ifndef _MAIN_SCREEN_H
#define _MAIN_SCREEN_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"

#include "open_chat_client.h"  //open chat client头文件
// #include "figure_vending_machine/goods_mangm.h"

#include"openChat/screen_objs.h"  //标准库



extern lv_obj_t * open_chat_main_interface_o;//聊天界面的主容器对象
void main_screen_init();
void main_screen();
/* 创建商品卡片 
@param parent_o 父对象
@param item_title 商品标题
@param price 商品价格
@param bg_pic_path 商品背景图片路径
@return 商品卡片对象
*/
////////
void load_open_chat_main_interface(lv_obj_t * main_screen_o);
void add_open_chat_ol_client_to_list(lv_obj_t * open_chat_ol_list, struct clientmsg * online_clients);
void open_chat_ol_client_btn_cb(lv_event_t * e);
//发送消息按钮点击回调函数
void oc_send_msg_btn_cb(lv_event_t * e);
//发送文件按钮点击回调函数
void oc_send_file_btn_cb(lv_event_t * e);
//发送表情按钮点击回调函数
void oc_send_emotion_btn_cb(lv_event_t * e);

#endif