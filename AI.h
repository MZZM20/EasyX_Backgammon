#include "Chess.h"

class AI {
public:
    void init(Chess* chess);
    void go(); // 下棋
private:
    Chess* chess;
    vector<vector<int>> scoreMap;
    void calculateScore();
    ChessPos think();  // 考虑根据scoreMap选择下棋位置
};
