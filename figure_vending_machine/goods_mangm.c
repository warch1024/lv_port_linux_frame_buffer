#include"figure_vending_machine/goods_mangm.h"


goods_info_t * listed_goods = NULL; //全局已上架商品列表
//存储默认上架的商品名和图片路径
static char def_goods_title[][40] = {"2233干杯", 
                                     "2233干贝", 
                                     "2233乾杯",
                                     "2233赣北"},
        def_goods_pic_path[][100] = {"S:/IOT/projects/26-1-30/resources/item1.jpg\0", 
                                    "S:/IOT/projects/26-1-30/resources/item2.jpg\0",
                                    "S:/IOT/projects/26-1-30/resources/item3.jpg\0",
                                    "S:/IOT/projects/26-1-30/resources/item1.jpg\0"};
//商品价格和数量
static uint32_t def_goods_price[1000] = {22, 33, 99, 88},
        def_goods_num[1000] = {999, 999, 999, 1};


//初始化最开始
void init_def_goods_to_list(){
    listed_goods = init_listed_goods_list();    //初始化头节点
    for(int i=0; i<sizeof(def_goods_title)/sizeof(def_goods_title[0]); i++){
        goods_info_t tmp;
        //只需数据域
        strcpy(tmp.goods_pic_path, def_goods_pic_path[i]);
        strcpy(tmp.title, def_goods_title[i]);
        tmp.title[sizeof(tmp.goods_pic_path)-1] = '\0'; //防止溢出
        tmp.num = def_goods_num[i];
        tmp.price = def_goods_price[i];
        add_listed_goods_list(listed_goods, tmp);   //增加商品节点
    }
    modify_head_listed_goods_list(listed_goods);    //更新总价和数量
}


///////////////////////////
//初始化头节点
goods_info_t *init_listed_goods_list(){ 
    goods_info_t *tmp = (goods_info_t *)malloc(sizeof(goods_info_t));
    if(tmp){
        //初始化信息
        strcpy(tmp->title, "listed_goods_list");
        tmp->goods_pic_path[0] = '\0';
        tmp->price = 0;
        tmp->num = 0;
        //初始化指针
        tmp->next = NULL;
        tmp->prev = NULL;
        tmp->tail = tmp;
        return tmp;
    }
    return NULL;
}

//传入要上架的新商品保存至已上架列表
goods_info_t * add_listed_goods_list(goods_info_t *listed_goods_list, goods_info_t goods){
    if(listed_goods_list){  //输入头节点
       goods_info_t *tmp = (goods_info_t*)malloc(sizeof(goods_info_t));
       if(tmp){
            *tmp  =goods;   //保存数据域
            //保存指针域
            tmp->prev = listed_goods_list->tail;
            tmp->next = NULL;
            listed_goods_list->tail->next = tmp;
            listed_goods_list->tail = tmp;
        }
        modify_head_listed_goods_list(listed_goods_list);    //更新总价和数量
    }
    return listed_goods_list;
}

//删除已上架商品
goods_info_t *delete_listed_goods_list(goods_info_t *listed_goods_list, char * goods){  
    if(listed_goods_list && listed_goods_list->next != NULL){    //确保不是空指针,和链表非空
        for(goods_info_t * tmp_node = listed_goods_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(strcmp(tmp_node->title, goods) == 0){   //找放商品存放节点
                tmp_node->prev->next = tmp_node->next;  //调整上一个节点的next
                if(tmp_node == listed_goods_list->tail){  //待删节点是尾节点
                    listed_goods_list->tail = tmp_node->prev; //调整尾节点
                }
                else{   //后面还有节点
                    tmp_node->next->prev = tmp_node->prev;
                }
                free(tmp_node);//释放节点
                return listed_goods_list;
            }
        }
        modify_head_listed_goods_list(listed_goods_list);    //更新总价和数量
    }
    return listed_goods_list;
}

//查找商品节点
goods_info_t * find_listed_goods_list(goods_info_t *listed_goods_list, char * goods_name){
    for(goods_info_t * tmp_node = listed_goods_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
        if(strcmp(tmp_node->title, goods_name) == 0){ 
            return tmp_node;
        }
    }
    return NULL;
}
//修改库存,名字，数量，价格
int modify_listed_goods_list(goods_info_t *listed_goods_list, char * goods_name, uint32_t num, uint32_t price){
    if(listed_goods_list){  //输入头节点
       goods_info_t* goods_node = NULL;
       if((goods_node = find_listed_goods_list(listed_goods_list, goods_name))){    //找到商品
            goods_node->num = num;
            goods_node->price = price;
            return 1;
       }
       modify_head_listed_goods_list(listed_goods_list);    //更新总价和数量
    }
    return 0;
}
//结算后调整库存
int settle_modify_listed_goods_list(goods_info_t *listed_goods_list, char * goods_name, uint32_t num){
    if(listed_goods_list){  //输入头节点
       goods_info_t* goods_node = NULL;
       if((goods_node = find_listed_goods_list(listed_goods_list, goods_name))){    //找到商品
            goods_node->num = num;
            return 1;
       }
       modify_head_listed_goods_list(listed_goods_list);    //更新总价和数量
    }
    return 0;
}

//调整头节点
void modify_head_listed_goods_list(goods_info_t *listed_goods_list){
    if(listed_goods_list){
        listed_goods_list->price = 0;
        listed_goods_list->num = 0;
        for(goods_info_t * tmp_node = listed_goods_list->next; tmp_node != NULL; tmp_node = tmp_node->next){    //统计默认上架商品的总价
            listed_goods_list->price += tmp_node->price * tmp_node->num;    //记录总价
            listed_goods_list->num ++;  //只记录类数
        }
    }
}