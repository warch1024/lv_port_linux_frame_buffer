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
    if(!parent_obj && !kb){
        printf("debug: 父对象为空，或键盘为空，无法创建拼音输入法插件\n");
        return NULL;
    }
    lv_obj_t * pinyin_ime = lv_ime_pinyin_create(parent_obj); //创建拼音输入法插件
    if(!pinyin_ime){
        printf("debug: 创建拼音输入法插件失败\n");
        return NULL;
    }
    
    lv_obj_set_size(pinyin_ime, 1, 1);  //设置大小避免遮蔽
    lv_obj_set_pos(pinyin_ime, 0, 0);   //设置位置避免遮蔽
    lv_obj_add_style(pinyin_ime, def_text_style, 0); //输入法候选字正常显示中文
    lv_ime_pinyin_set_mode(pinyin_ime, LV_IME_PINYIN_MODE_K26); //设置默认模式
    lv_obj_t * cand_pannel = lv_ime_pinyin_get_cand_panel(pinyin_ime);   //获取拼音候选栏对象
    if(!cand_pannel){
        printf("debug: 获取拼音候选栏对象失败\n");
        return NULL;
    }
    lv_obj_set_width(cand_pannel, kb_width); // 绑定后续按字宽度到键盘
    lv_ime_pinyin_set_keyboard(pinyin_ime, kb); //将拼音插件绑定到键盘
    /* 如果使用自定义字典
        则在lv_config.h将LV_IME_PINYIN_USE_DEFAULT_DICT宏置0
        使用lv_ime_pinyin_set_dict()设置自定义字典
        使用lv_ime_pinyin_set_mode()设置输入模式
    */
   lv_obj_move_foreground(pinyin_ime); //将拼音输入法插件置于顶层
   lv_obj_move_foreground(cand_pannel); //将拼音候选栏置于顶层
    return cand_pannel;   // 返回候选栏
}
cn_kb_cp_ros tools_create_pinyin_ime(lv_obj_t * parent_obj, int weight, int height){ //将键盘放在obj上
    if(!parent_obj){
        printf("debug: 父对象为空，无法创建拼音输入法\n");
        return (cn_kb_cp_ros){NULL, NULL};
    }
    lv_obj_t * cn_kb = lv_keyboard_create(parent_obj);                                // 屏幕上添加键盘
    lv_obj_set_size(cn_kb, weight, height);
    lv_obj_set_pos(cn_kb, 0, 0); // 位置放在0,0否则偏移很大
    // 设置候选栏插件
    lv_obj_t * cn_kb_cp = add_pinyin_plugin(cn_kb, parent_obj, weight); // 窗口添加拼音插件
    // 隐藏键盘
    if(cn_kb && cn_kb_cp) {

        tools_hidden_pinyin_kb(cn_kb, cn_kb_cp);
        
        cn_kb_cp_ros pair = {.cn_kb = cn_kb, .cn_kb_cp = cn_kb_cp};
        return pair;
    }
    else{
        printf("debug: 创建拼音输入法失败，键盘%p, 候选栏%p\n", cn_kb, cn_kb_cp);
        return (cn_kb_cp_ros){NULL, NULL};
    }
}

