#ifndef _PLAYER_H
#define _PLAYER_H

#include <vector>

struct Card {
	int color, point;
	Card(int _color, int _point) { color = _color; point = _point;}
	void clear() { color = point = 0;}
};

class Player {
private:
	std::vector<Card> cards;	// 所有牌 最多七张
	std::vector<Card> common;	// 公共牌 最多五张
	std::vector<Card> myCards;	// 底牌
public:
	Player();
	void clear();
	void addCard(Card card);
	void setHoldCards(Card card1, Card card2);
	int checkResult();
};

#endif