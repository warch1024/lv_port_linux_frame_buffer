#include"figure_vending_machine/screen_objs.h"  //标准库

lv_obj_t                //管理所有屏幕
    * login_screen_o = NULL,   //登录界面
    * main_screen_o = NULL;   //主界面
    
lv_style_t * def_text_style = NULL; //默认提示文字的style
    



lv_obj_pn * objs_save_obj(lv_obj_pn * lv_obj_dic, lv_obj_t * obj, char * obj_name){    //保存新建对象到链表
    lv_obj_pn * new_lv_obj_pn = (lv_obj_pn*)malloc(sizeof(lv_obj_pn));  //新建对象
    if(new_lv_obj_pn){  //添加节点
        strcpy(new_lv_obj_pn->name, obj_name);  //对象名
        new_lv_obj_pn->obj = obj;   //保存lv对象

        lv_obj_dic->tail->next = new_lv_obj_pn; //挂载新节点
        new_lv_obj_pn->prev = lv_obj_dic->tail;    //挂载新节点
        new_lv_obj_pn->next = NULL;     //设置尾节点指针
        lv_obj_dic->tail = new_lv_obj_pn;   //设置新的尾节点

    }
    return lv_obj_dic;  //返回字典
}
//删除节点及节点存储的lv对象
lv_obj_pn * objs_del_obj(lv_obj_pn * lv_obj_dic, char * obj_name){    //保存新建对象到链表
    for(lv_obj_pn* tmp = lv_obj_dic->next; tmp != NULL; tmp = tmp->next){   //遍历节点
        if(strcmp(tmp->name, obj_name) == 0){   //匹配
            if(lv_obj_dic->tail == tmp){        //如果是尾节点
                lv_obj_dic->tail = tmp->prev;   //切换尾节点指针
                lv_obj_dic->tail->next = NULL;  //尾节点next置空
                lv_obj_del(tmp->obj);   //删除obj及其所有子对象
                free(tmp);  //释放尾节点
            }
            else if(1);
        }
    }
    return NULL;  //没找到返回空
}