void tools_hidden_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp){
    if(kb && cp){
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);//隐藏键盘
        lv_obj_add_flag(cp, LV_OBJ_FLAG_HIDDEN);//隐藏候选栏
        printf("debug: 隐藏拼音键盘键盘%p, 候选栏%p\n", kb, cp);
    }
}
void tools_show_pinyin_kb(lv_obj_t * kb, lv_obj_t * cp){
    if(kb && cp){
        //debug
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);//清除隐藏键盘
        lv_obj_clear_flag(cp, LV_OBJ_FLAG_HIDDEN);//清除隐藏候选栏
        printf("debug: 显示拼音键盘键盘%p, 候选栏%p\n", kb, cp);
        // //将键盘和候选栏置于顶层
        lv_obj_move_foreground(kb);
        lv_obj_move_foreground(cp);
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
lv_obj_t* tools_create_common_ta(lv_obj_t* parent_o, int width, int height, char* pht){
    lv_obj_t* ta_obj = lv_textarea_create(parent_o);    //添加输入框
    lv_obj_set_size(ta_obj, width, height);  //设置尺寸
    lv_textarea_set_placeholder_text(ta_obj, pht);  //设置提示词
    //设置提示词左对齐
    lv_textarea_set_align(ta_obj, LV_TEXT_ALIGN_LEFT);
    lv_obj_add_style(ta_obj, def_text_style,0);     //中文支持
    // lv_textarea_set_one_line(ta_obj,true);  //限定在一行
    // lv_textarea_set_max_length(ta_obj ,8);  //限制用户名长度
    return ta_obj;
}
lv_obj_t* tools_create_login_bt_lb(lv_obj_t* parent_o,char* text){ //给登录窗口按钮添加标签
    lv_obj_t * bt_lb_obj = lv_label_create(parent_o); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(bt_lb_obj, parent_o ,LV_ALIGN_CENTER,-5,-2); //设置对齐
    lv_obj_add_style(bt_lb_obj, def_text_style, 0);   //给组件添加样式
    lv_label_set_text(bt_lb_obj, text);   //给标签添加文字
    return bt_lb_obj;
}
lv_obj_t* tools_add_bt_lb(lv_obj_t* parent_o,char* text){ //给按钮添加标签
    lv_obj_t * bt_lb_obj = lv_label_create(parent_o); // 按钮作为标签的父窗口,等一会标签就会嵌套到按钮上
    lv_obj_align_to(bt_lb_obj, parent_o ,LV_ALIGN_CENTER,0,0); //设置对齐
    lv_obj_center(bt_lb_obj); //将标签居中对齐到按钮
    lv_obj_add_style(bt_lb_obj, def_text_style, 0);   //给组件添加样式
    lv_label_set_text(bt_lb_obj, text);   //给标签添加文字
    return bt_lb_obj;
}
lv_obj_t* tools_create_lbed_bt(lv_obj_t* parent_o,char* text, int width, int height, lv_color_t color){ //创建按钮
    lv_obj_t * bt_obj = lv_btn_create(parent_o);
    lv_obj_set_size(bt_obj, width, height);//设置尺寸
    //lv_obj_align_to(bt_obj, parent_o, LV_ALIGN_BOTTOM_LEFT, 5, 0);  //对齐
    lv_obj_set_style_bg_color(bt_obj, color, LV_PART_MAIN); // 设置按钮颜色
    lv_obj_t * bt_lb_obj = tools_add_bt_lb(bt_obj, text);//创建发送表情包按钮文字
    return bt_obj;//返回按钮对象
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
//给obj设置透明度,参数 LV_OPA_10
void tools_set_opa_style(lv_style_t * opa_style, lv_obj_t * obj, lv_opa_t opa_value){
    //  if(!main_screen_opa_style){ //登录窗口的透明度主题
    //     main_screen_opa_style = (lv_style_t *)malloc(sizeof(lv_style_t));
    // }
    if(opa_style){  //设置样式
        lv_style_init(opa_style);   //必须初始化
        lv_style_set_bg_opa(opa_style, opa_value);
        lv_obj_add_style(obj, opa_style, 0);
    }
}


void tools_set_bg_style(lv_style_t * bg_style, lv_obj_t * obj, lv_opa_t opa_value, char * fig_path){

    if(bg_style && obj){

        //先移除旧背景
        lv_obj_remove_style(obj, bg_style, LV_PART_ANY | LV_STATE_ANY);  //移除背景
        lv_style_init(bg_style);
        //lv_obj_add_state(login_check_box2, LV_STATE_CHECKED);//默认主题2
        
        // 设置背景图片
        lv_style_set_bg_img_src(bg_style, fig_path);
        // 设置背景图片的透明度
        lv_style_set_bg_img_opa(bg_style, opa_value);
        
        // 应用样式到屏幕
        lv_obj_add_style(obj, bg_style, 0);
    }
}

///////////////双向带尾节点指针链表
//初始化头节点
tools_dll_t *tools_init_dll_list(){ 
  
    tools_dll_t *head = (tools_dll_t*)malloc(sizeof(tools_dll_t));
    if(head){
        //初始化数据与
        head->data = NULL;
        head->num = 0;
        //初始化指针
        head->next = NULL;
        head->prev = NULL;
        head->tail = head;
        return head;
    }
    return NULL;
}

//增加新节点到链表
tools_dll_t * tools_add_dll_list_node(tools_dll_t * dll_list, void* data){
    if(dll_list){  //输入头节点
       tools_dll_t *tmp = (tools_dll_t*)malloc(sizeof(tools_dll_t));
       if(tmp){
            tmp->data = data;   //保存数据域
            tmp->num = 1;
            //保存指针域
            tmp->prev = dll_list->tail;
            tmp->next = NULL;
            dll_list->tail->next = tmp;
            dll_list->tail = tmp;
            dll_list->num ++;   //更新节点数量
        }
        return tmp; //返回当前节点
    }
}

//删除node节点
tools_dll_t *tools_delete_dll_list_node(tools_dll_t * dll_list, tools_dll_t * node){
    if(dll_list && dll_list->next != NULL){    //确保不是空指针,和链表非空
        for(tools_dll_t * tmp_node = dll_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(tmp_node == node){   //找到节点
                tmp_node->prev->next = tmp_node->next;  //调整上一个节点的next
                if(tmp_node == dll_list->tail){  //待删节点是尾节点
                    dll_list->tail = tmp_node->prev; //调整尾节点
                }
                else{   //后面还有节点
                    tmp_node->next->prev = tmp_node->prev;
                }
                //调整完成
                // if(tmp_node->data){
                //     free(tmp_node->data);   //释放数据域申请的内存
                // }
                free(tmp_node);//释放节点
                dll_list->num --;   //调整节点计数
                return dll_list;    //返回链表
            }
        }
    }
    return dll_list;
}

//查找数据所在节点,返回节点地址
tools_dll_t * tools_find_dll_list_node(tools_dll_t *dll_list, void * data){
    if(dll_list){
        for(tools_dll_t * tmp_node = dll_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(tmp_node->data == data){ 
                return tmp_node;
            }
        }
    }
    return NULL;    //查找失败返回空
}
//修改data
int tools_modify_dll_list_node(tools_dll_t * dll_list, void * old_data, void * new_data){
    if(dll_list){  //输入头节点
        tools_dll_t * modify_node = NULL;
        if((modify_node = tools_find_dll_list_node(dll_list, old_data)) != NULL){
            modify_node->data = new_data;   //更新数据域
            return 1;
        }
    }
    return 0;
}


lv_obj_t* tools_get_peer_obj_via_user_label(lv_obj_t* peer_obj, void * user_label){
    lv_obj_t* item_window = lv_obj_get_parent(peer_obj);    //获取父对象
    if(item_window == NULL) return NULL;
    
    for(int i = 0; i < lv_obj_get_child_cnt(item_window); i++) {
        lv_obj_t* target_child = lv_obj_get_child(item_window, i);
        if(lv_obj_get_user_data(target_child) == (void*)user_label){
            
            return target_child;    //返回目标子对象
        }
    }
    return NULL;
}
