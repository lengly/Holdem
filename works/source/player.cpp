#include "player.h"

Player::Player() {
	clear();
}

void Player::clear() {
	cards.clear();
	common.clear();
	myCards.clear();
}

void Player::addCard(Card card) {
	cards.push_back(card);
	common.push_back(card);
}

void Player::setHoldCards(Card card1, Card card2) {
	myCards.push_back(card1);
	myCards.push_back(card2);
	cards.push_back(card1);
	cards.push_back(card2);
}