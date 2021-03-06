#include"unp.h"

static int read_cnt;	//记录当前缓冲区数据长度
static char *read_ptr;	//当前读取指针
static char read_buf[MAXLINE];	//缓冲区
/*
ssize_t my_read(int fd, char* ptr);
参数说明：
	fd：文件描述符
	ptr：当前读取返回的字符
返回值：
	1：正常读取
	0：无数据读
	-1：读取出错
*/
ssize_t my_read(int fd, char* ptr){
	if(read_cnt <= 0){
	again:
		if( (read_cnt = read(fd, read_buf, MAXLINE)) < 0 ){
			//read返回-1，表示读取失败，判断是否因为信号中断导致的临时失败
			if(errno == EINTR)
				goto again;
			return -1;
		}
		else if( read_cnt == 0 ){
			return 0;
		}
		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;
	return 1;
}

/*
ssize_t readline(int fd, char* vptr, size_t maxlen);
参数说明：
	fd：文件描述符
	vptr：读取数据的缓冲区
	maxlen：数据最大长度
返回值：
	n>0：读取的数据长度。
	-1：读取出错
*/
ssize_t readline(int fd, void* vptr, size_t maxlen){
	ssize_t n, rc;
	char c, *ptr;

	ptr = (char*)vptr;
	for(n=1; n<maxlen; n++){
		if( (rc = my_read(fd, &c)) == 1 ){
			*ptr++ = c;
			if(c == '\n')
				break;
		}
		else if(rc == 0){
			*ptr = 0;
			return n-1;
		}
		else{
			return -1;
		}
	}

	*ptr = 0;
	return n;
}

ssize_t
Readline(int fd, void* ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readn error");
	return(n);
}

ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = (char*)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break;				/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return(n - nleft);		/* return >= 0 */
}
/* end readn */

ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = readn(fd, ptr, nbytes)) < 0)
		err_sys("readn error");
	return(n);
}