#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include"tools/tools.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define NAMD_PIPE "/IOT/projects/26-2-9/maplay_pip"
/**
 视频播放器
 * 
 */
lv_style_t * player_text_style =NULL;
void init_player_font(){
    // 设置全局默认字体
    if(!player_text_style) {                                          
        player_text_style = (lv_style_t *)malloc(sizeof(lv_style_t)); // 申请字体style
        if(player_text_style != NULL) {
            tools_create_font_style(player_text_style, "/fonts/MSYH.TTC", 18); // 设置提示字体
        } else {
            perror("内存申请失败");
        }
    }

}
void mplayer_btn_cb(lv_event_t * e){
    printf("%s\n",e->user_data);
}

lv_obj_t * add_btn(lv_obj_t * parent_o, char * btn_text){
    lv_obj_t * mplayer_btn = lv_btn_create(parent_o);
    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(mplayer_btn, 100, 50);
    //添加按钮点击回调函数，点击按钮弹出窗口
    lv_obj_add_event_cb(mplayer_btn, mplayer_btn_cb, LV_EVENT_CLICKED, btn_text);

    //给按钮添加标签
    lv_obj_t * mplayer_btn_lb = lv_label_create(mplayer_btn);
    //按钮标签
    lv_label_set_text(mplayer_btn_lb, btn_text); //设置标签文字
    lv_obj_align(mplayer_btn_lb,LV_ALIGN_CENTER, 0,0);
    lv_obj_add_style(mplayer_btn_lb, player_text_style, 0);   //标签支持中文
    return mplayer_btn;
}


void make_player(){
    init_player_font();
    lv_obj_t * screen1 = lv_obj_create(NULL);
    lv_scr_load(screen1);   //必须加载屏幕，否则无法显示
    //设置尺寸
    lv_obj_set_size(screen1,800,480);
    //添加按钮
    lv_obj_t * fast_bk = add_btn(screen1, "快退\0");
    lv_obj_align(fast_bk, LV_ALIGN_BOTTOM_LEFT, 10 ,-10);
    lv_obj_t * fast_fw = add_btn(screen1, "快进\0");
    lv_obj_align_to(fast_fw, fast_bk, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
    lv_obj_t * fast_ps = add_btn(screen1, "暂停\0");
    lv_obj_align_to(fast_ps, fast_fw, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
    lv_obj_t * fast_ct = add_btn(screen1, "继续\0");
    lv_obj_align_to(fast_ct, fast_ps, LV_ALIGN_OUT_RIGHT_MID, 40, 0);


    //播放时频
    char cmd[200] = {0};
    snprintf(cmd, "mplayer -slave -quiet -input file=%s %s &", NAMD_PIPE, song_path);//播放视频
    system(cmd);


}


