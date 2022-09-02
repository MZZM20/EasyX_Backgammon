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
	// Sleep(1000); //假装思考
	chess->downChess(&pos, WHITECHESS);
}

void AI::calculateScore()
{
	scoreMap = vector<vector<int>>(chess->getGradeSize(), vector<int>(chess->getGradeSize(), 0));  // 分数图清零
	vector<vector<int>> direct = { {1, 0}, {1, -1}, {0, -1}, {-1, -1} };  // 横西北竖西南
	// 进攻回合，如果此处下白棋
	for (int i = 0; i < scoreMap.size(); i++)
		for (int j = 0; j < scoreMap.size(); j++) {
			if (chess->getChessData(i, j) != 0) continue;  
	
			int sumScore = 0;  // 在该点下棋所得总分
			for (auto dir : direct) {
				int score; // 每个方向上的打分
				int continuousNum = 1; // 存在连续棋子的个数，最低为1
				int blankNum = 0;  // 两个边界存在解的个数， 最高为2
				int halmaNum = 0; // 每个方向上跳棋位数

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
					scoreMap[i][j] = INT_MAX;  // 存在最优解，直接返回
					return;
				}
				int edgeNum = 0; // 一端的连续空白位数
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
				// 分数计算规则
				score = continuousNum * continuousNum * 1000 * blankNum + halmaNum * halmaNum * 100;
				sumScore += score;
			}
			scoreMap[i][j] = sumScore;
		}

	// 防御回合，如果此处下黑棋
	for (int i = 0; i < scoreMap.size(); i++)
		for (int j = 0; j < scoreMap.size(); j++) {
			if (chess->getChessData(i, j) != 0) continue;

			int sumScore = 0;  // 在该点下棋所得总分
			for (auto dir : direct) {
				int score; // 每个方向上的打分
				int continuousNum = 1; // 存在连续棋子的个数，最低为1
				int blankNum = 0;  // 两个边界存在解的个数， 最高为2
				int halmaNum = 0; // 每个方向上跳棋位数

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
					scoreMap[i][j] = INT_MAX;  // 存在最优解，直接返回
					return;
				}

				int edgeNum = 0; // 一端的连续空白位数
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
				// 分数计算规则
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
	std::uniform_int_distribution<int> u(0, vec.size() - 1); // 左闭右闭区间
	e.seed(time(0));
	num = u(e);
	return ChessPos(vec[num][0], vec[num][1]);
}
