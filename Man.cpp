#include "Man.h"
#include <iostream>
void Man::init(Chess* chess)
{
	this->chess = chess;
}
void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;

	while (true) {
		msg = GetMouseMsg(); // 获取鼠标点击消息，阻塞式
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) // 判断 鼠标左键 && 点击位置是否有效
			break;
	}
	// std::cout << pos.x << " " << pos.y << "  ";
	chess->downChess(&pos, BLACKCHESS);
}
