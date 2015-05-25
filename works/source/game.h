#ifndef _GAME_H
#define _GAME_H

// 花色
#define SPADES				1	// 黑桃
#define HEARTS				2	// 红桃
#define CLUBS				3	// 梅花
#define DIAMONDS			4	// 方片
// 牌面
#define HIGH_CARD			1	// 高牌
#define ONE_PAIR			2	// 一对
#define TWO_PAIR			3	// 两对
#define THREE_OF_A_KIND		4	// 三条
#define STRAIGHT			5	// 顺子
#define FLUSH				6	// 同花
#define FULL_HOUSE			7	// 葫芦
#define FOUR_OF_A_KIND		8	// 四条
#define STRAIGHT_FLUSH		9	// 同花顺
// 状态
#define HOLD				0	// 底牌后询问
#define FLOP				1	// 三张公共牌后询问
#define TURN				2	// 转牌后询问
#define RIVER				3	// 河牌后询问

#endif