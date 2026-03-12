#ifndef _TEST_1_H
#define _TEST_1_H


extern lv_style_t pinyin_plugin_font_style;   //

void bt1_cb(lv_event_t * e);
void mydemo();
void demo_pic();
void demo_pic_2();
void demo_label();
void switch1_cb(lv_event_t *e);
void demo_switch();
void demo_window();
void checkbox_cb(lv_event_t *e);
void demo_checkbox();
void demo_pic_on_btn(char * pic_path);
void demo_freetype_disp(char * cn_s);
int led_click_lighting(char* led_file, int s_flag);
btn_switch_cb(lv_event_t * e);
void demo_btn_switch_led();

void create_font_style(lv_style_t *style, char* font_path, int font_size);
void timer_event_cb(lv_timer_t * e);
void demo_timer();
void demo_lists();
void btn_cb_list(lv_event_t *e);
void demo_matrix_btn();
void btnmatrix1_cb(lv_event_t *e);
void bt_cb(lv_event_t *e);
void demo_text_area_kb();
void add_pinyin_plugin(lv_obj_t * kb);
void demo_tabview();
void demo_msgbox();
void demo_type_design();
#endif