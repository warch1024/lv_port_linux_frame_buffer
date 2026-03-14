#include "open_chat_client.h"  //我自定义的头文件，里面包含了所有常用的头文件
//开发板作为tcp客户端
/*
WARNING: 连续给开发板发送消息，每个send之间要间隔5ms，否则开发板会报错！！！
*/   
static pthread_t oc_recv_thread_id;	//接收消息线程的id
static pthread_attr_t oc_recv_thread_attr;//线程属性变量
int oc_client_sock;

struct clientmsg *oc_online_client_ll = NULL; //全局变量，指向存储的所有连接到此服务器的客户端的信息
struct sockaddr_in oc_client_addr;	//保存此客户端的信息
//函数声明


  
///////////////////////openchat
//打印交功能选择互列表
void print_func_list(){
	printf("****\t1 获取所有在线客户端\t****\n");
	printf("****\t2 单独聊天\t\t****\n");
	printf("****\t3 发送文件\t\t****\n");
	printf("****\t4 发送表情包\t\t****\n");
}

//发送4类消息：
//1.获取所有在线客户端信息：发送消息格式：get$all$online$clients#本机ip@本机port
//							单个客户端信息：client#1.1.1.1@10000
//								。。。
//2.单聊消息：c2c$chat#源ip@源port@msg@目的ip@目的port
//3.发送文件：send$file#源ip@源port@文件名@文件大小@目的ip@目的port
//						文件数据：文件数据
//						。。。
//4.接收表情包消息：send$emoticon#源ip@源port@表情文件名@表情大小
//						表情包数据：文件数据
//						。。。
// 客户端交互服务函数
void client_interaction_handler(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr, struct clientmsg *online_clients_ll){
	int ret;
	char select_buf[10];
	
	while(1){
		// clear_socket_buffer(*oc_client_sock_p);	//先清空缓存
		// print_func_list();	//打印菜单	1获取所有在线client，2单独和某client聊天，3发文件 ，4发表情包（图片）
		
		// bzero(select_buf,sizeof(select_buf));
		// fgets(select_buf, sizeof(select_buf), stdin);	//获取一行输入
		// select_buf[strcspn(select_buf, "\n")] = '\0';  // 去除末尾的换行符
		// //debug
		// printf("debug——4个功能选择完成\n");
		// if(strcmp(select_buf, "1")== 0){	//获取在线客户端
		// 	send_get_all_online_clients_cmd(oc_client_sock_p, oc_client_addr);
		// }
		// else if(strcmp(select_buf, "2")== 0){	//单独聊天
		// 	// c2c_chat_send_handler(oc_client_sock_p, online_clients_ll);
		// }
		// else if(strcmp(select_buf, "3")== 0){	//发文件
		// 	// send_file_handler(oc_client_sock_p, online_clients_ll);	//客户端给客户端发送文件
		// }
		// else if(strcmp(select_buf, "4")== 0){	//发表情包
	
		// }
	}

}
//4类功能命令发送函数：
//1 发送获取所有在线客户端命令
void send_get_all_online_clients_cmd(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr){
	char sbuf[SYNC_MSG_SIZE] = {0};
	//get$all$online$clients#本机ip@本机port
	char source_ip[INET_ADDRSTRLEN];
	sprintf(sbuf, "get$all$online$clients#%s@%hu",
		inet_ntop(AF_INET, &oc_client_addr->sin_addr, source_ip, sizeof(source_ip)), ntohs(oc_client_addr->sin_port));	//get$all$online$clients#本机ip@本机port
	send(*oc_client_sock_p, sbuf, sizeof(sbuf), 0);
	printf("获取在线客户端发送成功%s\n",sbuf);
}
//2 发送单独聊天消息
void send_c2c_chat_msg(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr, struct clientmsg *des_client_info,
	 char *msg_buf){
	if(oc_client_sock_p == NULL){
		printf("错误：本机套接字不存在\n");
		return;
	}
	if(oc_client_addr == NULL){
		printf("错误：本机地址不存在\n");
		return;
	}
	if(des_client_info == NULL){
		printf("错误：目标客户端不存在\n");
		return;
	}
	if(msg_buf == NULL){
		printf("错误：消息为空\n");
		return;
	}
	char sbuf[SYNC_MSG_SIZE] = {0};
	//c2c$chat#源ip@源port@msg@目的ip@目的port
	char source_ip[INET_ADDRSTRLEN];
	sprintf(sbuf, "c2c$chat#%s@%hu@%s@%s@%hu",
		inet_ntop(AF_INET, &oc_client_addr->sin_addr, source_ip, sizeof(source_ip)), ntohs(oc_client_addr->sin_port),
		 msg_buf, des_client_info->ip, des_client_info->port);	//c2c$chat#源ip@源port@msg@目的ip@目的port
	send(*oc_client_sock_p, sbuf, sizeof(sbuf), 0);
	printf("单独聊天发送成功%s\n",sbuf);
}
//3 发送发文件命令
void send_file_cmd_content(int *oc_client_sock_p, struct sockaddr_in * oc_client_addr, struct clientmsg *des_client_info,
	 char *abs_file_path){
		 //send$file#源ip@源port@文件名@文件大小@目的ip@目的port
	struct stat file_stat;

	if(stat(abs_file_path, &file_stat) == -1) {
		perror("stat");
        return;
    }
	//发送文件命令
	long file_size = 0;
	char cmd_sbuf[SYNC_MSG_SIZE] = {0};
	file_size = file_stat.st_size;
	char source_ip[INET_ADDRSTRLEN];
	char file_name[64] = {0};
	strcpy(file_name, get_filename_from_path(abs_file_path));
	//命令内容：send$file#源ip@源port@文件名@文件大小@目的ip@目的port
	sprintf(cmd_sbuf, "send$file#%s@%hu@%s@%ld@%s@%hu",
		inet_ntop(AF_INET, &oc_client_addr->sin_addr, source_ip, sizeof(source_ip)), ntohs(oc_client_addr->sin_port),
		 file_name, file_size, des_client_info->ip, des_client_info->port);	//send$file#源ip@源port@文件名@文件大小@目的ip@目的port
	send(*oc_client_sock_p, cmd_sbuf, sizeof(cmd_sbuf), 0);
	printf("发送发文件命令%s\n",cmd_sbuf);
	//发送文件数据
	FILE *file = fopen(abs_file_path, "rb");	//以二进制只读模式打开文件
	if(file == NULL) {
        perror("fopen");
        return;
    }
	//发送文件数据
    char file_buf[SYNC_MSG_SIZE];
	//每次读取SYNC_MSG_SIZE字节数据发送，发送数据大小固定为SYNC_MSG_SIZE、
	int send_k = file_size / sizeof(file_buf) + 1;
	while(send_k-- ){
		bzero(file_buf, sizeof(file_buf));	//清空缓存
		int ret = fread(file_buf, 1, sizeof(file_buf), file);	//每次读取SYNC_MSG_SIZE字节，读到文件末尾时返回0
		if(ret == 0){
			perror("fread");
			break;
		}
		send(*oc_client_sock_p, file_buf, sizeof(file_buf), 0);	//发送SYNC_MSG_SIZE字节数据
		usleep(5);	//等待10ms，确保数据发送完成
		printf("发送文件数据，已发送-%d次\n",send_k);
	}
    fclose(file);
	printf("发送文件数据发送成功，文件大小%ld字节\n",file_size);
}

