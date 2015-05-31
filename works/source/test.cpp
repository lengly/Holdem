#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include "game.h"
#include "card.h"
using namespace std;

void test_7to5() {
	Card test[7];
	test[0] = Card(1,13);
	test[1] = Card(1,13);
	test[2] = Card(1,13);
	test[3] = Card(1,13);
	test[4] = Card(1,13);
	test[5] = Card(1,13);
	test[6] = Card(1,13);
	int result = Card::sevenToFive(test);
	// printf("result: %d\n", result);
	// for(int i=0;i<7;i++) {
	// 	printf("%d  %d\n", test[i].color, test[i].point);
	// }
}

void test_calc() {
	bool flag[4][12];
	memset(flag,1,sizeof(flag));
	int c,p;
	vector<Card> hold, common, total;
	
	// for(int i = 0; i < 2; i++) {
	// 	while( c=(rand()%4)+1, p=(rand()%13+2), !flag[c][p] );
	// 	flag[c][p] = false;
	// 	hold.push_back(Card(c,p));
	// }
	//hold[0] = Card(1,13);
	//hold[1] = Card(3,2);
	// for(int i = 0; i < 3; i++) {
	// 	while( c=(rand()%4)+1, p=(rand()%13+2), !flag[c][p] );
	// 	flag[c][p] = false;
	// 	common.push_back(Card(c,p));
	// }

	hold.push_back(Card(1,13));
	hold.push_back(Card(2,4));
	common.push_back(Card(1,8));
	common.push_back(Card(1,12));
	common.push_back(Card(3,4));
	// common.push_back(Card(4,8));

	total.push_back(hold[0]);
	total.push_back(hold[1]);
	total.push_back(common[0]);
	total.push_back(common[1]);
	total.push_back(common[2]);
	// total.push_back(common[3]);

	double ans = Card::calc(hold, common, total);
	printf("%.6lf\n", ans);
}

int main() {
	//freopen("output","w",stdout);
	clock_t  clockBegin, clockEnd;    
    clockBegin = clock();    

    // for(int i=0;i<6000000;i++)  // 14s
    // 	test_7to5();
    for(int i=0;i<1;i++)
		test_calc();
 
	clockEnd = clock();    
    //printf("time:  %.6lf\n", (clockEnd - clockBegin) / 1000000.0);    
	
	return 0;
}