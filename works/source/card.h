#ifndef _CARD_H
#define _CARD_H

#include "game.h"
#include <string>
#include <vector>

class Card {
public:
	int color, point;
	Card() {}
	Card(int _color, int _point);
	Card(std::string _color, std::string _point);
	void clear();
	static double calc(std::vector<Card> hold, std::vector<Card> common, std::vector<Card> total);
	static bool compare(std::vector<Card> my, std::vector<Card> cmpr);
	static int sevenToFive(std::vector<Card> &p);
	// static bool cmp1(const Card &c1, const Card &c2); // 先按花色排序再按点排序
	// static bool cmp2(const Card &c1, const Card &c2); // 先按点排序再按花色排序

	bool operator == (const Card &c);
	bool operator != (const Card &c);
};

// 先按花色排序再按点排序
struct cmp1 {
	bool operator() (const Card &c1, const Card &c2) {
		return (c1.color > c2.color) || (c1.color == c2.color && c1.point > c2.point);
	}
};
// 先按点排序再按花色排序
struct cmp2 {
	bool operator() (const Card &c1, const Card &c2) {
		return (c1.point > c2.point) || (c1.point == c2.point && c1.color > c2.color);
	}
};


#endif