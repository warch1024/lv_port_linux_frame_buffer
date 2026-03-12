#include "figure_vending_machine/user_info_mangm.h"

tools_dll_t * user_info_list = NULL;  //全局保存用户信息变量



//验证用户信息 成功返回1
int verify_user_name_psw(user_info_t * usr_info, tools_dll_t * user_info_list){
    if(user_info_list && usr_info){
        //debug
        printf("验证1-%d\n",user_info_list->num);
        for(tools_dll_t * tmp_node = user_info_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            printf("验证2\n");
            //debug
            printf("链表%s\n%s\n",((user_info_t*)(tmp_node->data))->user_name, 
                ((user_info_t*)(tmp_node->data))->user_psw
            );
            printf("传入%s\n%s\n",usr_info->user_name, usr_info->user_psw);
            if(strcmp(((user_info_t*)(tmp_node->data))->user_name, usr_info->user_name)){ //用户名验证成功
                if(strcmp(((user_info_t*)(tmp_node->data))->user_psw, usr_info->user_psw)){ //密码验证成功
                    return 1;//成功返回1
                }
                else{
                    return -2;  //密码错误
                }
            }
            else{
                return -1;  //用户名错误
            }
        }
    }
    return 0;
}


//提取输入的用户名和密码
user_info_t * input_info(){   
    user_info_t * user_tmp = (user_info_t *)malloc(sizeof(user_info_t));
    if(user_tmp){
        
        printf("输入用户名：");
        scanf("%s",&user_tmp->user_name[0]);
        printf("输入密码：");
        scanf("%s",&user_tmp->user_psw[0]);
        return user_tmp;
    }
    else{
        perror("内存分配失败");
        return NULL;
    }
   
}
//查找用户名不存在返回0
int find_user_name_in_list(char* user_name, tools_dll_t * user_info_list){
    if(user_info_list && user_name){
        for(tools_dll_t * tmp_node = user_info_list->next; tmp_node != NULL; tmp_node = tmp_node->next){
            if(strcmp(((user_info_t*)(tmp_node->data))->user_name, user_name)){ //存在重名用户
                return 1;
            }
        }
    }
    return 0;
}


//初始化链表
void init_user_info_list(char* user_info_file_path){
    if(!user_info_list) { // 为空则初始化
        user_info_list = tools_init_dll_list();
    } // 加载到链表
    FILE * user_info_recorder = fopen(user_info_file_path, "r"); // 存在则读取
    //debug
    printf("init-1\n");
    if(user_info_recorder != NULL) {                             // 打开成功
        //debug
        printf("init-2\n");
        char name_pwd_buf[32] = {0};                             // 文件读取行缓冲
        while(fgets(name_pwd_buf, 32, user_info_recorder)){           // 循环读取每一行
            //debug
            printf("init-3\n");
            char * name = strtok(name_pwd_buf, "@");             // 保存用户名
            char * psw  = strtok(name_pwd_buf, "\n");            // 保存密码
            if(name && psw) {
                user_info_t * user_tmp = (user_info_t *)malloc(sizeof(user_info_t));
                strcpy(user_tmp->user_name, name);
                strcpy(user_tmp->user_psw, psw);
                tools_add_dll_list_node(user_info_list, user_tmp); // 保存信息到链表
            }
        }
    }
    // 否则没有用户信息要读取
}

//添加一个注册的用户信息到链表
void add_user_info_to_list(tools_dll_t * user_info_list, user_info_t* user_info){
    tools_add_dll_list_node(user_info_list, user_info);
}

//将链表信息写入文件
int write_user_info_list_to_file(tools_dll_t *user_info_list, char* user_info_file_path){
    if(user_info_list && user_info_file_path) {
        FILE * user_info_recorder = fopen(user_info_file_path, "w"); // 标准io打开文件,文件不存在则新建
        if(user_info_recorder != NULL) {                             // 打开成功
            for(tools_dll_t * tmp_node = user_info_list->next; tmp_node != NULL; tmp_node = tmp_node->next) {
                char user_name_pwd_buf[32] = {0};
                sprintf(user_name_pwd_buf, "%s@%s\n", 
                    &(((user_info_t *)(tmp_node->data))->user_name[0]),
                    &(((user_info_t *)(tmp_node->data))->user_psw[0])
                );
                fwrite(user_name_pwd_buf, strlen(&user_name_pwd_buf[0]), 1, user_info_recorder);    //写入文件
            }
            fclose(user_info_recorder); //写入成功关闭文件
            return 1; // 成功返回1
        } else {
            perror("用户信息保存失败");
            return 0;
        }
    }
}

//注册的信息最后保存到文件
void user_register(tools_dll_t *user_info_list, char* user_info_file_path){
    user_info_t * tmp = input_info();
    if(find_user_name_in_list(tmp->user_name, user_info_list) == 1) { // 用户名已存在
        free(tmp);                                                    // 释放暂存的信息
        user_register(user_info_list, user_info_file_path);           // 重新注册
    } else {
        add_user_info_to_list(user_info_list, tmp);                                  // 添加信息到链表
        if(write_user_info_list_to_file(user_info_list, user_info_file_path) == 1) { // 写入链表到文件
        }
    }
}



int is_admin(user_info_t * user){
    if(!strcmp("admin", user->user_name)) {   // 用户名验证成功
        if(!strcmp("123456", user->user_psw)) { // 密码验证成功
            return 1;                                                                 // 成功返回1
        }
    }
    return 0;
}