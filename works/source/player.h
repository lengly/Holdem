#ifndef _PLAYER_H
#define _PLAYER_H

#include <vector>
#include "card.h"

class Player {
private:
	std::vector<Card> cards;		// 所有牌 最多七张
	std::vector<Card> common;		// 公共牌 最多五张
	std::vector<Card> myCards;		// 底牌
	int money, jetton;				// 剩余金币 当前筹码 
	int circle, round, now;			// 这一圈的下注 这一轮的下注 当前应下注金额
	int maxInitialMoney;			// 最大进场筹码
	int smallBind, bigBind;			// 小盲注 大盲注
	int _status;
public:
	Player();
	void clear();
	void addCard(Card card);
	void addHold(Card card);
	void setHoldCards(Card card1, Card card2);
	int checkResult();
	void startRound();
	void bet(int _bet);
	int status();
	void status(int s);
};

#endif