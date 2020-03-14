#include"unp.h"
void sig_chld(int signo);
void str_server(int);

int main(int argc, char** argv){
	int listenfd;
	struct sockaddr_in server_address;

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
	Signal(SIGCHLD, sig_chld);


	int connfd;
	pid_t childpid;
	while(true){
		//4.accept() :调用accept函数（阻塞），等待新的连接。
		struct sockaddr_in client_address;
		socklen_t client_addrlength = sizeof(client_address);
		connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
		if(connfd < 0){
			if(errno == EINTR)
				continue;
			else
				err_sys("accept error");
		}

		childpid = fork();
		if(fork < 0){
			err_sys("fork error\n");
		}
		else if(childpid == 0){
			close(listenfd);
			str_server(connfd);
			return 0;
		}
		//7.close()
		close(connfd);
	}
}
void sig_chld(int signo){
	pid_t pid;
	int stat;
	//waitpid正常返回，返回收集到的子进程id > 0
	//waitpid在循环中调用，保证了当最后一个SIGCHLD到达时，总能回收所有子进程。
	while(pid = waitpid(-1, &stat, WNOHANG) > 0){
		printf("child %d terminated\n", pid);
	}
	return;
}

void str_server(int connfd){
	size_t n;
	char buf[MAXLINE];
again:
	//5.read()
	while( (n = read(connfd, buf, MAXLINE)) > 0 ){
		//6.write()
		Writen(connfd, buf, n);
	}

	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		err_sys("str_echo: read error");
}

