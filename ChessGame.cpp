#include "ChessGame.h"

ChessGame::ChessGame(AI* ai, Man* man, Chess* chess)
{
	this->ai = ai;
	this->man = man;
	this->chess = chess;

	ai->init(chess);
	man->init(chess);
}

void ChessGame::play()
{
	chess->init();
	while (true) {
		man->go();
		if (chess->gameOver(BLACKCHESS)) {
			chess->init();
			continue;
		}
		ai->go();
		if (chess->gameOver(WHITECHESS)) {
			chess->init();
			continue;
		}
	}
}

