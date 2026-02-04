
#include"figure_vending_machine/figure_vending_machine.h"


int figure_vending_machine(){
    init_base_widgets();        //初始化中文字体支持
    login_screen(); //显示登陆界面
}
//初始化必要组件，输入法，中文支持style等
void init_base_widgets(){
    // 设置全局默认字体
    if(!def_text_style) {                                          
        def_text_style = (lv_style_t *)malloc(sizeof(lv_style_t)); // 申请字体style
        if(def_text_style != NULL) {
            tools_create_font_style(def_text_style, "/fonts/MSYH.TTC", 18); // 设置提示字体
        } else {
            perror("内存申请失败");
        }
    }

}