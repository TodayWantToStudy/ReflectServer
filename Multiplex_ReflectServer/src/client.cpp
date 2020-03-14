#include"unp.h"
void str_client(FILE*, int);

int main(int argc, char** argv){
	int sockfd;
	struct sockaddr_in server_addr;
	if(argc != 2){
		printf("usage: <exename> <IPaddress>\n");
		return 1;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(SERVER_PORT);

	//1.socket()
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//2.connect()
	int ret = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	assert(ret >= 0);

	str_client(stdin, sockfd);
	return 0;
}

void str_client(FILE* fp, int sockfd){
	fd_set rset;
	char buf[MAXLINE];
	int maxfdpl, stdineof, n;
	
	stdineof = 0;
	FD_ZERO(&rset);
	for( ; ; ){
		if(stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdpl = max(fileno(fp), sockfd) + 1;

		//一直阻塞于select，直至描述符集中的描述符就绪
		if(select(maxfdpl, &rset, NULL, NULL, NULL) <= 0){
			err_sys("select error");
		}
		//测试套接字描述符是否就绪
		if(FD_ISSET(sockfd, &rset)){
			n = read(sockfd, buf, MAXLINE);
			if(n < 0)
				err_sys("read error");
			else if(n == 0){
				if(stdineof == 1)
					return;
				else
					err_sys("str_client: server terminated prematurely");
			}

			if(writen(fileno(fp), buf, n) != n)
				err_sys("writen error");
		}
		//测试标准输入描述符是否就绪
		if(FD_ISSET(fileno(fp), &rset)){
			n = read(fileno(fp), buf, MAXLINE);
			if(n < 0)
				err_sys("read error");
			else if(n == 0){
				stdineof = 1;
				if(shutdown(sockfd, SHUT_WR) < 0)
					err_sys("shutdown error");
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			if(writen(sockfd, buf, n) != n)
				err_sys("writen error");
		}
	}
}
