#include"unp.h"
const int INFTIM = -1;
const int OPEN_MAX = 65535;
int main(int argc, char** argv){
	char buf[MAXLINE];
	struct pollfd client[OPEN_MAX];
	int i, n;
	int listenfd, connfd, sockfd;
	struct sockaddr_in server_address, client_address;

	//填写套接字地址结构体
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(SERVER_PORT);

	//1.socket() :创建套接字，指定期望的通信协议类型
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	//2.bind() :绑定本地协议地址，指定套接字的本地协议地址。
	int ret = bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address));
	assert(ret != -1);
	//3.listen() :将主动套接字转换为被动套接字
	ret = listen(listenfd, 5);
	assert(ret != -1);

	//初始化client数组
	client[0].fd = listenfd;		//第一项用于监听套接字
	client[0].events = POLLRDNORM;	//测试POLLRDNORM事件
	for(i=1; i<OPEN_MAX; i++){
		client[i].fd = -1;
	}

	int maxi = 0;
	for( ; ; ){
		int nready = poll(client, maxi+1, INFTIM);

		//监听套接字可读，表示有新客户连接
		if(client[0].revents & POLLRDNORM){
			socklen_t client_addrlength = sizeof(client_address);
			connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
			//找到第一个可用项，记录该客户连接的描述符
			for(i=1; i<OPEN_MAX; i++){
				if(client[i].fd < 0){
					maxi = max(maxi, i);	//更新maxi
					client[i].fd = connfd;	//记录描述符
					client[i].events = POLLRDNORM;//设置POLLRDNORM事件
					break;
				}
			}
			if(i == OPEN_MAX)
				err_sys("too many clients");
			if(--nready <= 0)
				continue;
		}

		//检测是否有已连接描述符可读
		for(i=1; i<=maxi; i++){
			if((sockfd = client[i].fd) < 0)
				continue;
			//已连接描述符可读，表示客户发送消息
			if(client[i].revents & (POLLRDNORM | POLLERR)){
				n = read(sockfd, buf, MAXLINE);
				if(n > 0)
					Writen(sockfd, buf, n);
				else if(n == 0){
					close(sockfd);
					client[i].fd = -1;
				}
				if(n < 0){
					if(errno != ECONNRESET){
						err_sys("read error");
					}
					close(sockfd);
					client[i].fd = -1;								
				}
				if(--nready <= 0)
					break;
			}
		}
	}
	
}

