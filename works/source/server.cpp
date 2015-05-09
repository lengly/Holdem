#include <cstdio>  
#include <cstdlib>  
#include <cerrno>  
#include <cstring>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/wait.h> 
#include <arpa/inet.h>
#include <unistd.h>
#define SERVPORT 3333	/*服务器监听端口号 */  
#define BACKLOG 10	/* 最大同时连接请求数 */  
#define MAXDATASIZE 1000

using namespace std;

int main()  
{  
	int sock_fd,client_fd;	/*sock_fd：监听socket；client_fd：数据传输socket */  
	int sin_size, recvbytes;  
	char buf[MAXDATASIZE];
	struct sockaddr_in my_addr;	/* 本机地址信息 */  
	struct sockaddr_in remote_addr;	/* 客户端地址信息 */  
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
		perror("socket创建出错！");  
		exit(1);  
	}  
	my_addr.sin_family=AF_INET;  
	my_addr.sin_port=htons(SERVPORT);  
	my_addr.sin_addr.s_addr = INADDR_ANY;  
	bzero(&(my_addr.sin_zero),8);  
	if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {  
		perror("bind出错！");  
		exit(1);  
	}  
	if(listen(sock_fd, BACKLOG) == -1) {  
		perror("listen出错！");  
		exit(1);  
	}  
	while(1) {  
		sin_size = sizeof(struct sockaddr_in);  
		if((client_fd = accept(sock_fd, (struct sockaddr *)&remote_addr, (socklen_t*)&sin_size)) == -1) {  
			perror("accept出错");  
			continue;  
		}  
		if((recvbytes=recv(client_fd, buf, MAXDATASIZE, 0)) == -1) {  
			perror("recv出错！");  
			exit(1);  
		}  
		buf[recvbytes] = '\0';  
		printf("Received: %s",buf); 
		printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));  
		if(!fork()) {	/* 子进程代码段 */  
			if(send(client_fd, "Hello, you are connected!\n", 26, 0) == -1) {  
				perror("send出错！");  
			}  
			close(client_fd);  
			exit(0);	
		}  
		close(client_fd);  
	}  
	return 0;
}   