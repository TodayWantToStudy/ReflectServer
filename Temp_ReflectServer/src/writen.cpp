#include"unp.h"

//输出当前errno对应的错误信息，未完成。
void err_sys(const char* fmt){
	printf("%s\n", fmt);
	printf("errno:%d\n", errno);
	exit(1);
}

/*
ssize_t write(int fd,const void*buf,size_t count);
参数说明：
	fd:文件描述符
	buf:需要写入的字符串
	count:每次写入的字节数
*/

//write函数的安全调用，写入固定长度的信息
ssize_t writen(int fd, const void* vptr, size_t n){
	size_t nleft;	//剩余需要写入字节数
	size_t nwritten;//这次写入的字节数
	const char* ptr;//指向当前需要写入的缓冲区

	ptr = (const char*)(vptr);
	nleft = n;
	while(nleft > 0){
		//nwritten = 这次调用write函数写入的字节数
		if( (nwritten = write(fd, ptr, nleft)) <= 0 ){
			//如果write返回值为-1，并且errno == EINTR，表示缓冲区满，无法一次写入
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			//如果write返回值等于0，出错
			else
				return -1;
		}
		nleft -= nwritten;	//更新剩余字节数
		ptr += nwritten;	//移动指针，指向缓冲区需要写入的下一部分。
	}
	return n;	
}

//written函数的包裹函数
void Writen(int fd, const void* ptr, size_t nbytes){
	if(writen(fd, ptr, nbytes) != nbytes)
		err_sys("writen error");
}



