#ifndef _CARD_H
#define _CARD_H

#include "game.h"
#include <string>
#include <vector>

class Card {
private:
	int color, point;
public:
	Card() {}
	Card(int _color, int _point);
	Card(std::string _color, std::string _point);
	void clear();
	static double calc(std::vector<Card> hold, std::vector<Card> common, std::vector<Card> total);
	bool notin (const std::vector<Card> &c); // 判断当前牌是否在这个vector中

	bool operator == (const Card &c);
	bool operator != (const Card &c);
};

#endif