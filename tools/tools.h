#ifndef _TOOLS_H
#define _TOOLS_H


extern lv_obj_t * tools_cn_kb;

void tools_create_font_style(lv_style_t *style, char* font_path, int font_size);

static void add_pinyin_plugin(lv_obj_t * kb);

lv_obj_t * tools_create_pinyin_ime(lv_obj_t * scr_act, int weight, int height);

#endif