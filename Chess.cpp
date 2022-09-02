#include "Chess.h"
#include <mmstream.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
	this->gradeSize = gradeSize;
	this->marginX = marginX;
	this->marginY = marginY;
	this->chessSize = chessSize;
	// playerFlag = true; // 表示黑棋先走

	this->chessMap = vector<vector<int>>(gradeSize, vector<int>(gradeSize, 0));
}
void Chess::init()
{
	initgraph(897, 895);

	// 现实棋盘的图片
	loadimage(0, "res/棋盘2.jpg");
	// 播放开场音乐
	mciSendString("play res/start.wav", 0, 0, 0);
	// 加载黑白棋子，加载到内存，但先不显示
	loadimage(&imgCHESSBLACK, "res/black.png", chessSize, chessSize, true);
	loadimage(&imgCHESSWHITE, "res/white.png", chessSize, chessSize, true);
	// 棋盘清零
	this->chessMap = vector<vector<int>>(gradeSize, vector<int>(gradeSize, 0));
	// playerFlag = true; // 表示黑棋先走
}
void Chess::downChess(ChessPos* cp, ChessType ct)
{
	mciSendString("play res/down7.WAV", 0, 0, 0);

	int x = marginX + cp->x * chessSize - 0.5 * chessSize;
	int y = marginY + cp->y * chessSize - 0.5 * chessSize;

	if (ct == WHITECHESS) {
		putimagePNG(x, y, &imgCHESSWHITE);
	}
	else {
		putimagePNG(x, y, &imgCHESSBLACK);
	}
	updateGameMap(cp, ct);
}

bool Chess::clickBoard(int x, int y, ChessPos* cp)
{
	if (x < marginX || x > marginX + (gradeSize - 1) * chessSize || y < marginY || y > marginY + (gradeSize - 1) * chessSize)
		return false;
	int m = (x - marginX) / chessSize;
	int n = (y - marginY) / chessSize;
	int i = (x - marginX) - m * chessSize;
	int j = (y - marginY) - n * chessSize;
	if (i > chessSize / 2) m++;
	if (j > chessSize / 2) n++;
	if (chessMap[m][n] != 0) return false;  // 如果此处已下棋子，则点击失效
	cp->x = m;
	cp->y = n;
	
	return true;
}

int Chess::getGradeSize()
{
	return gradeSize;
}

bool Chess::gameOver(ChessType ct)
{
	// 检查黑棋或白棋是否达到5子
	int chessNum = ct == WHITECHESS ? 1 : -1;
	vector<vector<int>> direct = { {1, 0}, {1, -1}, {0, -1}, {-1, -1} };  // 横西北竖西南
	int i = lastPos.x;
	int j = lastPos.y;
	for (auto dir : direct) {
		int continuousNum = 1; // 存在连续棋子的个数，最低为1

		int right_i = i + dir[0];
		int right_j = j + dir[1];
		while (-1 < right_i && right_i < gradeSize && -1 < right_j && right_j < gradeSize) {
			if (chessMap[right_i][right_j] == chessNum) continuousNum++;
			else break;
			right_i += dir[0];
			right_j += dir[1];
		}
		int left_i = i - dir[0];
		int left_j = j - dir[1];
		while (-1 < left_i && left_i < gradeSize && -1 < left_j && left_j < gradeSize) {
			if (chessMap[left_i][left_j] == chessNum) continuousNum++;
			else break;
			left_i -= dir[0];
			left_j -= dir[1];
		}
		if (continuousNum == 5) {
			if (chessNum == -1) {  //黑棋赢（玩家赢）
				mciSendString("play res/不错.mp3", 0, 0, 0);
				loadimage(0, "res/胜利.jpg");
			}
			else {
				mciSendString("play res/失败.mp3", 0, 0, 0);
				loadimage(0, "res/失败.jpg");
			}

			_getch(); // 补充头文件 #include <conio.h>
			return true;
		}
	}
	return false;
}

int Chess::getChessData(ChessPos* pos)
{
	return 0;
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

void Chess::updateGameMap(ChessPos* cp, ChessType ct) {
	lastPos = ChessPos(cp->x, cp->y);
	if (ct == WHITECHESS) chessMap[cp->x][cp->y] = 1;
	else chessMap[cp->x][cp->y] = -1;
}

