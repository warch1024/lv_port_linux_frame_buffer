#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include"led.h"
#include "test-1.h"
#include "lvgl/src/extra/others/ime/lv_ime_pinyin.h"
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
    //使用时在make文件加上MAINSRC = ./main.c ./test_1.c ./eva_pic.c 
    // lv_obj_t * eva_pic = lv_img_create(lv_scr_act()); // 创建图片对象
    // LV_IMG_DECLARE(eva);           // 声明图片，不是.c里面数组的名字，是结构体的名字
    // lv_img_set_src(eva_pic, &eva); // 设置图片源，不是.c里面数组的名字，是结构体的名字
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
    static lv_style_t style;    //定义样式对象
    lv_style_init(&style);  //初始化样式
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);  //居中对齐（左，右，居中）

    /*Create a label with the new style*/
    lv_obj_t * label = lv_label_create(lv_scr_act());   //创建组件，使用样式
    lv_obj_add_style(label, &style, 0); //给组件添加样式
    lv_label_set_text(label, cn_s);
    lv_obj_center(label);
}



int led_click_lighting(char* led_file, int s_flag){
    int led_fd = open(led_file, O_RDWR);    // /dev/Led
    if(led_fd != -1){
        if(s_flag == 0){
            ioctl(led_fd, LED1, LED_OFF);
            ioctl(led_fd, LED2, LED_OFF);
            ioctl(led_fd, LED3, LED_OFF);
            ioctl(led_fd, LED4, LED_OFF);
        }
        else{
            for(int i=0; i<4; i++){
                ioctl(led_fd, _IO(TEST_MAGIC, i), LED_ON);
                
            }
        }
        close(led_fd);
        return 1;
    }
    return 0;
}

btn_switch_cb(lv_event_t * e){  //led灯回调函数
    led_click_lighting("/dev/Led", *(int*)(e->user_data));
    printf("切换状态%d\n",*(int*)(e->user_data));
    *(int*)(e->user_data) = ~(*(int*)(e->user_data));
}
void demo_btn_switch_led(){     //使用触屏按钮开关led灯

    
       //// 创建按钮
    lv_obj_t * bt1 = lv_btn_create(lv_scr_act());

    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(bt1, 300, 100);
    lv_obj_set_pos(bt1, 400, 240);

    // 给按钮设置文字-->LVGL没有提供专门的方法
    // 思路:创建一个标签(专门显示文字的),把标签嵌套到按钮上就可以显示文字
    //// 创建标签
    static lv_style_t yahei_style;    //这个样式会被别处函数调用使用
    create_font_style(&yahei_style, "/fonts/MSYH.TTC", 48);   //设置样式

    lv_obj_t * lb1 = lv_label_create(bt1); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_add_style(lb1, &yahei_style, 0);   //给组件添加样式
    // 给标签设置文字内容
    lv_label_set_text(lb1, "灯开关");   //给标签添加文字
    // 给按钮设置事件响应函数
    // lv_obj_add_event_cb(bt1,bt1_cb,LV_EVENT_CLICKED,NULL);
    // 单独讲解第四个参数的使用
    static int n1 = 0;  //demo_btn_switch_led函数返回n1失效,回调btn_switch_cb会被统一管理
    lv_obj_add_event_cb(bt1, btn_switch_cb, LV_EVENT_CLICKED, &n1);
}

