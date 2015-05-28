#include "card.h"
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

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

double Card::calc(std::vector<Card> hold, std::vector<Card> common, std::vector<Card> total) {
	// 这里对手使用common  自己使用total
	int win = 0, lose = 0;
	bool flag[5][15];
	memset(flag,1,sizeof(flag));
	for(int i=0;i<total.size();i++) {
		flag[total[i].color][total[i].point] = false;
	}
	int n = 5 - common.size();
	int p1,c1,p2,c2,p3,c3,p4,c4;
	
	// 枚举对手底牌
	for(int c1 = 1; c1 < 5; c1++) {
		for(int p1 = 2; p1 < 15; p1++) {
			if (flag[c1][p1]) {
				flag[c1][p1] = false;
				common.push_back(Card(c1, p1));
				for(int c2 = c1; c2 < 5; c2++) {
					for(int p2 = 2; p2 < 15; p2++) {
						if (c2 == c1 && p2 <= p1) continue;
						if (flag[c2][p2]) {
							flag[c2][p2] = false;
							common.push_back(Card(c2, p2));
							// 枚举剩余公共牌
							if (n == 0) {
								compare(total, common) ? win++ : lose++;
							} else if (n == 1) {
								for(int c3 = 1; c3 < 5; c3++) {
									for(int p3 = 2; p3 < 15; p3++) {
										if (flag[c3][p3]) {
											flag[c3][p3] = false;
											common.push_back(Card(c3, p3));
											total.push_back(Card(c3, p3));

											compare(total, common) ? win++ : lose++;

											flag[c3][p3] = true;
											common.pop_back();
											total.pop_back();
										}
									}
								}
							} else if (n == 2) {
								for(int c3 = 1; c3 < 5; c3++) {
									for(int p3 = 2; p3 < 15; p3++) {
										if (flag[c3][p3]) {
											flag[c3][p3] = false;
											common.push_back(Card(c3, p3));
											total.push_back(Card(c3, p3));
											for(int c4 = c3; c4 < 5; c4++) {
												for(int p4 = 2; p4 < 15; p4++) {
													if (flag[c4][p4]) {
														flag[c4][p4] = false;
														common.push_back(Card(c4, p4));
														total.push_back(Card(c4, p4));

														compare(total, common) ? win++ : lose++;

														flag[c4][p4] = true;
														common.pop_back();
														total.pop_back();
													}
												}
											}
											flag[c3][p3] = true;
											common.pop_back();
											total.pop_back();
										}
									}
								}
							}
							flag[c2][p2] = true;
							common.pop_back();
						}
					}
				}
				flag[c1][p1] = true;
				common.pop_back();
			}
		}
	}
	double p = win + lose;
	if (p > 0) p = win / p;
	return p;
}

bool Card::compare(std::vector<Card> my, std::vector<Card> cmpr) {
	if (my.size() != 7 || cmpr.size() != 7) return false;
	int level_my = sevenToFive(my);
	int level_cmpr = sevenToFive(cmpr);
	if (level_my > level_cmpr) return true;
	if (level_my < level_cmpr) return false;

	if (my.size() != 5 || cmpr.size() != 5) return false;
	for(int i=0;i<my.size();i++) {
		if (my[i].point > cmpr[i].point) return true;
		if (my[i].point < cmpr[i].point) return false;
	}
	return false;
}

int Card::sevenToFive(std::vector<Card> &p) {
	bool flag;
	int i, j, status;
	// 同花顺
	sort(p.begin(), p.end(), cmp1());
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
	sort(p.begin(), p.end(), cmp2());
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
	sort(p.begin(), p.end(), cmp1());
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
	sort(p.begin(), p.end(), cmp2());
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
			sort(p.begin()+2, p.end(), cmp2());
			i = 0;
			status = ONE_PAIR;
			goto end;
		}
	}
	// 高牌
	i = 0;
	status = HIGH_CARD;
end:
	if (i == 0) p.erase(p.begin()+5),p.erase(p.begin()+5);
	if (i == 1) p.erase(p.begin()+0),p.erase(p.begin()+5);
	if (i == 2) p.erase(p.begin()+0),p.erase(p.begin()+0);
	return status;
}

bool Card::operator == (const Card &c) {
	return (this->color == c.color) && (this->point == c.point);
}

bool Card::operator != (const Card &c) {
	return (this->color != c.color) || (this->point != c.point);
}
