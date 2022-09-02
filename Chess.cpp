#include "Chess.h"
#include <mmstream.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>

void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
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
	// playerFlag = true; // ��ʾ��������

	this->chessMap = vector<vector<int>>(gradeSize, vector<int>(gradeSize, 0));
}
void Chess::init()
{
	initgraph(897, 895);

	// ��ʵ���̵�ͼƬ
	loadimage(0, "res/����2.jpg");
	// ���ſ�������
	mciSendString("play res/start.wav", 0, 0, 0);
	// ���غڰ����ӣ����ص��ڴ棬���Ȳ���ʾ
	loadimage(&imgCHESSBLACK, "res/black.png", chessSize, chessSize, true);
	loadimage(&imgCHESSWHITE, "res/white.png", chessSize, chessSize, true);
	// ��������
	this->chessMap = vector<vector<int>>(gradeSize, vector<int>(gradeSize, 0));
	// playerFlag = true; // ��ʾ��������
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
	if (chessMap[m][n] != 0) return false;  // ����˴��������ӣ�����ʧЧ
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
	// �����������Ƿ�ﵽ5��
	int chessNum = ct == WHITECHESS ? 1 : -1;
	vector<vector<int>> direct = { {1, 0}, {1, -1}, {0, -1}, {-1, -1} };  // ������������
	int i = lastPos.x;
	int j = lastPos.y;
	for (auto dir : direct) {
		int continuousNum = 1; // �����������ӵĸ��������Ϊ1

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
			if (chessNum == -1) {  //����Ӯ�����Ӯ��
				mciSendString("play res/����.mp3", 0, 0, 0);
				loadimage(0, "res/ʤ��.jpg");
			}
			else {
				mciSendString("play res/ʧ��.mp3", 0, 0, 0);
				loadimage(0, "res/ʧ��.jpg");
			}

			_getch(); // ����ͷ�ļ� #include <conio.h>
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