void create_font_style(lv_style_t *style, char* font_path, int font_size){
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

void timer_event_cb(lv_timer_t * e){
    printf("你好\n");
}
void demo_timer(){
    lv_timer_create(timer_event_cb, 1000, NULL);      //创建定时器
    lv_timer_enable(true);  //开启所有定时器
}


lv_obj_t * list_bt1, *list_bt2, *list_bt3, *list_bt4;
lv_obj_t * list1 ;

void demo_lists(){
    //lv_obj_t * list1 = lv_list_create(lv_scr_act());
    list1 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list1,400,480);    //设置位置
    lv_obj_set_pos(list1,399,0);    //设置大小
    static lv_style_t yahei_style;    //这个样式会被别处函数调用使用
    create_font_style(&yahei_style, "/fonts/MSYH.TTC", 48);   //设置样式

    
    lv_obj_add_style(list1, &yahei_style, 0);   //给组件添加样式
    // 给标签设置文字内容
    lv_list_add_text(list1, "列表11111111111111111111111111111111111111111");   //给标签添加文字1
    lv_list_add_text(list1, "列表2");   //给标签添加文字
    lv_list_add_text(list1, "列表3");   //给标签添加文字
    lv_list_add_text(list1, "列表4");   //给标签添加文字
        //列表框添加按钮
    
    list_bt1=lv_list_add_btn(list1,NULL, "无线网络");
    list_bt2=lv_list_add_btn(list1,NULL, "软键盘");
    list_bt3=lv_list_add_btn(list1,NULL, "打开文件");

    //给三个按钮添加事件响应函数
    lv_obj_add_event_cb(list_bt1,btn_cb_list,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(list_bt2,btn_cb_list,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(list_bt3,btn_cb_list,LV_EVENT_CLICKED,NULL);
}
void btn_cb_list(lv_event_t *e){
      //判断究竟点击的是哪个按钮
    if(e->target==list_bt1)
    {
        //获取你点击的按钮上的文字内容
        printf("你点击的按钮文字内容是: %s\n",lv_list_get_btn_text(list1,list_bt1));
        printf("点击了列表框中的无线网络按钮!\n");
    }
   else if(e->target==list_bt2)
        printf("点击了列表框中的软键盘按钮!\n");
    else if(e->target==list_bt3)
        printf("点击了列表框中的打开文件按钮!\n");
}



lv_obj_t* btnmatrix1;
    //往矩阵按钮中添加你想要的按钮
lv_obj_t * matrix_bt1, *matrix_bt2, *matrix_bt3;
//矩阵按钮的事件响应函数
void demo_matrix_btn(){
      //创建三个按钮-->控制矩阵键盘上按钮的属性
    //第一个按钮:控制矩阵键盘上所有按钮的隐藏和显示
    matrix_bt1=lv_btn_create(lv_scr_act());
    //第二个按钮:控制矩阵键盘上按钮的禁用
    matrix_bt2=lv_btn_create(lv_scr_act());
    //第三个按钮:控制矩阵键盘上按钮可以使用
    matrix_bt3=lv_btn_create(lv_scr_act());
        //创建矩阵按钮
    
    lv_obj_set_size(matrix_bt1,100,50);
    lv_obj_set_pos(matrix_bt1,10,20);
    lv_obj_set_size(matrix_bt2,100,50);
    lv_obj_set_pos(matrix_bt2,10,100);
    lv_obj_set_size(matrix_bt3,100,50);
    lv_obj_set_pos(matrix_bt3,10,200);
     //添加事件响应函数
    lv_obj_add_event_cb(matrix_bt1,bt_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(matrix_bt2,bt_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(matrix_bt3,bt_cb,LV_EVENT_CLICKED,NULL);

    btnmatrix1=lv_btnmatrix_create(lv_scr_act());
    //设置位置,大小
    lv_obj_set_size(btnmatrix1,400,480);
    lv_obj_set_pos(btnmatrix1,100,0);
    //本函数返回会释放btnmatrix_map但是其他地方会用到,
    static const char* btnmatrix_map[] = {"1", "2","3","\n",
                                "4", "5","6","\n",
                                "7", "8","9","\n",
                                "#", "0","En",NULL}; 

    lv_btnmatrix_set_map(btnmatrix1,btnmatrix_map);
    lv_obj_add_event_cb(btnmatrix1,btnmatrix1_cb,LV_EVENT_CLICKED,NULL);
}

void btnmatrix1_cb(lv_event_t *e)
{
    uint16_t id; //存放按钮的ID

    //方法1: 获取你点击的按钮的字面值(按钮上的文字)
    //lv_obj_t *obj=lv_event_get_target(e); //获取你点击的那个按钮地址
    //id=lv_btnmatrix_get_selected_btn(obj);
    //char  *text=lv_btnmatrix_get_btn_text(btnmatrix1,id); 

    //if(strcmp(text,"1")==0)
        //printf("你点击是1\n");
    //else if(strcmp(text,"2")==0)
        //printf("你点击是2\n");  
        
    //验证:  e->target保存的究竟是整个矩阵按钮的地址,还是矩阵按钮中某个点击的按钮的地址
    //三个地址都是一样的,无论你点击哪个按钮,全部地址都是一样
    //但是获取到的ID是不一样,ID从0开始
    printf("整个矩阵按钮对象的地址: %p\n",btnmatrix1);
    printf("e->target存放的地址: %p\n",e->target);
    printf("你点击的那个按钮的地址: %p\n",lv_event_get_target(e));

    printf("你点击的那个按钮ID号是: %hu\n",lv_btnmatrix_get_selected_btn(e->target));
}
int click_count=0;
void bt_cb(lv_event_t *e)
{
    //判断点击的是哪个按钮
    if(e->target==matrix_bt1)
    {
        click_count++;
        if(click_count%2==1) //奇数次
            //隐藏矩阵键盘上所有的按钮
            lv_btnmatrix_set_btn_ctrl_all(btnmatrix1, LV_BTNMATRIX_CTRL_HIDDEN);
        else
            lv_btnmatrix_clear_btn_ctrl_all(btnmatrix1, LV_BTNMATRIX_CTRL_HIDDEN);
    }
    else if(e->target==matrix_bt2)
    {
        //printf("按钮222222\n");
        //禁用矩阵键盘上所有的按钮
        lv_btnmatrix_set_btn_ctrl_all(btnmatrix1, LV_BTNMATRIX_CTRL_DISABLED);
    }
    else if(e->target==matrix_bt3)
    {
        //恢复使用矩阵键盘上所有的按钮
        lv_btnmatrix_set_btn_ctrl_all(btnmatrix1, LV_BTNMATRIX_CTRL_CHECKABLE);
    }
}
// #############################输入法候选字字体与字号
lv_style_t pinyin_plugin_font_style;   //
void add_pinyin_plugin(lv_obj_t * kb){  //给kb添加中文支持
    
    create_font_style(&pinyin_plugin_font_style,"/fonts/MSYH.TTC", 20);
    lv_obj_t * pinyin_ime = lv_ime_pinyin_create(lv_scr_act()); //创建拼音输入法插件
    lv_obj_set_size(pinyin_ime, 1, 1);  //设置大小避免遮蔽
    lv_obj_set_pos(pinyin_ime, 0, 0);   //设置位置避免遮蔽
    lv_obj_add_style(pinyin_ime, &pinyin_plugin_font_style, 0); //输入法候选字正常显示中文
    lv_ime_pinyin_set_mode(pinyin_ime, LV_IME_PINYIN_MODE_K26); //设置默认模式
    lv_obj_t * cand_panel = lv_ime_pinyin_get_cand_panel(pinyin_ime);   //获取拼音候选栏对象
    lv_obj_set_width(cand_panel, 600); // 设置大小
    lv_ime_pinyin_set_keyboard(pinyin_ime, kb); //将拼音插件绑定到键盘
    /* 如果使用自定义字典
        则在lv_config.h将LV_IME_PINYIN_USE_DEFAULT_DICT宏置0
        使用lv_ime_pinyin_set_dict()设置自定义字典
        使用lv_ime_pinyin_set_mode()设置输入模式
    */

}
void demo_text_area_kb(){
    lv_obj_t * ta = lv_textarea_create(lv_scr_act());   //创建文本区
    //lv_textarea_set_one_line(ta,true);  //限定在一行,框满不会换行,会一直向右增长行
    //lv_textarea_set_accepted_chars(ta, "0123456789");   //设置限定输入字符
    //lv_textarea_set_password_mode(ta, true); //密码输入使用*代替,开启密码模式
    lv_obj_set_size(ta,500,200);
    lv_obj_set_pos(ta,150,0);
   
    //设置默认提示文字
    static lv_style_t def_text_style;
    create_font_style(&def_text_style,"/fonts/MSYH.TTC", 30);
    lv_textarea_set_placeholder_text(ta, "点击输入");
    lv_obj_add_style(ta, &def_text_style,0);

    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());   //创建键盘
     lv_obj_set_size(kb,600,220);
    lv_obj_set_pos(kb,0,0);     //位置放在0,0否则偏移很大
    //lv_obj_align_to(cand_panel, kb,LV_ALIGN_BOTTOM_MID, 0, -200); // 对齐到键盘
    add_pinyin_plugin(kb);
    
    //软键盘跟文本框关联-->关联之后软键盘输入的字符才可以在文本框显示
    lv_keyboard_set_textarea(kb,ta);

}
void demo_tabview(){
    lv_obj_t * tab_view = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 50);  //默认放在左上角
    lv_obj_t * tab_sub1 = lv_tabview_add_tab(tab_view, "tab1");
    lv_obj_t * tab_sub2 = lv_tabview_add_tab(tab_view, "tab2");
    lv_obj_t * tab_sub3 = lv_tabview_add_tab(tab_view, "tab3");
    //向第一个子选项中添加组件
    lv_obj_t * bt1 = lv_btn_create(tab_sub1); 
    lv_obj_t * ta = lv_textarea_create(tab_sub2);   //创建文本区
    lv_obj_t * kb = lv_keyboard_create(tab_sub3);
    lv_tabview_set_act(tab_view, 1, LV_ANIM_OFF);   //设置默认选项卡

}


void demo_msgbox(){
    static const char * mbbts[] = {"continue", "exit", ""};     //这个函数会结束需将变量延长生命周期
    lv_obj_t * tab_view = lv_msgbox_create(lv_scr_act(), "new msg","hello",mbbts, false);

}

void demo_type_design(){
    lv_obj_t * bt1 = lv_btn_create(lv_scr_act());   //在屏幕上创建按钮

    // 设置按钮的坐标位置和大小-->设置某个属性
    lv_obj_set_size(bt1, 200, 200);
    lv_obj_set_pos(bt1, 200, 140);
    lv_obj_set_style_bg_color(bt1, lv_color_hex(0x0fff0000),LV_STATE_DEFAULT);  //设置组件的背景颜色
    lv_obj_set_style_bg_opa(bt1, 100,LV_STATE_DEFAULT);  //设置组件的背景颜色透明度
     //创建标签
    lv_obj_t *label1=lv_label_create(lv_scr_act());
    //设置标签相对于按钮的位置对齐
    lv_obj_align_to(label1,bt1,LV_ALIGN_OUT_BOTTOM_MID,0,20);
    
    //设置标签字体大小
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_30, LV_STATE_DEFAULT);
    //设置标签的前景色-->文字的颜色
    lv_obj_set_style_text_color(label1, lv_color_hex(0x5084db), LV_STATE_DEFAULT);
    //标签设置文字
    lv_label_set_text(label1,"I am super man!");
     //创建复选框
    lv_obj_t* checkbox = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(checkbox, "passwd");
    //设置复选框按钮的上面对齐
    lv_obj_align_to(checkbox,bt1,LV_ALIGN_OUT_TOP_MID,0,-20);

    //设置复选框的状态-->禁止勾选
    lv_obj_add_state(checkbox,LV_STATE_CHECKED | LV_STATE_DISABLED);
    lv_obj_add_flag(bt1, LV_OBJ_FLAG_HIDDEN);//隐藏按钮
}