//接收4类消息：
//传输客户端消息时独占接收线程，不允许其他线程接收
//1.获取所有在线客户端信息：接收消息格式：clients$info#10@10240
//						单个客户端信息：client#1.1.1.1@10000
//							。。。
//2.单聊消息：c2c$chat#源ip@源port@msg
//传输文件时独占接收线程，不允许其他线程接收
//3.接收文件消息：send$file#源ip@源port@文件名@文件大小
//						文件数据：文件数据
//						。。。
//传输表情包时独占接收线程，不允许其他线程接收
//4.接收表情包消息：send$emoticon#源ip@源port@表情文件名@表情大小
//						表情包数据
//						。。。

//线程任务函数：接收服务器发送过来的所有消息
void *recv_servermsg(void *arg)
{
	char rbuf[SYNC_MSG_SIZE];	//最高支持发送1k
	int ret;
	int client_sock = *(int *)arg;	//传入本机的sock
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);	//设置立即取消此线程
	
	while(1){//debug
		printf("debug: 重置接收缓存\n");
		//接收服务器发送过来的信息
		bzero(rbuf,sizeof(rbuf)); 
		ret=recv(client_sock,rbuf,sizeof(rbuf),0);
		//debug
		printf("debug: 接收服务器消息\n");
		if(ret==0){
			printf("对方(服务器端)断开连接了!\n");	
			exit(0);
		}
		else if(ret< 0){
			printf("接收服务器消息失败\n");
			continue;
		}
		else if(ret < SYNC_MSG_SIZE){
			printf("接收服务器消息不完整\n");
			continue;
		}
		else if(strcmp(rbuf,"quit")==0){
			exit(0);	//结束进程
		}
		else{
			//实际工作：根据消息头判断消息类型
			char * head_cmd_buf = strtok(rbuf, "#"), *msg_buf_p = strtok(NULL, "#");//获取消息头和消息体
			char msg_buf[SYNC_MSG_SIZE] = {0};	//消息体缓存
			bzero(msg_buf, sizeof(msg_buf));
			if(msg_buf_p == NULL){	//发生错误
				printf("bug 消息格式错误%s\n",rbuf);	
				continue;//重新接收
			}
			else{	//通信正常
				strcpy(msg_buf, msg_buf_p);	//保存转发的消息
			}
			//1.接收所有在线客户端信息：接收消息格式：clients$info#10@10240
			if(strcmp(head_cmd_buf, "clients$info")== 0){	//获取所有在线客户端
				//消息格式：10@10240
				recv_clients_info((int *)arg, oc_online_client_ll, msg_buf);
			}
			//2.接收单独聊天消息：接收消息格式：c2c$chat#源ip@源port@msg
			else if(strcmp(head_cmd_buf, "c2c$chat")== 0){	//单独聊天
				printf("接收到单独聊天消息：%s\n", msg_buf);
				//消息格式：源ip@源port@msg
				c2c_chat_recv_handler(msg_buf);	//接收转发来的消息
			}
			else if(strcmp(head_cmd_buf, "send$file")== 0){	//接收文件
				
				recv_file_handler(client_sock, msg_buf);	//接收文件
	
			}
			else if(strcmp(head_cmd_buf, "emoticon")== 0){	//发文件
	
			}
			else{
				printf("未知消息格式：%s\n", rbuf);
			}
			//debug
			printf("debug: 接收服务器消息完成111\n");
			clear_socket_buffer(oc_client_sock);
		}
	}
}
//接收4类功能函数
//1 接收所有在线客户端信息
void recv_clients_info(int *oc_client_sock_p,struct clientmsg * online_clients_ll, char msg_buf[SYNC_MSG_SIZE]){
	clear_socket_buffer(*oc_client_sock_p);
	//msg_buf命令格式：10@10240, [clients$info#10@10240]
	char *client_num_str = strtok(msg_buf, "@");
	int client_num = atoi(client_num_str);
	printf("当前在线客户端数量：%d\n", client_num);
	//接收每个客户端信息,格式：client#1.1.1.1@10000
	char rbuf[SYNC_MSG_SIZE] = {0};
	empty_list(online_clients_ll);	//先清空旧的链表
	int ret = 0;
	char *tmp = NULL, **pos = &tmp;
	for(int i = 0; i < client_num; i++){
		bzero(rbuf, sizeof(rbuf));
		ret = recv(*oc_client_sock_p, rbuf, sizeof(rbuf), 0);
		if(ret == 0){
			printf("对方(服务器端)断开连接,客户信息接收失败\n");	
			exit(0);
		}
		//保存客户端信息
		strtok_r(rbuf, "#", pos);
		struct clientmsg *client_info = malloc(sizeof(struct clientmsg));
		bzero(client_info, sizeof(struct clientmsg));
		strcpy(client_info->ip, strtok_r(NULL, "@", pos));
		client_info->port = (uint16_t)atoi(strtok_r(NULL, "@", pos));
		//将客户端信息添加到链表中
		insert_tail(client_info, online_clients_ll);
		printf("客户端信息：%s@%d\n", client_info->ip, client_info->port);
	}
	printf("debug——接收完成s发来的在线c打印链表\n");
	print_all_client(online_clients_ll);	//打印所有的已连接客户端的信息
	clear_socket_buffer(*oc_client_sock_p);
	//debug
	printf("debug: 接收所有在线客户端信息完成\n");
}
//2 接收单独聊天发来的信息
void c2c_chat_recv_handler(char rbuf[SYNC_MSG_SIZE]){
	//消息格式：192.168.11.89@10000@qqqqq@192.168.11.88@10000
	char *source_ip_s = strtok(rbuf, "@");	//得到源ip
	char *source_port_s = strtok(NULL, "@");	//得到源port
	char *msg_s = strtok(NULL, "@");	//得到消息本体
	char *des_ip_s = strtok(NULL, "@");	//获得目标ip
	char *des_port_s = strtok(NULL, "@");	//获得目标端口
	printf("%s@%s发来的消息：%s\n", source_ip_s, source_port_s, msg_s);
	//将消息显示在主屏幕
	oc_display_recv_send_message(msg_s, open_chat_main_interface_o, "recv", NULL);
}
//3 接收文件的处理函数
int recv_file_handler(int self_sock, char msg[SYNC_MSG_SIZE]){	//接收文件的处理函数
	//msg的内容是源ip@源port@文件名@文件大小@目的ip@目的port
	clear_socket_buffer(self_sock);	//清空接收缓存套接字
	//debug
	printf("debug 客户端要接受的文件信息：%s\n",msg);
	char file_path[200] = {0};	//要保存的文件的存放路径
	char *source_ip_s = strtok(msg, "@");	//获取源ip
	char *source_port_s = strtok(NULL, "@");	//获取源port
	char *file_name_s = strtok(NULL, "@");	//获取文件名
	char *file_size_s = strtok(NULL, "@");	//获取文件大小

	long file_size = strtol(file_size_s, NULL, 0);	//获取要接受多少kb数据
	bzero(file_path, sizeof(file_path));
	//获取当前文件的绝对路径
	char abs_path[200] = {0};
	bzero(abs_path, sizeof(abs_path));
	getcwd(abs_path, sizeof(abs_path));
	//debug
	// abs_path[0] = "/home/devvean/tmp";
	sprintf(file_path, "%s/%s",abs_path, file_name_s);	//组合文件存放路径
	//debug
	printf("debug 客户端要接受的字节数：%ld\n",file_size);
	//debug
	printf("debug 客户端要存储的文件路径：%s\n",file_path);
	int fd = open(file_path, O_CREAT|O_TRUNC|O_WRONLY, 0777);	//打开文件并写入
	//debug
	printf("debug 客户端打开的文件描述符：%d\n",fd);
	//获取目标客户端的信息
	//写入文件
	char recv_buf[SYNC_MSG_SIZE];
	int write_k = file_size / sizeof(recv_buf);
	while(write_k--){	//转发收到的文件数据
		bzero(recv_buf, sizeof(recv_buf));
		int ret = recv(self_sock, recv_buf,sizeof(recv_buf),0);	//接收数据
		//debug
		printf("debug rf客户端接收到：%d字节\n",ret);
		int size = write(fd, recv_buf, sizeof(recv_buf));	//写入文件
		//debug
		printf("debug 客户端写入：%d字节\n",size);
	}
	//最后一次写入数据
	{
		bzero(recv_buf, sizeof(recv_buf));
		int ret = recv(self_sock, recv_buf,sizeof(recv_buf),0);	//接收数据
		//debug
		printf("debug last客户端接收到：%d字节\n",ret);
		write(fd, recv_buf, file_size % sizeof(recv_buf));	//写入最后一次数据
		//debug
		printf("debug last客户端写入：%d字节\n",file_size % sizeof(recv_buf));
	}
	close(fd);
	printf("debug 文件写入完成\n");
	usleep(1000);//等待1ms，确保文件写入完成
	clear_socket_buffer(self_sock);	//清空接收缓存套接字
	//将发送成功的提示信息显示在聊天界面
    char send_success_msg[128] = {0};
    bzero(send_success_msg, sizeof(send_success_msg));
	char file_name[64] = {0};	//文件名
	bzero(file_name, sizeof(file_name));
	strcpy(file_name, file_name_s);	//获取文件名
	char extension[16] = {0};
	bzero(extension, sizeof(extension));
	strcpy(extension, strrchr(file_name, '.'));	//获取文件扩展名
	if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".png") == 0){	//如果是图片文件
		sprintf(send_success_msg, "接收表情包%s成功！", file_name);
		if(oc_main_interface_to_client != NULL && lv_obj_is_valid(oc_main_interface_to_client)){
			oc_display_recv_send_message(send_success_msg, oc_main_interface_to_client, "recv", file_path);
		}
    }else{	//如果是其他文件
        sprintf(send_success_msg, "接收文件%s成功！", file_name);
		if(oc_main_interface_to_client != NULL && lv_obj_is_valid(oc_main_interface_to_client)){
			oc_display_recv_send_message(send_success_msg, oc_main_interface_to_client, "recv", NULL);
		}
    }
    printf("debug 发送成功的提示信息： %s\n", send_success_msg);
}
//客户端通信功能初始化线程回调任务函数
void* init_open_chat_client(void * arg){	
	int ret;
	//debug
	printf("debug: T-init openchat_client\n");
	
	//定义ipv4地址结构体变量存放客户端需要绑定的ip和端口号
	
	bzero(&oc_client_addr,sizeof(oc_client_addr));
	oc_client_addr.sin_family=AF_INET;  //地址协议，IPV4
	oc_client_addr.sin_addr.s_addr=htonl(INADDR_ANY);    //绑定客户端自己的ip地址
	oc_client_addr.sin_port=htons(10000);  //端口设为0，由系统自动分配可用端口
	
	//定义ipv4地址结构体变量存放服务器的ip和端口号
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;  //地址协议，IPV4
	serveraddr.sin_addr.s_addr=inet_addr("192.168.11.88");    //服务器的ip地址
	serveraddr.sin_port=htons(30000);  //服务器的端口号
	
	//创建tcp套接字--》买手机
	oc_client_sock=socket(AF_INET,SOCK_STREAM,0);
	if(oc_client_sock==-1)
	{
		perror("买手机失败(创建tcp套接字)失败!\n");
		return -1;
	}
	
	//设置端口复用
	int on=1; //非0即可,是个开关，开启端口复用功能
	setsockopt(oc_client_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//绑定ip地址和端口号
	ret=bind(oc_client_sock,(struct sockaddr *)&oc_client_addr,sizeof(oc_client_addr));
	if(ret==-1)
	{
		perror("绑定ip和端口号失败了!\n");
		return -1;
	}
	
	//连接服务器
	ret=connect(oc_client_sock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(ret==-1)
	{
		perror("连接服务器失败了!\n");
		return -1;
	}
	//获取本机的实际ip，写入到client_addr,应该在连接服务器后
	socklen_t addr_len = sizeof(oc_client_addr);
	getsockname(oc_client_sock, (struct sockaddr*)&oc_client_addr, &addr_len);
	//初始化在线成员的存储链表
	oc_online_client_ll=client_init();
	
	//创建线程用于接收信息
	
	pthread_attr_init(&oc_recv_thread_attr);
	pthread_attr_setdetachstate(&oc_recv_thread_attr,PTHREAD_CREATE_DETACHED); //创建的所有线程可分离，无需手动回收
	//创建线程用于接收信息
	pthread_create(&oc_recv_thread_id, &oc_recv_thread_attr, recv_servermsg, &oc_client_sock);
	sleep(1);//等待接收线程启动
	send_get_all_online_clients_cmd(&oc_client_sock, &oc_client_addr);//同步所有在线客户端信息
	//运行主功能，须保持空转以维持线程运行
	client_interaction_handler(&oc_client_sock, &oc_client_addr, oc_online_client_ll);	//打开交互界面
	//挂机
	close(oc_client_sock);
	return 0;
}