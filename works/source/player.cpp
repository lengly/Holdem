#include "player.h"

Player::Player() {
	clear();
	money = jetton = 2000;
	maxInitialMoney = 2000;
	smallBind = 20;
	bigBind = 40;
}

void Player::clear() {
	cards.clear();
	common.clear();
	myCards.clear();
	circle = round = now = 0;
	money = jetton = 0;
	maxInitialMoney = 0;
}

void Player::addCard(Card card) {
	cards.push_back(card);
	common.push_back(card);
}

void Player::addHold(Card card) {
	cards.push_back(card);
	myCards.push_back(card);
}

void Player::setHoldCards(Card card1, Card card2) {
	myCards.push_back(card1);
	myCards.push_back(card2);
	cards.push_back(card1);
	cards.push_back(card2);
}

void Player::startRound() {
	now = round = circle = 0;
}

void Player::bet(int _bet) {
	jetton -= _bet;
	if (jetton <= 0) {
		jetton += maxInitialMoney;
		money -= maxInitialMoney;
	}
}

void Player::win(int _win) {
	jetton += _win;
}

int Player::status() {
	return _status;
}

void Player::status(int s) {
	_status = s;
}