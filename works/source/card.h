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

	bool operator == (const Card &c);
	bool operator != (const Card &c);
};

#endif