#include "card.h"
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstdio>

Card::Card(int _color, int _point) {
	color = _color;
	point = _point;
}

Card::Card(std::string _color, std::string _point) {
	if (_color == "SPADES") {
		color = SPADES;
	} else if (_color == "HEARTS") {
		color = HEARTS;
	} else if (_color == "CLUBS") {
		color = CLUBS;
	} else if (_color == "DIAMONDS") {
		color = DIAMONDS;
	}

	if (_point == "J") {
		point = 11;
	} else if (_point == "Q") {
		point = 12;
	} else if (_point == "K") {
		point = 13;
	} else if (_point == "A") {
		point = 14;
	} else {
		point = atoi(_point.c_str());
	}
}

void Card::clear() {
	 color = point = 0;
}

double Card::calc(std::vector<Card> _hold, std::vector<Card> _common, std::vector<Card> _total) {
	// 这里对手使用common  自己使用total
	int nc = _common.size(), nt = _total.size();
	Card common[7], total[7];
	for(int i = 0; i < nc; i++) {
		common[i] = _common[i];
	}
	for(int i = 0; i < nt; i++) {
		total[i] = _total[i];
	}

	int win = 0, lose = 0;
	bool flag[5][15];
	memset(flag,1,sizeof(flag));
	for(int i = 0; i < nt; i++) {
		flag[total[i].color][total[i].point] = false;
	}
	int n = 5 - nc;
	
	// random
	Card enu[7];
	srand(time(NULL));
	int c, p;
	for(int i = 0; i < MAX_ENU_TIME; i++) {
		for(int j = 0; j < n + 2; j++) {
			while( c=(rand()%4)+1, p=(rand()%13+2), !flag[c][p] );
			flag[c][p] = false;
			enu[j] = Card(c,p);	
		}
		for(int j = 0; j < n; j++) {
			common[4-j] = enu[j];
			total[6-j] = enu[j];
		}
		common[5] = enu[n];
		common[6] = enu[n+1];

		compare(total, common) ? win++ : lose++;

		for(int j = 0; j < n + 2; j++) {
			flag[enu[j].color][enu[j].point] = true;
		}
	}
	double pbt1 = win + lose;
	if (pbt1 > 0) pbt1 = win / pbt1;
	return pbt1;
	/*
	// printf("%.6lf\n", pbt);
	win = lose = 0;

	// 枚举对手底牌
	int p1,c1,p2,c2,p3,c3,p4,c4;
	for(int c1 = 1; c1 < 5; c1++) {
		for(int p1 = 2; p1 < 15; p1++) {
			if (flag[c1][p1]) {
				flag[c1][p1] = false;
				common[nc++] = Card(c1, p1);
				for(int c2 = c1; c2 < 5; c2++) {
					for(int p2 = 2; p2 < 15; p2++) {
						if (c2 == c1 && p2 <= p1) continue;
						if (flag[c2][p2]) {
							flag[c2][p2] = false;
							common[nc++] = Card(c2, p2);
							// 枚举剩余公共牌
							if (n == 0) {
								compare(total, common) ? win++ : lose++;
							} else if (n == 1) {
								for(int c3 = 1; c3 < 5; c3++) {
									for(int p3 = 2; p3 < 15; p3++) {
										if (flag[c3][p3]) {
											flag[c3][p3] = false;
											common[nc++] = Card(c3, p3);
											total[nt++] = common[nc-1];

											compare(total, common) ? win++ : lose++;

											flag[c3][p3] = true;
											nc--;
											nt--;
										}
									}
								}
							} else if (n == 2) {
								for(int c3 = 1; c3 < 5; c3++) {
									for(int p3 = 2; p3 < 15; p3++) {
										if (flag[c3][p3]) {
											flag[c3][p3] = false;
											common[nc++] = Card(c3, p3);
											total[nt++] = common[nc-1];
											for(int c4 = c3; c4 < 5; c4++) {
												for(int p4 = 2; p4 < 15; p4++) {
													if (flag[c4][p4]) {
														flag[c4][p4] = false;
														common[nc++] = Card(c4, p4);
														total[nt++] = common[nc-1];

														compare(total, common) ? win++ : lose++;

														flag[c4][p4] = true;
														nc--;
														nt--;
													}
												}
											}
											flag[c3][p3] = true;
											nc--;
											nt--;
										}
									}
								}
							}
							flag[c2][p2] = true;
							nc--;
						}
					}
				}
				flag[c1][p1] = true;
				nc--;
			}
		}
	}
	double pbt2 = win + lose;
	if (pbt2 > 0) pbt2 = win / pbt2;
	printf("%.6lf\n", pbt2);
	return pbt1-pbt2;
	*/
}

