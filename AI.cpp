#include "AI.h"
#include <ctime>
#include <random>

void AI::init(Chess* chess)
{
	this->chess = chess;
	scoreMap = vector<vector<int>>(chess->getGradeSize(), vector<int>(chess->getGradeSize(), 0));
}

void AI::go()
{
	ChessPos pos = think();
	// Sleep(1000); //��װ˼��
	chess->downChess(&pos, WHITECHESS);
}

void AI::calculateScore()
{
	scoreMap = vector<vector<int>>(chess->getGradeSize(), vector<int>(chess->getGradeSize(), 0));  // ����ͼ����
	vector<vector<int>> direct = { {1, 0}, {1, -1}, {0, -1}, {-1, -1} };  // ������������
	// �����غϣ�����˴��°���
	for (int i = 0; i < scoreMap.size(); i++)
		for (int j = 0; j < scoreMap.size(); j++) {
			if (chess->getChessData(i, j) != 0) continue;  
	
			int sumScore = 0;  // �ڸõ����������ܷ�
			for (auto dir : direct) {
				int score; // ÿ�������ϵĴ��
				int continuousNum = 1; // �����������ӵĸ��������Ϊ1
				int blankNum = 0;  // �����߽���ڽ�ĸ����� ���Ϊ2
				int halmaNum = 0; // ÿ������������λ��

				int right_i = i + dir[0];
				int right_j = j + dir[1];
				while (-1 < right_i && right_i < scoreMap.size() && -1 < right_j && right_j < scoreMap.size()) {
					if (chess->getChessData(right_i, right_j) == 1) continuousNum++;
					else break;
					right_i += dir[0];
					right_j += dir[1];
				}
				int left_i = i - dir[0];
				int left_j = j - dir[1];
				while (-1 < left_i && left_i < scoreMap.size() && -1 < left_j && left_j < scoreMap.size()) {
					if (chess->getChessData(left_i, left_j) == 1) continuousNum++;
					else break;
					left_i -= dir[0];
					left_j -= dir[1];
				}
				if (continuousNum == 5) {
					scoreMap[i][j] = INT_MAX;  // �������Ž⣬ֱ�ӷ���
					return;
				}
				int edgeNum = 0; // һ�˵������հ�λ��
				while (-1 < right_i && right_i < scoreMap.size() && -1 < right_j && right_j < scoreMap.size()) {
					if (chess->getChessData(right_i, right_j) != -1) edgeNum++;
					else break;
					if (chess->getChessData(right_i, right_j) == 1) halmaNum++;
					right_i += dir[0];
					right_j += dir[1];
				}
				if (edgeNum >= 5 - continuousNum) blankNum++;
				edgeNum = 0;
				while (-1 < left_i && left_i < scoreMap.size() && -1 < left_j && left_j < scoreMap.size()) {
					if (chess->getChessData(left_i, left_j) != -1) edgeNum++;
					else break;
					if (chess->getChessData(left_i, left_j) == 1) halmaNum++;
					left_i -= dir[0];
					left_j -= dir[1];
				}
				if (edgeNum >= 5 - continuousNum) blankNum++;
				//if (blankNum > 0) blankNum = 1;
				// �����������
				score = continuousNum * continuousNum * 1000 * blankNum + halmaNum * halmaNum * 100;
				sumScore += score;
			}
			scoreMap[i][j] = sumScore;
		}

	// �����غϣ�����˴��º���
	for (int i = 0; i < scoreMap.size(); i++)
		for (int j = 0; j < scoreMap.size(); j++) {
			if (chess->getChessData(i, j) != 0) continue;

			int sumScore = 0;  // �ڸõ����������ܷ�
			for (auto dir : direct) {
				int score; // ÿ�������ϵĴ��
				int continuousNum = 1; // �����������ӵĸ��������Ϊ1
				int blankNum = 0;  // �����߽���ڽ�ĸ����� ���Ϊ2
				int halmaNum = 0; // ÿ������������λ��

				int right_i = i + dir[0];
				int right_j = j + dir[1];
				while (-1 < right_i && right_i < scoreMap.size() && -1 < right_j && right_j < scoreMap.size()) {
					if (chess->getChessData(right_i, right_j) == -1) continuousNum++;
					else break;
					right_i += dir[0];
					right_j += dir[1];
				}
				int left_i = i - dir[0];
				int left_j = j - dir[1];
				while (-1 < left_i && left_i < scoreMap.size() && -1 < left_j && left_j < scoreMap.size()) {
					if (chess->getChessData(left_i, left_j) == -1) continuousNum++;
					else break;
					left_i -= dir[0];
					left_j -= dir[1];
				}
				if (continuousNum == 5) {
					scoreMap[i][j] = INT_MAX;  // �������Ž⣬ֱ�ӷ���
					return;
				}

				int edgeNum = 0; // һ�˵������հ�λ��
				while (-1 < right_i && right_i < scoreMap.size() && -1 < right_j && right_j < scoreMap.size()) {
					if (chess->getChessData(right_i, right_j) != 1) edgeNum++;
					else break;
					if (chess->getChessData(right_i, right_j) == -1) halmaNum++;
					right_i += dir[0];
					right_j += dir[1];
				}
				if (edgeNum >= 5 - continuousNum) blankNum++;
				edgeNum = 0;
				while (-1 < left_i && left_i < scoreMap.size() && -1 < left_j && left_j < scoreMap.size()) {
					if (chess->getChessData(left_i, left_j) != 1) edgeNum++;
					else break;
					if (chess->getChessData(left_i, left_j) == -1) halmaNum++;
					left_i -= dir[0];
					left_j -= dir[1];
				}
				if (edgeNum >= 5 - continuousNum) blankNum++;
				//if (blankNum > 0) blankNum = 1;
				// �����������
				score = continuousNum * continuousNum * 1000 * blankNum + halmaNum * halmaNum * 100;
				sumScore += score;
				if (continuousNum == 2) {
					int n = 0;
				}
			}
			scoreMap[i][j] = max(scoreMap[i][j], sumScore);
		}
}

ChessPos AI::think()
{
	calculateScore();
	int num = 0;
	vector<vector<int>> vec;
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap.size(); j++) {
			if (scoreMap[i][j] > num) {
				num = scoreMap[i][j];
				vec.clear();
				vec.push_back({ i, j });
			}
			else if (scoreMap[i][j] == num) {
				vec.push_back({ i, j });
			}
		}
	}
	std::default_random_engine e;
	std::uniform_int_distribution<int> u(0, vec.size() - 1); // ����ұ�����
	e.seed(time(0));
	num = u(e);
	return ChessPos(vec[num][0], vec[num][1]);
}
