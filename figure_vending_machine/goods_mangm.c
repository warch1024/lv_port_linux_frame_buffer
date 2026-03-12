#include"figure_vending_machine/goods_mangm.h"


tools_dll_t * listed_goods = NULL; //全局已上架商品列表
//存储默认上架的商品名和图片路径
static char def_goods_title[][40] = {"2233干杯", 
                                     "2233干贝", 
                                     "2233乾杯",
                                     "2233赣北",
                                    "2233[]~(￣▽￣)~*"},
        def_goods_pic_path[][100] = {"S:/IOT/projects/26-1-30/resources/item1.jpg\0", 
                                    "S:/IOT/projects/26-1-30/resources/item2.jpg\0",
                                    "S:/IOT/projects/26-1-30/resources/item3.jpg\0",
                                    "S:/IOT/projects/26-1-30/resources/item1.jpg\0",
                                    "S:/IOT/projects/26-1-30/resources/item3.jpg\0",};
//商品价格和数量
static uint32_t def_goods_price[1000] = {22, 33, 99, 88, 998},
        def_goods_num[1000] = {999, 999, 999, 1, 10};


//初始化最开始
void init_def_goods_to_list(){
    //初始化头节点
    if(!listed_goods){
        listed_goods = tools_init_dll_list();    

    
    //初始化默认商品到已上架列表
    for(int i=0; i<sizeof(def_goods_title)/sizeof(def_goods_title[0]); i++){
        goods_info_t * tmp = (goods_info_t*)malloc(sizeof(goods_info_t));   //商品信息对象
        //只需数据域
        strcpy(tmp->goods_pic_path, def_goods_pic_path[i]);
        strcpy(tmp->title, def_goods_title[i]);
        tmp->goods_pic_path[sizeof(tmp->goods_pic_path)-1] = '\0'; //防止溢出
        tmp->title[sizeof(tmp->title)-1] = '\0'; //防止溢出
        tmp->num = def_goods_num[i];    //设置商品的数量
        tmp->price = def_goods_price[i];    //商品价格
        tools_add_dll_list_node(listed_goods, tmp);   //增加商品节点
    }
}
    
}


///////////////////////////

//传入要上架的新商品保存至已上架列表
goods_info_t * list_listed_goods_list(tools_dll_t *listed_goods_list, goods_info_t new_goods){
    if(listed_goods_list){  //输入头节点
        goods_info_t * tmp = (goods_info_t *)malloc(sizeof(goods_info_t));
        if(tmp){
            *tmp = new_goods;   //复制数据域
            tools_add_dll_list_node(listed_goods_list, tmp);//添加节点
        }
        return tmp; //返回刚上架的商品地址
    }
}

//删除已上架商品
goods_info_t *delete_listed_goods_list(tools_dll_t *listed_goods_list, goods_info_t * del_goods){  
    if(listed_goods_list && del_goods){    //确保不是空指针,和链表非空
        tools_delete_dll_list_node(listed_goods_list, del_goods);
    }
    return listed_goods_list;
}

//查找商品返回商品对象地址
goods_info_t * find_listed_goods_list(tools_dll_t *listed_goods_list, char * goods_name){
    
    for(tools_dll_t * tmp_node = listed_goods_list->next; tmp_node != NULL; tmp_node = tmp_node->next){

        if(strcmp(((goods_info_t *)(tmp_node->data))->title, goods_name) == 0){ 
        
            return (goods_info_t *)(tmp_node->data);    //找到返回商品信息节点
        }
    }
    return NULL;    //找不到返回NULL
}
//修改库存,名字，数量，价格
int modify_listed_goods_list(tools_dll_t *listed_goods_list, char * goods_name, uint32_t num, uint32_t price){
    if(listed_goods_list){  //输入头节点
       goods_info_t* goods_node = NULL;
       if((goods_node = find_listed_goods_list(listed_goods_list, goods_name))){    //找到商品
            goods_node->num = num;
            goods_node->price = price;
            return 1;
       }
    }
    return 0;
}
//结算后调整单个商品库存
int settle_modify_listed_goods_list(tools_dll_t *listed_goods_list, char * goods_name, uint32_t num){
    if(listed_goods_list){  //输入头节点
       goods_info_t* goods_node = NULL;
       if((goods_node = find_listed_goods_list(listed_goods_list, goods_name))){    //找到商品
            goods_node->num = num;
            return 1;
       }
    }
    return 0;
}

void print_goods_info(tools_dll_t * listed_goods_list){
    //getchar();
     for(tools_dll_t * tmp_node = listed_goods_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
        printf("%s\n%s\nnum: %d\tprice: %d\n", ((goods_info_t*)(tmp_node->data))->goods_pic_path,
            ((goods_info_t*)(tmp_node->data))->title,
            ((goods_info_t*)(tmp_node->data))->num,
            ((goods_info_t*)(tmp_node->data))->price);
    }
}