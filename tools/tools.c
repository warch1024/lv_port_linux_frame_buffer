#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include "lvgl/src/extra/others/ime/lv_ime_pinyin.h"
#include "tools/tools.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <strings.h>


lv_obj_t * tools_cn_kb;

void tools_create_font_style(lv_style_t *style, char* font_path, int font_size){
    /*Create a font*/ //创建字体对象
    static lv_ft_info_t info;   //生命周期增长,作用域不变
    bzero(&info ,sizeof(info));
    // lv_ft_info_t * info = (lv_ft_info_t*)malloc(sizeof(lv_ft_info_t));  //info会在后续使用
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = font_path;
    info.weight = font_size;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }
    /*Create style with the new font*/// 创建样式对象
    //static lv_style_t style;    //定义样式对象
    lv_style_init(style);  //初始化样式
    lv_style_set_text_font(style, info.font);
    lv_style_set_text_align(style, LV_TEXT_ALIGN_CENTER);  //居中对齐（左，右，居中）
}
static void add_pinyin_plugin(lv_obj_t * kb, lv_obj_t * obj){  //给kb添加中文支持, 返回拼音输入法插件对象
      
    static lv_style_t pinyin_plugin_font_style;   //拼音输入法候选字的style
    tools_create_font_style(&pinyin_plugin_font_style,"/fonts/MSYH.TTC", 20);
    lv_obj_t * pinyin_ime = lv_ime_pinyin_create(obj); //创建拼音输入法插件
    lv_obj_set_size(pinyin_ime, 1, 1);  //设置大小避免遮蔽
    lv_obj_set_pos(pinyin_ime, 0, 0);   //设置位置避免遮蔽
    lv_obj_add_style(pinyin_ime, &pinyin_plugin_font_style, 0); //输入法候选字正常显示中文
    lv_ime_pinyin_set_mode(pinyin_ime, LV_IME_PINYIN_MODE_K26); //设置默认模式
    lv_obj_t * cand_panel = lv_ime_pinyin_get_cand_panel(pinyin_ime);   //获取拼音候选栏对象

    lv_obj_set_width(cand_panel, 500); // 绑定后续按字宽度到键盘
    lv_ime_pinyin_set_keyboard(pinyin_ime, kb); //将拼音插件绑定到键盘
    /* 如果使用自定义字典
        则在lv_config.h将LV_IME_PINYIN_USE_DEFAULT_DICT宏置0
        使用lv_ime_pinyin_set_dict()设置自定义字典
        使用lv_ime_pinyin_set_mode()设置输入模式
    */
}
lv_obj_t * tools_create_pinyin_ime(lv_obj_t * obj, int weight, int height){ //将键盘放在obj上
    tools_cn_kb = lv_keyboard_create(obj);   //屏幕上添加键盘
    lv_obj_set_size(tools_cn_kb,weight,height);
    lv_obj_set_pos(tools_cn_kb,0,0);     //位置放在0,0否则偏移很大
    //lv_obj_align_to(cand_panel, kb,LV_ALIGN_BOTTOM_MID, 0, -200); // 对齐到键盘
    static lv_obj_t  * pinyin_ime;  //插件必须有全局生命周期
    add_pinyin_plugin(tools_cn_kb, obj);
    lv_obj_add_flag(tools_cn_kb, LV_OBJ_FLAG_HIDDEN);//隐藏键盘
    return tools_cn_kb;
}