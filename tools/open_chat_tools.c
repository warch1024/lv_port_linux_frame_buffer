#include"open_chat_tools.h"


void clear_socket_buffer(int sock) {
    char buffer[2048];
    int n;
    
    // 设置为非阻塞模式
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL 失败");
        return;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl 设置非阻塞模式失败");
        return;
    }

    // 循环读取直到没有数据
    while ((n = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        // 持续读取并丢弃数据
    }
    if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("recv 清空缓冲区时出错");
    }

    // 恢复原来的阻塞模式
    if (fcntl(sock, F_SETFL, flags) == -1) {
        perror("fcntl 恢复阻塞模式失败");
    }
}

//初始化链表的头结点
struct clientmsg *client_init()
{
	struct clientmsg *head=malloc(sizeof(struct clientmsg));
	head->sock = -1;
	head->next=NULL;
	return head;
}

//尾插某个节点到链表的尾部
int insert_tail(struct clientmsg *node,struct clientmsg *head)
{
	if(head && node){	//只有在客户端都断开且发送线程退出的情况下
		//找到链表的尾部
		while(head->next != NULL){
			head = head->next;
		}
		//把节点插入到尾部
		head->next = node;
		return 0;
	}
	else{
		perror("链表/节点为空");
	}
}

//删除节点
int list_delete(char ipbuf[20],unsigned short delport,struct clientmsg *head)
{
	//定义两个指针,一前一后遍历链表
	struct clientmsg *p=head->next;
	struct clientmsg *q=head;
	
	while(p!=NULL)
	{
		if(strcmp(p->ip,ipbuf)==0 && p->port==delport)	//p指向待删节点
			break;
		
		//p和q往后挪动
		p=p->next;
		q=q->next;
	}
	
	//删除p这个节点
	q->next=p->next;
	p->next=NULL;
	printf("IP：%s clientmsg node is deleted\n",p->ip);
	free(p);
}

struct clientmsg * find_client(char ip[20], unsigned short port, struct clientmsg *head){
	struct clientmsg *p = head;	//获取已连接客户端tcp信息
	while(p->next!=NULL){
		p=p->next;
		if((strcmp(p->ip,ip)==0) && (p->port==port))	//p指向要发信息的客户端
			break;
	}
	return p;
}
//打印所有的客户端ip和端口
void print_all_client(struct clientmsg * myhead){
	struct clientmsg *p=myhead;	//获取已连接客户端tcp信息
	int count = 0;
	if(myhead->next ==NULL){	//空链表直接不打印
		return ;
	}
	else{
		//system("clear");
		printf("目前在线的客户端: \n");
		p = p->next;
		while(p != NULL){
			printf("%d: %s  %hu\n", ++ count, p->ip,p->port);
			p = p->next;
		}
	}
}

int get_client_count(struct clientmsg * myhead){
	if(myhead && myhead->next){
		int count = 0;
		for(myhead = myhead->next; myhead != NULL; myhead = myhead->next){
			count ++;
		}
		return count;	//返回已连接的客户端数
	}
	return -1;
}

void empty_list(struct clientmsg * head){
	if(head && head->next){
		struct clientmsg *p = head->next;  // 从第一个有效节点开始
		struct clientmsg *q;
	
		// 遍历链表，逐个释放节点
		while(p != NULL){
			q = p->next;  // 保存下一个节点
			free(p);  // 释放当前节点
			p = q;  // 移动到下一个节点
		}
		
		// 重置头结点
		head->next = NULL;
	}
}
/**
 * 从文件全路径中提取文件名
 * @param path 文件全路径
 * @return 文件名（不包含路径部分）
 */
const char* get_filename_from_path(const char* path) {
    if (path == NULL) {
        return NULL;
    }

    const char* filename = path;
    const char* temp = path;

    // 查找最后一个 '/' 或 '\'
    while (*temp != '\0') {
        if (*temp == '/' || *temp == '\\') {
            filename = temp + 1;
        }
        temp++;
    }

    return filename;
}