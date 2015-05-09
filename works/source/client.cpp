#include <cstdio>  
#include <cstdlib>  
#include <cerrno>  
#include <cstring>  
#include <netdb.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h> 
#include <unistd.h>
#define SERVPORT 3333  
#define MAXDATASIZE 100	/*每次最大数据传输量 */  

using namespace std;

int main(int argc, char *argv[])  
{  
	int sock_fd, recvbytes;  
	char buf[MAXDATASIZE];  
	struct hostent *host;  
	struct sockaddr_in serv_addr;

	if(argc< 2) {  
		fprintf(stderr,"Please enter the server's hostname!\n");  
		exit(1);  
	}  
	if((host=gethostbyname(argv[1])) == NULL) {  
		herror("gethostbyname出错！");  
		exit(1);  
	}  
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
		perror("socket创建出错！");  
		exit(1);  
	}  
	serv_addr.sin_family=AF_INET;  
	serv_addr.sin_port=htons(SERVPORT);  
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);  
	bzero(&(serv_addr.sin_zero),8);  
	if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {  
		perror("connect出错！");  
		exit(1);  
	}  
	if((recvbytes=recv(sock_fd, buf, MAXDATASIZE, 0)) == -1) {  
		perror("recv出错！");  
		exit(1);  
	}  
	buf[recvbytes] = '\0';  
	printf("Received: %s",buf);  
	close(sock_fd);  
	return 0;
}  