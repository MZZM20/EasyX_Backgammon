#include "AI.h"
#include "Man.h"
#include "Chess.h"

class ChessGame {
public:
    ChessGame(AI* ai, Man* man, Chess* chess);
    void play(); // ��ʼ��Ϸ
private:
    Man* man;
    AI* ai;
    Chess* chess;
};