bool Card::compare(Card* _my, Card* _cmpr) {
	Card my[7], cmpr[7];
	for(int i = 0; i < 7; i++) {
		my[i] = _my[i];
		cmpr[i] = _cmpr[i];
	}

	int level_my = sevenToFive(my);
	int level_cmpr = sevenToFive(cmpr);
	if (level_my > level_cmpr) return true;
	if (level_my < level_cmpr) return false;

	for(int i = 0; i < 5; i++) {
		if (my[i].point > cmpr[i].point) return true;
		if (my[i].point < cmpr[i].point) return false;
	}
	return false;
}

int Card::sevenToFive(Card* p) {
	bool flag;
	int i, j, status;
	// 同花顺
	std::sort(p, p + 7, cmp1());
	for(i = 0; i < 3; i++) {
		flag = true;
		for(j = i + 1; j < i + 5; j++) {
			if (p[j].color != p[i].color || p[j].point != p[j-1].point-1) {
				flag = false;
				break;
			}
		}
		if (flag) {
			status = STRAIGHT_FLUSH;
			goto end;
		}
	}
	// 四条
	std::sort(p, p + 7, cmp2());
	for(i = 0; i < 4; i++) {
		flag = true;
		for(j = i + 1; j < i + 4; j++) {
			if (p[j].point != p[i].point) {
				flag = false;
				break;
			}
		}
		if (flag) {
			status = FOUR_OF_A_KIND;
			goto end;	
		}
	}
	// 葫芦
	for(i = 0; i < 5; i++) {
		flag = true;
		if (p[i].point != p[i+1].point) flag = false;
		if (p[i].point != p[i+2].point) flag = false;

		if (flag) {
			for(j = i + 3; j < i + 6; j++) {
				if (p[j%7].point == p[(j+1)%7].point) {
					status = FULL_HOUSE;
					goto end;		
				}
			}
		}
	}
	// 同花
	std::sort(p, p + 7, cmp1());
	for(i = 0; i < 3; i++) {
		flag = true;
		for(j = i + 1; j < i + 5; j++) {
			if (p[j].color != p[i].color) {
				flag = false;
				break;
			}
		}
		if (flag) {
			status = FLUSH;
			goto end;
		}
	}
	// 顺子
	std::sort(p, p + 7, cmp2());
	for(i = 0; i < 3; i++) {
		flag = true;
		for(j = i + 1; j < i + 5; j++) {
			if (p[j].point != p[j-1].point - 1) {
				flag = false;
				break;
			}
		}
		if (flag) {
			status = STRAIGHT;
			goto end;
		}
	}
	// 三条
	for(i = 0; i < 5; i++) {
		flag = true;
		if (p[i].point != p[i+1].point) flag = false;
		if (p[i].point != p[i+2].point) flag = false;
		if (flag) {
			status = THREE_OF_A_KIND;
			goto end;
		}
	}
	// 两对  一对
	for(i = 0; i < 4; i++) {
		if (p[i].point == p[i+1].point) {
			for(j = i + 2; j < 6; j++) {
				if (p[j].point == p[j+1].point) {
					std::swap(p[0], p[i]);
					std::swap(p[1], p[i+1]);
					std::swap(p[2], p[j]);
					std::swap(p[3], p[j+1]);
					if (p[4].point < p[5].point) std::swap(p[4], p[5]);
					if (p[4].point < p[5].point) std::swap(p[4], p[5]);
					i = 0;
					status = TWO_PAIR;
					goto end;	
				}
			}
			std::swap(p[0], p[i]);
			std::swap(p[1], p[i+1]);
			std::sort(p + 2, p + 7, cmp2());
			i = 0;
			status = ONE_PAIR;
			goto end;
		}
	}
	// 高牌
	i = 0;
	status = HIGH_CARD;
end:
	for(j = 0; j < 5; j++) {
		p[j] = p[i+j];
	}
	return status;
}

bool Card::operator == (const Card &c) {
	return (this->color == c.color) && (this->point == c.point);
}
/*
bool Card::operator != (const Card &c) {
	return (this->color != c.color) || (this->point != c.point);
}
*/
void Card::operator = (const Card &c) {
	this->color = c.color;
	this->point = c.point;
}