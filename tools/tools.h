#ifndef _TOOLS_H
#define _TOOLS_H


extern lv_obj_t * tools_cn_kb, *tools_cand_pannel;

void tools_create_font_style(lv_style_t *style, char* font_path, int font_size);

static lv_obj_t * add_pinyin_plugin(lv_obj_t * kb, lv_obj_t * parent_obj, int kb_width);
lv_obj_t * tools_create_pinyin_ime(lv_obj_t * obj, int weight, int height);
void tools_show_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp);
void tools_hidden_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp);

#endif