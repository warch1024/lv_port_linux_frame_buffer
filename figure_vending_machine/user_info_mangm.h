#ifndef _USER_INFO_MANGM_H
#define _USER_INFO_MANGM_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/main_screen.h"
// #include "tools/tools.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

// #include<stdio.h>
typedef struct User_info
{
    char user_name[10];
    char user_pwd[20];
}user_info;


/* 输入用户信息
 * @brief 输入用户信息
 * 
 * @return user_info* 
 */
user_info * input_info();

/* 查找用户名
 * @brief 输入用户信息
 * 
 * @return user_info* 
 */
int find_user_name(char* user_name, FILE * p_user_record);

/* 保存用户信息
 * @brief 保存用户信息
 * 
 * @return int 
 */
int save_user_info(user_info * p_user_info, FILE* p_user_record);

/* 用户注册
 * @brief 注册用户
 * 
 * @record_file 传入用户记录所在文件路径
 */
int user_register(char* record_file);

#endif