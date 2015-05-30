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
#include <queue>
#define MAXDATASIZE 1000

using namespace std;

int m_socket_id = -1;
queue<string> que;
int qc = 0;
char buf[MAXDATASIZE];
int recvbytes, sendbytes, len;
Player player;
int count = 0;
string s_color,s_point;
int my_id;

void receive() {
	stringstream ss;
	string s;

	recvbytes=recv(m_socket_id, buf, MAXDATASIZE, 0);
	if (recvbytes <= 0) return;

	ss.clear();
	ss.str(string(buf,recvbytes));
	while (ss >> s) {
		que.push(s);
		if (s[0] == '/' && s != "/common") {
			qc++;
		} else if (s == "game-over") {
			qc++;
		}
	}
}

bool check_level(vector<Card> hold) {
	// AA KK QQ JJ
	if (hold[0].point == hold[1].point && hold[0].point > 10) return true;
	// AK
	if (hold[0].point + hold[1].point >= 27) return true;
	return false;
}

bool solve() {
	string s;
	int maxBet; // 当前单人下注最大金额
	int totBet; // 总下注金额
	int t_pid, t_jetton, t_money, t_now;
	int cnt_raise, cnt_call;
	int my_now_total, my_now_raise; // 我的当前已下注金额  我的当前应加注金额
	int alive; // 活着人数
	vector<vector<string> > record;
	string t_action, my_action;
	double p;
	bool flag;
	while (qc > 0) {
		qc--;
		s = que.front();
		que.pop();
		if (s == "seat/") {
			count++;
			player.startRound();
			while (s != "/seat") {
				s = que.front();
				que.pop();
				if (s == "/seat") break;
				if (s == "button:") continue;
				else if (s == "small" || s == "big") {
					s = que.front();
					que.pop();
				} else {
					t_pid = atoi(s.c_str());
					s = que.front(); que.pop();
					t_jetton = atoi(s.c_str());
					s = que.front(); que.pop();
					t_money = atoi(s.c_str());
					if (t_pid == my_id) {
						player.setMoney(t_money, t_jetton);
					}
				}
			}
		} else if (s == "game-over") {
			return true;;
		} else if (s == "blind/") {
			while (s = que.front(), que.pop(), s != "/blind") {
				if (my_id == atoi(s.c_str())) {
					s = que.front();
					que.pop();
					player.bet(atoi(s.c_str()));
				} else {
					que.pop();
				}
			}
		} else if (s == "hold/") {
			player.status(HOLD);
			player.startCircle();
			for(int i=0;i<2;i++) {
				s_color = que.front(); que.pop();
				s_point = que.front(); que.pop();
				player.addHold(Card(s_color, s_point));
			}
			que.pop();
		} else if (s == "inquire/") {
			
			maxBet = 0;
			alive = 0;
			my_now_total = 0;
			my_now_raise = 0;
			record.clear();
			vector<string> tmp;
			my_action = "";
			cnt_raise = 0;
			while (s = que.front(), que.pop(), s != "/inquire") {
				if (s == "total") {
					s = que.front(); que.pop();
					s = que.front(); que.pop();
					totBet = atoi(s.c_str());
				} else {
					tmp.clear();
					t_pid = atoi(s.c_str());
					s = que.front(); que.pop(); tmp.push_back(s);
					t_jetton = atoi(s.c_str());
					s = que.front(); que.pop(); tmp.push_back(s);
					t_money = atoi(s.c_str());
					s = que.front(); que.pop(); tmp.push_back(s);
					t_now = atoi(s.c_str());
					s = que.front(); que.pop(); tmp.push_back(s);
					t_action = s;

					record.push_back(tmp);

					if (t_action != "fold") alive++;
					if (t_action == "raise") cnt_raise++;
					if (t_action == "call") cnt_call++;
					if (t_pid == my_id) {
						my_now_total = t_now;
						my_action = t_action;
						player.setMoney(t_money, t_jetton);
					}
					if (t_pid == my_id && t_action == "blind" && t_now == player.bigBind) player.isBigBind(true);
					if (maxBet < t_now) maxBet = t_now;
				}
			}

			// my_now_raise = maxBet - my_now_total;
			if (alive == 1) {
				// 如果所有人都弃牌  check
				sprintf(buf, "check");
			} else if (player.status() == HOLD) {
				// 底牌轮
				if (check_level(player.myCards)) {
					//只打level2以上的牌
 
					// 如果前面没有人raise, raise
					if (cnt_raise == 0) {
						my_now_raise = 200 + cnt_call * 40;
						sprintf(buf, "raise %d", my_now_raise);
					} else if (cnt_raise > 1) {
						sprintf(buf, "all_in");
					} else if (cnt_raise == 1 && my_action == "raise") {
						// 如果只有我raise
						sprintf(buf, "check");
					} else {
						//如果有人在你前面call这个raise all in 
						//如果这个raise超过你的1/3筹码,all in
						flag = false;
						int tt = record.size() - 1;
						for(int i = min(tt,6); i >= 0; i--) {
							if (record[i][3] == "raise") {
								for(int j = i - 1; j >= 0; j--) {
									if (record[j][3] == "call") {
										flag = true; break;
									}
								}
							}
						}
						if (flag || (maxBet - my_now_total) > player.jetton / 3) {
							sprintf(buf, "all_in");
						} else {
							my_now_raise = 200;
							sprintf(buf, "raise %d", my_now_raise);
						}
					}
				} else if (player.isBigBind()){
					sprintf(buf, "check");
				} else {
					sprintf(buf, "fold");
				}
			} else if (player.status() == FLOP) {
				double p = player.calcProbility();
				if (p > 0.85) {
					//sprintf(buf, "call");
					if (cnt_raise > 0) sprintf(buf, "raise 200");
					else if (cnt_raise == 0) sprintf(buf, "raise %d", 200);
				} else {
					if (cnt_raise == 0) sprintf(buf, "check");
					else sprintf(buf, "fold");
				}
			} else {
				if (cnt_raise > 0 && my_now_total == 40) sprintf(buf, "fold");
				else sprintf(buf, "call");
			}



			// sprintf(buf, "all_in");
			len = strlen(buf);
			if ((sendbytes = send(m_socket_id, buf, len, 0)) != len) {
				perror("send error");
				close(m_socket_id);
				return 0;
			}
		} else if (s == "flop/") {
			player.status(FLOP);
			player.startCircle();
			for(int i=0;i<3;i++) {
				s_color = que.front(); que.pop();
				s_point = que.front(); que.pop();
				player.addCard(Card(s_color, s_point));
			}
			que.pop();
		} else if (s == "turn/") {
			player.status(TURN);
			player.startCircle();
			s_color = que.front(); que.pop();
			s_point = que.front(); que.pop();
			player.addCard(Card(s_color, s_point));
			que.pop();
		} else if (s == "river/") {
			player.status(RIVER);
			player.startCircle();
			s_color = que.front(); que.pop();
			s_point = que.front(); que.pop();
			player.addCard(Card(s_color, s_point));
			que.pop();
		} else if (s == "showdown/") {
			while (s != "/showdown") {
				s = que.front();
				que.pop();
			}
		} else if (s == "pot-win/") {
			while (s = que.front(), que.pop(), s != "/pot-win") {
				if (my_id == atoi(s.c_str())) {
					s = que.front();
					que.pop();
					player.win(atoi(s.c_str()));
				} else {
					que.pop();
				}
			}
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	string pname = "lengly";

	if (argc != 6) {
		printf("Input format error\n");
		return 0;
	}
	// 提取命令行參數
	in_addr_t server_ip = inet_addr(argv[1]);
	in_port_t server_port = atoi(argv[2]);
	in_addr_t my_ip = inet_addr(argv[3]);
	in_port_t my_port = atoi(argv[4]);
	my_id = atoi(argv[5]);
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
	while(0 != connect(m_socket_id, (struct sockaddr *)&server_addr, sizeof(sockaddr))) {
		usleep(100*1000);
	}
	// if(connect(m_socket_id, (struct sockaddr *)&server_addr, sizeof(sockaddr))) {  
	// 	perror("connect出错！");  
	// 	exit(1);  
	// }  
	printf("Connect server success(%s:%u)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

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
	que = queue<string>();
	qc = 0;
	// 开始比赛
	while (true) {
		// 接收消息
		receive();

		// 是否有至少一条完整指令
		if (qc <= 0) continue;

		// 开始解析
		if (solve()) break;
	}

	close(m_socket_id);  
	return 0;
}