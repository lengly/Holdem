#ifndef _CARD_H
#define _CARD_H

#include <string>
#include <cstdlib>
#include "game.h"

struct Card {
	int color, point;
	Card() {}
	Card(std::string _color, std::string _point) {
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
	void clear() { color = point = 0;}
};

#endif