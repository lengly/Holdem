#include "card.h"
#include <cstring>
#include <cstdlib>
#include <vector>

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

bool Card::compare(std::vector<Card> my, std::vector<Card> cmpr) {
	return true;
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

bool Card::operator == (const Card &c) {
	return (this->color == c.color) && (this->point == c.point);
}

bool Card::operator != (const Card &c) {
	return (this->color != c.color) || (this->point != c.point);
}
