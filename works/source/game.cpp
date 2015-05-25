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
#include <arpa/inet.h>
#include <sstream>
#include "game.h"
#include "player.h"
#define MAXDATASIZE 1000

using namespace std;

int m_socket_id = -1;

int main(int argc, char *argv[]) {
	int recvbytes, sendbytes, len;
	string pname = "lengly";
	char buf[MAXDATASIZE];
	stringstream ss;
	string s;
	Player player;
	string s_color,s_point;
	int count = 0;

	if (argc != 6) {
		printf("Input format error\n");
		return 0;
	}
	// 提取命令行參數
	in_addr_t server_ip = inet_addr(argv[1]);
	in_port_t server_port = atoi(argv[2]);
	in_addr_t my_ip = inet_addr(argv[3]);
	in_port_t my_port = atoi(argv[4]);
	int my_id = atoi(argv[5]);
	string my_name = "lengly";

	// 建立socket连接
	if ((m_socket_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		return 0;
	}

	// 綁定自己的ip
	struct sockaddr_in my_addr;
	my_addr.sin_addr.s_addr = my_ip;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(my_port);

	long flag = 1;
	setsockopt(m_socket_id, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));

	if (bind(m_socket_id, (sockaddr*)&my_addr, sizeof(sockaddr)) < 0) {
		perror("bind error");
		return 0;
	}

	// 连接服务器
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = server_ip;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	printf("Try to connect server(%s:%u)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
	// while(0 != connect(m_socket_id, (struct sockaddr *)&server_addr, sizeof(sockaddr))) {
	// 	usleep(100*1000);
	// }
	if(connect(m_socket_id, (struct sockaddr *)&server_addr, sizeof(sockaddr))) {  
		perror("connect出错！");  
		exit(1);  
	}  
	// printf("Connect server success(%s:%u)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

	// 注册
	memset(buf,0,sizeof(buf));
	sprintf(buf, "reg: %d %s \n", my_id, my_name.c_str());
	len = strlen(buf);
	if ((sendbytes = send(m_socket_id, buf, len, 0)) != len) {
		perror("Send error");
		close(m_socket_id);
		return 0;
	}
	perror("Send success");
	// 开始比赛
	while (true) {
		recvbytes=recv(m_socket_id, buf, MAXDATASIZE, 0);
		if (recvbytes <= 0) continue;
		// 开始解析
		ss.clear();
		ss.str(string(buf,recvbytes));
		ss >> s;
		if (s == "seat/") {
			count++;
			// TODO maxInitialMoney
			player.startRound();

		} else if (s == "game-over") {
			break;
		} else if (s == "blind/") {
			while (ss >> s, s != "/blind") {
				if (my_id == atoi(s.c_str())) {
					ss >> s;
					player.bet(atoi(s.c_str()));
				}
			}
		} else if (s == "hold/") {
			player.status(HOLD);
			for(int i=0;i<2;i++) {
				ss >> s_color >> s_point;
				player.addHold(Card(s_color, s_point));
			}
		} else if (s == "inquire/") {
			// TODO response
			sprintf(buf, "call");
			len = strlen(buf);
			if ((sendbytes = send(m_socket_id, buf, len, 0)) != len) {
				perror("send error");
				close(m_socket_id);
				return 0;
			}
		} else if (s == "flop/") {
			player.status(FLOP);
			for(int i=0;i<3;i++) {
				ss >> s_color >> s_point;
				player.addCard(Card(s_color, s_point));
			}

		} else if (s == "turn/") {
			player.status(TURN);
			ss >> s_color >> s_point;
			player.addCard(Card(s_color, s_point));
		} else if (s == "river/") {
			player.status(RIVER);
			ss >> s_color >> s_point;
			player.addCard(Card(s_color, s_point));
		} else if (s == "showdown/") {

		} else if (s == "pot-win/") {

		}

	}


	close(m_socket_id);  
	return 0;
}