#ifndef _LOGIN_SCREEN_H
#define _LOGIN_SCREEN_H 

#include "figure_vending_machine/main_screen.h"
#include "figure_vending_machine/user_info_mangm.h"
#include "tools/tools.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

void hidden_kb_cb(lv_event_t * e);

void ta_kb_associate_cb(lv_event_t * e);
void login_btn_cb(lv_event_t *e);


void login_screen();
void login_window();
void logup_btn_cb(lv_event_t * e);
void logup_window();
void sm_logup_btn_cb(lv_event_t * e);

void bk_logup_btn_cb(lv_event_t * e);
void save_logup_info();



#endif