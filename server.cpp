#include"unp.h"

void str_server(int);

const int MAXLINE = 1024;
const int SERVER_PORT = 8890;
int main(int argc, char** argv){
	int listenfd;
	struct sockaddr_in server_address;

	//填写套接字地址结构体
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(SERVER_PORT);

	//创建套接字，指定期望的通信协议类型
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	//绑定本地协议地址，指定套接字的本地协议地址。
	int ret = bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address));
	assert(ret != -1);
	//将主动套接字转换为被动套接字
	ret = listen(listenfd, 5);
	assert(ret != -1);

	int connfd;
	pid_t childpid;
	while(true){
		//调用accept函数（阻塞），等待新的连接。
		struct sockaddr_in client_address;
		socklen_t client_addrlength = sizeof(client_address);
		connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
		if(connfd < 0){
			printf("accept error\n");
			return 1;
		}

		childpid = fork();
		if(fork < 0){
			printf("fork error\n");
			return 2;
		}
		else if(childpid == 0){
			close(listenfd);
			str_server(connfd);
			return 0;
		}
		close(connfd);
	}
}

void str_server(int connfd){
	size_t n;
	char buf[MAXLINE];
again:
	while( (n = read(connfd, buf, MAXLINE)) > 0 ){
		Writen(connfd, buf, n);
	}

	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		err_sys("str_echo: read error");
}
