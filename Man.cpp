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
		msg = GetMouseMsg(); // ��ȡ�������Ϣ������ʽ
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) // �ж� ������ && ���λ���Ƿ���Ч
			break;
	}
	// std::cout << pos.x << " " << pos.y << "  ";
	chess->downChess(&pos, BLACKCHESS);
}
