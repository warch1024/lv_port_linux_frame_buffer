#include "figure_vending_machine/user_info_mangm.h"


user_info * input_info(){   //提取输入的用户名和密码
    user_info * user_tmp = (user_info *)malloc(sizeof(user_info));
    if(user_tmp){
        
        printf("输入用户名：");
        scanf("%s",&user_tmp->user_name[0]);
        printf("输入密码：");
        scanf("%s",&user_tmp->user_pwd[0]);
        return user_tmp;
    }
    else{
        perror("内存分配失败");
        return NULL;
    }
   
}
int find_user_name(char* user_name, FILE * p_user_record){
    if(p_user_record && user_name){
        char name_pwd_buf[32]={0};
        while(fgets(name_pwd_buf, 32, p_user_record)){    //循环读取每一行
            char * name = strtok(name_pwd_buf,"@");
            if(name){
                if(!strcmp(name, user_name)){
                    return 1;   //有重复用户名
                }
            }
            else{
                perror("切割错误");
                return -1;
            }
        }
        return 0;   //     无重复用户名
    }
    else{
        perror("文件描述符错误");
        return -2;
    }
}

int save_user_info(user_info * p_user_info, FILE* p_user_record){   //保存用户信息到文件
    if(p_user_info && p_user_record){
        char user_name_pwd_buf[32] = {0};
        sprintf(user_name_pwd_buf, "%s@%s",&p_user_info->user_name[0], &p_user_info->user_pwd[0]);
        fwrite(user_name_pwd_buf, strlen(&user_name_pwd_buf[0]), 1, p_user_record);
        fwrite("\n", 1, 1, p_user_record);
        return 1;
    }
    else{
        perror("用户信息保存失败");
        return 0;
    }
}
int user_register(char* record_file){

    while(1){
        user_info * p_user = input_info();
        if(p_user){
            FILE * user_info_recorder = fopen(record_file, "r");    //标准io打开文件
            if(user_info_recorder == NULL){ //文件不存在则新建
                user_info_recorder = fopen(record_file, "w");
                fclose(user_info_recorder);
                user_info_recorder = fopen(record_file, "r");
            }
            if(user_info_recorder){
                int find_result = find_user_name(&p_user->user_name[0], user_info_recorder);    //查找用户名
                if(find_result == 0){   //添加新用户到文件
                    fclose(user_info_recorder);
                    user_info_recorder = fopen(record_file, "a");
                    if(user_info_recorder){
                        if(save_user_info(p_user, user_info_recorder)){ //保存用户信息到文件
                            printf("ok\n");
                            fclose(user_info_recorder); //关闭文件
                            free(p_user);   //释放暂存用户信息的内存
                            break;
                        }
                        else{   //保存失败，关闭文件和释放缓存
                            fclose(user_info_recorder);
                            free(p_user);
                            printf("保存失败");
                            return 0;
                        }
                    }
                    else{   //追加模式打开文件失败，关闭文件和释放缓存
                        free(p_user);
                        perror("添加失败");
                        return 0;
                    }
                }
                else if(find_result == 1){  //用户名已存在，重新输入注册信息
                    fclose(user_info_recorder);
                    free(p_user);
                    //system("clear");
                    printf("用户名已存在!\n");
                    continue;
                }
            }
        }
    }
    return 1;   //添加成功返回1
}
