#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include "game.h"
#include "card.h"
using namespace std;

void test_7to5() {
	vector<Card> test;
	test.push_back(Card(1,13));
	test.push_back(Card(3,12));
	test.push_back(Card(3,14));
	test.push_back(Card(2,3));
	test.push_back(Card(1,4));
	test.push_back(Card(2,9));
	test.push_back(Card(4,6));
	int result = Card::sevenToFive(test);
	printf("result: %d\n", result);
	for(int i=0;i<test.size();i++) {
		printf("%d  %d\n", test[i].color, test[i].point);
	}
}

void test_calc() {
	vector<Card> hold, common, total;
	hold.push_back(Card(1,2));
	hold.push_back(Card(2,10));
	common.push_back(Card(1,2));
	common.push_back(Card(2,4));
	common.push_back(Card(3,6));
	//common.push_back(Card(4,8));

	total.push_back(hold[0]);
	total.push_back(hold[1]);
	total.push_back(common[0]);
	total.push_back(common[1]);
	total.push_back(common[2]);
	//total.push_back(common[3]);

	double ans = Card::calc(hold, common, total);
	printf("%.6lf\n", ans);
}

int main() {

	//test_7to5();
	test_calc();
	return 0;
}