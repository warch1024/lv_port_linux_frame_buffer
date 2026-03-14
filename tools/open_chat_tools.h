#ifndef __OPEN_CHAT_TOOLS_H__
#define __OPEN_CHAT_TOOLS_H__

#include"openChat/screen_objs.h"  //标准库头文件

struct clientmsg
{
	//数据域
	int sock; //目前连接成功客户端套接字
	char ip[20]; //客户端ip地址
	unsigned short port; //客户端端口号
	
	//指针域
	struct clientmsg *next;
};

extern int oc_client_sock;
extern struct clientmsg *oc_online_client_ll; //全局变量，指向存储的所有连接到此服务器的客户端的信息

void clear_socket_buffer(int sock);
struct clientmsg *client_init();
int insert_tail(struct clientmsg *node,struct clientmsg *head);
int list_delete(char ipbuf[20],unsigned short delport,struct clientmsg *head);
struct clientmsg * find_client(char ip[20], unsigned short port, struct clientmsg *head);
void print_all_client(struct clientmsg * myhead);
// void *send_msgto_client(void *arg);
int get_client_count(struct clientmsg * myhead);
void empty_list(struct clientmsg * head);
/**
 * 从文件全路径中提取文件名
 * @param path 文件全路径
 * @return 文件名（不包含路径部分）
 */
const char* get_filename_from_path(const char* path);

#endif