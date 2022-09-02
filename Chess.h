#ifndef CHESS_H_
#define CHESS_H_
#include <graphics.h>
#include <vector>
using namespace std;
// ������
struct ChessPos {
    int x;
    int y;
    ChessPos(int x = 0, int y = 0) : x(x), y(y) {}
};

typedef enum {
    WHITECHESS = 1, // ����
    BLACKCHESS = -1 // ����
} ChessType;

class Chess {
public:
    Chess(int gradeSize, int marginX, int marginY, float chessSize);
    void init();
    void downChess(ChessPos* cp, ChessType ct);
    bool clickBoard(int x, int y, ChessPos* cp); // ����true��ʾ����λ����ȷ
    int getGradeSize(); // ��ȡ���̵Ĵ�С 13, 17, 19
    bool gameOver(ChessType ct); // �������Ƿ����
    // ��ȡָ��λ���Ǻ��壬���ǰ��壬���ǿհ�
    int getChessData(ChessPos* pos);
    int getChessData(int row, int col);
private:
    IMAGE imgCHESSBLACK;
    IMAGE imgCHESSWHITE;

    int gradeSize;
    float chessSize;
    int marginX;
    int marginY;
    ChessPos lastPos; //�������λ��, Chess��private���ݳ�Ա

    vector<vector<int>> chessMap;
    bool playerFlag;
    void updateGameMap(ChessPos* cp, ChessType ct);
};
#endif