#include<stdio.h>	//C语言标准IO库
#include<stdlib.h>	//C语言标准库，包含malloc、free、exit等
#include<string.h>	//C语言提供有关字符数组的函数库，包括strlen、strcmp、strcpy等

#include<unistd.h>	//C语言提供 POSIX 操作系统 API 的访问功能的头文件
#include<errno.h>	//C语言提供的错误码errno相关的宏
#include<assert.h>	//C语言提供有关防御式编程的库，assert(int exp)本质上是宏定义。

#include<sys/socket.h>	//定义创建套接字的系列函数
#include<netinet/in.h>	//定义网际套接字地址结构sockaddr_in，以及htonl、htons等字节序函数
#include<arpa/inet.h>

const int MAXLINE = 1024;
const int SERVER_PORT = 8890;

void err_sys(const char *fmt);	//输出当前errno对应的错误信息，未完成。

ssize_t writen(int fd, const void *vptr, size_t nbytes);	//write函数的安全调用，写入固定长度的信息
void Writen(int fd, const void *ptr, size_t nbytes);	//writen的包裹函数，未正确写入固定长度信息则报错


ssize_t my_read(int fd, char* ptr);						//从描述符中读取数据，存放在缓冲区中，逐个返回
ssize_t readline(int fd, char* vptr, size_t maxlen);	//通过my_read()逐个读取数据，直到长度超出或者遇到换行
