#include "card.h"
#include <string>
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

double Card::calc(std::vector<Card> hold, std::vector<Card> common, std::vector<Card> total) {

}

bool Card::notin(const std::vector<Card> &c) {
	for (int i=0;i<c.size();i++) {
		if (*this == c[i]) return false;
	}
	return true;
}

bool Card::operator == (const Card &c) {
	return (this->color == c.color) && (this->point == c.point);
}

bool Card::operator != (const Card &c) {
	return (this->color != c.color) || (this->point != c.point);
}
