#ifndef _LOGIN_SCREEN_H
#define _LOGIN_SCREEN_H 

#include "figure_vending_machine/main_screen.h"
#include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"

#include "figure_vending_machine/goods_mangm_con.h"

#include"figure_vending_machine/screen_objs.h"  //标准库
/*初始化登陆屏幕前置组件
*/
void login_screen_base_wigdet_init(lv_obj_t * screen);

void hidden_kb_cb(lv_event_t * e);

void ta_kb_associate_cb(lv_event_t * e);
void login_btn_cb(lv_event_t *e);

void add_switch_full_screen_theme_checkbox(lv_obj_t * full_screen_o);

void login_screen();
void login_window();
void logup_btn_cb(lv_event_t * e);
void logup_window();
void sm_logup_btn_cb(lv_event_t * e);

void bk_logup_btn_cb(lv_event_t * e);
void save_logup_info();
void load_def_login_theme(lv_obj_t * screen_o, char * def_theme_path);
void admin_management();

#endif