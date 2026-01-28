#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

void bt1_cb(lv_event_t * e)
{
    // 打印主函数传递过来的信息
    printf("主函数传递的信息是: %d\n", *((int *)(e->user_data)));
    printf("恭喜登录成功!\n");
}


void mydemo()
{
    // 创建按钮
    lv_obj_t * bt1 = lv_btn_create(lv_scr_act());

    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(bt1, 100, 50);
    lv_obj_set_pos(bt1, 400, 240);

    // 给按钮设置文字-->LVGL没有提供专门的方法
    // 思路:创建一个标签(专门显示文字的),把标签嵌套到按钮上就可以显示文字
    // 创建标签
    lv_obj_t * lb1 = lv_label_create(bt1); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    // 给标签设置文字内容
    lv_label_set_text(lb1, "login");
    // 给按钮设置事件响应函数
    // lv_obj_add_event_cb(bt1,bt1_cb,LV_EVENT_CLICKED,NULL);
    // 单独讲解第四个参数的使用
    int n1 = 1;
    lv_obj_add_event_cb(bt1, bt1_cb, LV_EVENT_CLICKED, &n1);
}
void demo_pic()
{

    lv_obj_t * eva_pic = lv_img_create(lv_scr_act()); // 创建图片对象
    LV_IMG_DECLARE(eva);           // 声明图片，不是.c里面数组的名字，是结构体的名字
    lv_img_set_src(eva_pic, &eva); // 设置图片源，不是.c里面数组的名字，是结构体的名字
}
void demo_pic_2()
{

    lv_obj_t * img1 = lv_img_create(lv_scr_act());  //创建图片
    lv_img_set_src(img1, "S:/IOT/projects/26-1-28/eva.bmp"); // 设置图片路径名
}
void demo_label()
{
    // 创建标签
    lv_obj_t * lb1 = lv_label_create(lv_scr_act());

    // 设置标签的坐标位置和大小-->设置某个属性
    lv_obj_set_size(lb1, 50, 50);
    lv_obj_set_pos(lb1, 150, 60);

    // 设置长模式-->做出动画效果
    lv_label_set_long_mode(lb1, LV_LABEL_LONG_SCROLL);

    // 设置文字
    lv_label_set_text(lb1, "Welcome Leader to yueqian zhidao gongzuo!");
    int num =56;
    lv_label_set_text_fmt(lb1, "Speed: %d RPM",num);

}

lv_obj_t *switch1;

//开关组件对应的事件响应函数
void switch1_cb(lv_event_t *e)
{
    //获取开关组件的状态-->通过状态值判定开关是开启还是关闭
    if(lv_obj_has_state(switch1, LV_STATE_CHECKED))
        printf("开关打开了\n");
    else
        printf("开关关闭了\n");
}
void demo_switch(){
    //创建开关组件-->有两种状态,开和关
    switch1=lv_switch_create(lv_scr_act());

    //设置开关组件的坐标位置和大小-->设置某个属性
    lv_obj_set_size(switch1,80,50); 
    lv_obj_set_pos(switch1,150,60);

    //给开关组件设置事件响应函数
    lv_obj_add_event_cb(switch1,switch1_cb,LV_EVENT_VALUE_CHANGED,NULL);

}

lv_obj_t *win1;  //小窗口1
lv_obj_t *win2;  //小窗口2

void demo_window(){
    //创建小窗口
    win1=lv_obj_create(lv_scr_act());
    win2=lv_obj_create(lv_scr_act());

    //设置小窗口的坐标位置和大小-->设置某个属性
    lv_obj_set_size(win1,300,230); 
    lv_obj_set_pos(win1,490,0);
    lv_obj_set_size(win2,300,230); 
    lv_obj_set_pos(win2,490,240);

    //往两个小窗口里面添加你需要的组件(如果不添加,两个小窗口都是空白)
    lv_switch_create(win1); //往小窗里面添加想要的组件
    lv_btn_create(win2);

}

lv_obj_t *checkbox1;  //复选框1
lv_obj_t *checkbox2;  //复选框2
lv_obj_t *checkbox3;  //复选框3
lv_obj_t *checkbox4;  //复选框4
//四个复选框共用一个事件响应函数
void checkbox_cb(lv_event_t *e)
{
    //判断究竟是哪个复选框导致这个事件响应函数被调用的
    if(e->target==checkbox1)
    {
        printf("你点击了复选框1\n");
        //进一步判断是你把复选框勾上还是取消了勾
        if(lv_obj_has_state(checkbox1,LV_STATE_CHECKED))
            printf("你勾上了复选框1111\n");
        else
            printf("你取消了复选框1111的勾选\n");
    }      
    else if(e->target==checkbox2)
    {
        printf("你点击了复选框2\n");    
        //进一步判断是你把复选框勾上还是取消了勾
        if(lv_obj_has_state(checkbox2,LV_STATE_CHECKED))
            printf("你勾上了复选框2222\n");
        else
            printf("你取消了复选框2222的勾选\n");
    }
         
}
void demo_checkbox(){
        //创建复选框
    checkbox1=lv_checkbox_create(lv_scr_act());
    checkbox2=lv_checkbox_create(lv_scr_act());
    checkbox3=lv_checkbox_create(lv_scr_act());
    checkbox4=lv_checkbox_create(lv_scr_act());

    //设置复选框的坐标位置和大小-->设置某个属性
    lv_obj_set_size(checkbox1,100,50); 
    lv_obj_set_pos(checkbox1,300,50);
    lv_obj_set_size(checkbox2,100,50); 
    lv_obj_set_pos(checkbox2,300,110);
    lv_obj_set_size(checkbox3,100,50); 
    lv_obj_set_pos(checkbox3,300,210);
    lv_obj_set_size(checkbox4,100,50); 
    lv_obj_set_pos(checkbox4,300,310);

    //在复选框的右侧添加文本内容
    lv_checkbox_set_text(checkbox1, "money duoduo");
    lv_checkbox_set_text(checkbox2, "event shaoshao");
    lv_checkbox_set_text(checkbox3, "family jinjin");
    lv_checkbox_set_text(checkbox4, "fuli duoduo");

    //给复选框添加事件响应函数
    //传统的做法:四个复选框,写四个不同的事件响应函数
    //偷懒的做法:四个复选框共用一个事件响应函数
    lv_obj_add_event_cb(checkbox1,checkbox_cb,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_add_event_cb(checkbox2,checkbox_cb,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_add_event_cb(checkbox3,checkbox_cb,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_add_event_cb(checkbox4,checkbox_cb,LV_EVENT_VALUE_CHANGED,NULL);
}

void demo_pic_on_btn(char * pic_path){
    lv_obj_t * bt1 = lv_btn_create(lv_scr_act());   //在屏幕上创建按钮

    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(bt1, 200, 200);
    lv_obj_set_pos(bt1, 200, 140);
    lv_obj_t * img1 = lv_img_create(bt1);  //创建图片
     char path[50] ="0";
     sprintf(path, "S:%s",pic_path);
    lv_img_set_src(img1, path); // 设置图片路径名

}

void demo_freetype_disp(char * cn_s)
{
    /*Create a font*/
    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = "/fonts/MSYH.TTC";
    info.weight = 62;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    /*Create style with the new font*/
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    /*Create a label with the new style*/
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, cn_s);
    lv_obj_center(label);
}
