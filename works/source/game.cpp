#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAXDATASIZE 1000
#define pname lengly

using namespace std;

int main(int argc, char *argv[]) {
	int sock_fd, recvbytes, sendbytes, port, pid, len;
	string pname = "lengly";
	char buf[MAXDATASIZE];
	struct hostent *host;
	struct sockaddr_in serv_addr;

	if (argc < 6) {
		printf("Input format error\n");
		return 0;
	}
	// get host ip
	if ((host = gethostbyname(argv[1])) == NULL) {
		herror("gethostbyname error!");
		return 0;
	}
	// get post & pid
	port = atoi(argv[4]);
	pid = atoi(argv[5]);
	// socket
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		return 0;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr = *((struct in_addr *) host->h_addr);
	bzero(&(serv_addr.sin_zero), 8);
	if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) { 
		perror("connect error");
		close(sock_fd);
		return 0;
	}
	// 注册
	sprintf(buf, "reg: %d %s \n", pid, pname.c_str());
	len = strlen(buf);
	if ((sendbytes = send(sock_fd, buf, len, 0)) != len) {
		perror("send error");
		close(sock_fd);
		return 0;
	}

/*


	if((recvbytes=recv(sock_fd, buf, MAXDATASIZE, 0)) == -1) {  
		perror("recv出错！");  
		exit(1);  
	}  
	buf[recvbytes] = '\0';  
	printf("Received: %s",buf);  
   */
	close(sock_fd);  
	return 0;
}