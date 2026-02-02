#include "tools/tools.h"


//lv_obj_t * tools_cn_kb = NULL, *tools_cand_pannel = NULL;



void tools_create_font_style(lv_style_t *style, char* font_path, int font_size){    //传入style对象指针
    /*Create a font*/ //创建字体对象
    static lv_ft_info_t info;   //生命周期增长,作用域不变
    bzero(&info ,sizeof(info));
    // lv_ft_info_t * info = (lv_ft_info_t*)malloc(sizeof(lv_ft_info_t));  //info会在后续使用
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = font_path;
    info.weight = font_size;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {   //初始化字体
        LV_LOG_ERROR("create failed.");
    }
    /*Create style with the new font*/// 创建样式对象
    //static lv_style_t style;    //定义样式对象
    lv_style_init(style);  //初始化样式
    lv_style_set_text_font(style, info.font);   //设置样式
    lv_style_set_text_align(style, LV_TEXT_ALIGN_CENTER);  //居中对齐（左，右，居中）
}
static lv_obj_t * add_pinyin_plugin(lv_obj_t * kb, lv_obj_t * parent_obj, int kb_width){  //给kb添加中文支持, 返回拼音输入法插件对象
      
    //static lv_style_t pinyin_plugin_font_style;   //拼音输入法候选字的style
    //tools_create_font_style(&pinyin_plugin_font_style,"/fonts/MSYH.TTC", 20);
    
    lv_obj_t * pinyin_ime = lv_ime_pinyin_create(parent_obj); //创建拼音输入法插件
    lv_obj_set_size(pinyin_ime, 1, 1);  //设置大小避免遮蔽
    lv_obj_set_pos(pinyin_ime, 0, 0);   //设置位置避免遮蔽
    lv_obj_add_style(pinyin_ime, def_text_style, 0); //输入法候选字正常显示中文
    lv_ime_pinyin_set_mode(pinyin_ime, LV_IME_PINYIN_MODE_K26); //设置默认模式
    lv_obj_t * cand_pannel = lv_ime_pinyin_get_cand_panel(pinyin_ime);   //获取拼音候选栏对象

    lv_obj_set_width(cand_pannel, kb_width); // 绑定后续按字宽度到键盘
    lv_ime_pinyin_set_keyboard(pinyin_ime, kb); //将拼音插件绑定到键盘
    /* 如果使用自定义字典
        则在lv_config.h将LV_IME_PINYIN_USE_DEFAULT_DICT宏置0
        使用lv_ime_pinyin_set_dict()设置自定义字典
        使用lv_ime_pinyin_set_mode()设置输入模式
    */
    return cand_pannel;   // 返回候选栏
}
cn_kb_cp_pair tools_create_pinyin_ime(lv_obj_t * parent_obj, int weight, int height){ //将键盘放在obj上
    lv_obj_t * cn_kb = lv_keyboard_create(parent_obj);                                // 屏幕上添加键盘
    lv_obj_set_size(cn_kb, weight, height);
    lv_obj_set_pos(cn_kb, 0, 0); // 位置放在0,0否则偏移很大
    // 设置候选栏插件
    lv_obj_t * cn_kb_cp = add_pinyin_plugin(cn_kb, parent_obj, weight); // 窗口添加拼音插件
    // 隐藏键盘
    if(cn_kb && cn_kb_cp) {

        tools_hidden_pinyin_kb(cn_kb, cn_kb_cp);
    }
    cn_kb_cp_pair pair = {.cn_kb = cn_kb, .cn_kb_cp = cn_kb_cp};
    return pair;
}

void tools_hidden_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp){
    if(kb && cp){
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);//隐藏键盘
        lv_obj_add_flag(cp, LV_OBJ_FLAG_HIDDEN);//隐藏键盘
    }
}
void tools_show_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp){
    if(kb && cp){
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);//隐藏键盘
        lv_obj_clear_flag(cp, LV_OBJ_FLAG_HIDDEN);//隐藏键盘
    }
}

lv_obj_t* tools_create_login_ta(lv_obj_t* parent_o, int width, int height, char* pht){
    lv_obj_t* ta_obj = lv_textarea_create(parent_o);    //添加输入框
    lv_obj_set_size(ta_obj, width, height);  //设置尺寸
    lv_textarea_set_placeholder_text(ta_obj, pht);  //设置提示词
    lv_obj_add_style(ta_obj, def_text_style,0);     //中文支持
    lv_textarea_set_one_line(ta_obj,true);  //限定在一行
    lv_textarea_set_max_length(ta_obj ,8);  //限制用户名长度
    return ta_obj;
}
lv_obj_t* tools_create_login_bt_lb(lv_obj_t* parent_o,char* text){ //给登录窗口按钮添加标签
    lv_obj_t * bt_lb_obj = lv_label_create(parent_o); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(bt_lb_obj, parent_o ,LV_ALIGN_CENTER,-5,-2); //设置对齐
    lv_obj_add_style(bt_lb_obj, def_text_style, 0);   //给组件添加样式
    lv_label_set_text(bt_lb_obj, text);   //给标签添加文字
    return bt_lb_obj;
}
lv_obj_t * tools_create_login_checkbox(lv_obj_t * parent_o, int width, int height, char * text){   //在父窗口创建复选框
    //创建复选框
    lv_obj_t * checkbox = lv_checkbox_create(parent_o);       //放在登录窗口

    //设置大小和对齐方式
    if(width != -1 && height != -1){
        lv_obj_set_size(checkbox, width, height); 
    }
    
    //在复选框的右侧添加文本内容
    lv_checkbox_set_text(checkbox, text);

    lv_obj_add_style(checkbox, def_text_style, 0);   //支持中文显示
    return checkbox;
}

//将输入框和键盘连接
void tools_ta_kb_associate(lv_obj_t * ta, lv_obj_t * kb){   
    
    if(ta && kb){
        lv_keyboard_set_textarea(kb, ta);
    }
}