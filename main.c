#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (800 * 480)


//按钮1的事件响应函数
void bt1_cb(lv_event_t * e)
{
    //打印主函数传递过来的信息
    printf("主函数传递的信息是: %d\n",*((int *)(e->user_data)));
    printf("恭喜登录成功!\n");
}

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);


    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon)
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/


    /*Create a Demo*/
    lv_demo_widgets();

    {
        //创建按钮
    lv_obj_t *bt1=lv_btn_create(lv_scr_act());

    //设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(bt1,100,50); 
    lv_obj_set_pos(bt1,400,240);

    //给按钮设置文字-->LVGL没有提供专门的方法
    //思路:创建一个标签(专门显示文字的),把标签嵌套到按钮上就可以显示文字
    //创建标签
    lv_obj_t *lb1=lv_label_create(bt1);  //按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    //给标签设置文字内容
    lv_label_set_text(lb1,"login");
     //给按钮设置事件响应函数
    //lv_obj_add_event_cb(bt1,bt1_cb,LV_EVENT_CLICKED,NULL);
    //单独讲解第四个参数的使用
    int n1=698;
    lv_obj_add_event_cb(bt1,bt1_cb,LV_EVENT_CLICKED,&n1);
    }




    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
