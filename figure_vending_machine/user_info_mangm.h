#ifndef _USER_INFO_MANGM_H
#define _USER_INFO_MANGM_H
// #include "figure_vending_machine/login_screen.h"    
// #include "figure_vending_machine/main_screen.h"
#include "tools/tools.h"

#include"figure_vending_machine/screen_objs.h"  //标准库

// #include<stdio.h>

typedef struct User_info_t{
    char user_name[10];
    char user_psw[20];
}user_info_t;

extern tools_dll_t * user_info_list;

int verify_user_name_psw(user_info_t * usr_info, tools_dll_t * user_info_list);
user_info_t * input_info();
int find_user_name_in_list(char* user_name, tools_dll_t * user_info_list);
void init_user_info_list(char* user_info_file_path);
void add_user_info_to_list(tools_dll_t * user_info_list, user_info_t* user_info);
void user_register(tools_dll_t *user_info_list, char* user_info_file_path);
int write_user_info_list_to_file(tools_dll_t *user_info_list, char* user_info_file_path);
int is_admin(user_info_t * user);
#endif