#ifndef __OPEN_CHAT_CLIENT_H__
#define __OPEN_CHAT_CLIENT_H__

#include "../tools/open_chat_tools.h"
#include "openChat/main_screen.h"
#include "openChat/chat_func.h"
#define SYNC_MSG_SIZE 1024

extern struct clientmsg *oc_online_client_ll; //全局变量，指向存储的所有连接到此服务器的客户端的信息
extern int oc_client_sock;	//保存客户端的套接字
extern struct sockaddr_in oc_client_addr;	//保存此客户端的信息

void client_interaction_handler(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr, struct clientmsg *online_clients);
void print_func_list();
void send_get_all_online_clients_cmd(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr);
void send_c2c_chat_msg(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr, struct clientmsg *des_client_info,
    char *msg_buf);
void send_file_cmd_content(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr, struct clientmsg *des_client_info,
    char *abs_file_path);
void *recv_servermsg(void *arg);
void recv_clients_info(int *oc_client_sock_p,struct clientmsg * online_clients_ll, char msg_buf[SYNC_MSG_SIZE]);
void c2c_chat_recv_handler(char rbuf[SYNC_MSG_SIZE]);
int recv_file_handler(int self_sock, char msg[SYNC_MSG_SIZE]);

//传入客户端套接字指针，在重建接收线程时使用
void* init_open_chat_client(void * arg);


#endif