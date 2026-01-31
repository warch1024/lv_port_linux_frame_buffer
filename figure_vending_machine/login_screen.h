#ifndef _LOGIN_SCREEN_H
#define _LOGIN_SCREEN_H 



void hidden_kb_cb(lv_event_t * e);

void login_btn_cb(lv_event_t *e);
void ta_kb_associate_cb(lv_event_t * e);

/*创建登录窗口的输入框，并返回输入框对象，添加中文支持，限定一行，限定8长度
* @param parent_o 父对象
* @param width 宽度
* @param height 高度
* @param pht 提示
*/
lv_obj_t* create_login_ta(lv_obj_t* parent_o, int width, int height, char* pht);

/*创建登录窗口的按钮标签，并返回按钮对象，设置内部对齐，支持中文
* @param parent_o 按钮对象
* @param width 宽度
* @param height 高度
* @param text 按钮文字
*/
lv_obj_t* create_login_bt_lb(lv_obj_t* parent_o,char* text);
void login_window();
void login_screen();
void sm_logup_btn_cb(lv_event_t * e);
void back_to_login_screen();
void logup_window();
void logup_btn_cb(lv_event_t * e);
void bk_logup_btn_cb(lv_event_t * e);




#endif