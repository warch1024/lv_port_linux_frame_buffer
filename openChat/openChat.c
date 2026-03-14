
#include"openChat/openChat.h"
//用于通信的线程对象
static pthread_t client_thread_id;


int open_chat(){
    //初始化功能
    open_chat_init();
    login_screen_init(); //转到登陆屏幕
}
//初始化必要组件，输入法，中文支持style等
void init_base_widget(){
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
//open chat 功能初始化处理函数
void open_chat_init(){
    //初始化openchat_client
    //定义线程属性变量并初始化
    pthread_attr_t client_thread_attr;//线程属性变量
	pthread_attr_init(&client_thread_attr);
	pthread_attr_setdetachstate(&client_thread_attr,PTHREAD_CREATE_DETACHED); //创建的所有线程可分离，无需手动回收
    //初始化openchat_client
    //debug
    printf("debug: client初始化 = %p\n", oc_online_client_ll);
    //单独的线程初始化openchat_client
    pthread_create(&client_thread_id, NULL, init_open_chat_client, NULL);
    //debug
    printf("debug: client初始2 \n");
    //初始化中文支持
    init_base_widget();        //初始化中文字体支持
}