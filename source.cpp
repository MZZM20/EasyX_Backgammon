#include <iostream>
#include "ChessGame.h"

int main() {
	Man man;
	AI ai;
	Chess chess(13, 43, 43, 67);
	ChessGame game(&ai, &man, &chess);

	game.play();
	return 0;
}