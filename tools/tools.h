#ifndef _TOOLS_H
#define _TOOLS_H


extern lv_obj_t * tools_cn_kb, *tools_cand_pannel;

/*创建字体样式函数
参数：
    style：样式指针
    font_path:字体存储路径，指定使用的字体的存储路径
    font_size:字体大小
*/
void tools_create_font_style(lv_style_t *style, char* font_path, int font_size);

/*内部-输入法添加拼音插件
参数：
    kb：要添加插件的键盘
    parent_obj:插件放置对象
    kb_width: 键盘宽度，候选栏绑定此宽度
*/
static lv_obj_t * add_pinyin_plugin(lv_obj_t * kb, lv_obj_t * parent_obj, int kb_width);

/*创建拼音输入法
参数：
    parent_obj：键盘父对象
    weight:键盘宽度
    height: 键盘高度
*/
lv_obj_t * tools_create_pinyin_ime(lv_obj_t * parent_obj, int weight, int height);

/*显示拼音键盘
参数：
    kb：要显示的键盘
    cp: 要显示的候选栏
*/
void tools_show_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp);

/*隐藏拼音键盘
参数：
    kb：要隐藏的键盘
    cp: 要隐藏的候选栏
*/
void tools_hidden_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp);

#endif