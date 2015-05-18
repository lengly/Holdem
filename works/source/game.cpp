#include <iostream>
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
#include <sstream>
#include "game.h"
#include "player.h"
#define MAXDATASIZE 1000

using namespace std;

int main(int argc, char *argv[]) {
	//freopen("output","w",stdout);
	int sock_fd, recvbytes, sendbytes, port, pid, len;
	string pname = "lengly";
	char buf[MAXDATASIZE];
	struct hostent *host;
	struct sockaddr_in serv_addr;
	stringstream ss;
	string s;
	Player player;
	string s_color,s_point;

	if (argc < 6) {
		printf("Input format error\n");
		return 0;
	}
	// 解析服务器ip
	if ((host = gethostbyname(argv[1])) == NULL) {
		herror("gethostbyname error!");
		return 0;
	}
	// 解析端口和pid
	port = atoi(argv[2]);
	pid = atoi(argv[5]);
	// 建立socket连接
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
	// 开始比赛
	while (true) {
		if((recvbytes=recv(sock_fd, buf, MAXDATASIZE, MSG_WAITALL)) == -1) {  
			perror("recv出错！");  
			continue;
		} 
		if (recvbytes == 0) continue;
		// 开始解析
		ss.clear();
		ss.str(string(buf,recvbytes));
		ss >> s;
		if (s == "seat/") {
			// TODO maxInitialMoney
			player.startRound();

		} else if (s == "game-over") {
			break;
		} else if (s == "blind/") {
			while (ss >> s, s != "/blind") {
				if (pid == atoi(s.c_str())) {
					ss >> s;
					player.bet(atoi(s.c_str()));
				}
			}
		} else if (s == "hold/") {
			for(int i=0;i<2;i++) {
				ss >> s_color >> s_point;
				player.addHold(Card(s_color, s_point));
			}
		} else if (s == "inquire/") {
			// TODO response
			sprintf(buf, "call");
			len = strlen(buf);
			if ((sendbytes = send(sock_fd, buf, len, 0)) != len) {
				perror("send error");
				close(sock_fd);
				return 0;
			}
		} else if (s == "flop/") {
			for(int i=0;i<3;i++) {
				ss >> s_color >> s_point;
				player.addCard(Card(s_color, s_point));
			}

		} else if (s == "turn/") {
			ss >> s_color >> s_point;
			player.addCard(Card(s_color, s_point));
		} else if (s == "river/") {
			ss >> s_color >> s_point;
			player.addCard(Card(s_color, s_point));
		} else if (s == "showdown/") {

		} else if (s == "pot-win/") {

		}

	}


	close(sock_fd);  
	return 0;
}