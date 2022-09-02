#ifndef CHESS_H_
#define CHESS_H_
#include <graphics.h>
#include <vector>
using namespace std;
// 棋盘类
struct ChessPos {
    int x;
    int y;
    ChessPos(int x = 0, int y = 0) : x(x), y(y) {}
};

typedef enum {
    WHITECHESS = 1, // 白棋
    BLACKCHESS = -1 // 黑棋
} ChessType;

class Chess {
public:
    Chess(int gradeSize, int marginX, int marginY, float chessSize);
    void init();
    void downChess(ChessPos* cp, ChessType ct);
    bool clickBoard(int x, int y, ChessPos* cp); // 返回true表示落子位置正确
    int getGradeSize(); // 获取棋盘的大小 13, 17, 19
    bool gameOver(ChessType ct); // 检查棋局是否结束
    // 获取指定位置是黑棋，还是白棋，还是空白
    int getChessData(ChessPos* pos);
    int getChessData(int row, int col);
private:
    IMAGE imgCHESSBLACK;
    IMAGE imgCHESSWHITE;

    int gradeSize;
    float chessSize;
    int marginX;
    int marginY;
    ChessPos lastPos; //最近落子位置, Chess的private数据成员

    vector<vector<int>> chessMap;
    bool playerFlag;
    void updateGameMap(ChessPos* cp, ChessType ct);
};
#endif