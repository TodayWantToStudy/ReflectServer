#include"unp.h"

void str_client(FILE*, int);

const int MAXLINE = 1024;
const int SETVER_PORT = 8890;
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
	server_addr.sin_port = htons(SETVER_PORT);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int ret = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	assert(ret >= 0);

	str_client(stdin, sockfd);
	return 0;
}

void str_client(FILE* fp, int sockfd){
	size_t rc;
	char sendline[MAXLINE], recvline[MAXLINE];
	while(fgets(sendline, MAXLINE, fp) != NULL){
		Writen(sockfd,sendline, strlen(sendline));
		if(readline(sockfd, recvline, MAXLINE) < 0){
			printf("readline error");
			printf("errno %d", errno);
			exit(1);
		}
		fputs(recvline, stdout);
	}
}
