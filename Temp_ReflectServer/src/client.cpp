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
	int maxfdpl;
	int nready;
	fd_set rset;
	char sendline[MAXLINE], recvline[MAXLINE];
	
	FD_ZERO(&rset);
	for( ; ; ){
		FD_SET(sockfd, &rset);
		FD_SET(fileno(fp), &rset);
		maxfdpl = max(fileno(fp), sockfd) + 1;
		nready = select(maxfdpl, &rset, NULL, NULL, NULL);
		if(nready < 0)
			err_sys("select error");

		if(FD_ISSET(sockfd, &rset)){
			if(Readline(sockfd, recvline, MAXLINE) == 0)
				err_sys("str_client: server terminated prematurely");
			fputs(recvline, stdout);
		}
		if(FD_ISSET(fileno(fp), &rset)){
			if(fgets(sendline, MAXLINE, fp) == NULL)
				return;
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
