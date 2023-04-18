#pragma once


// 試合中の盤面保持、勝利判定を行うクラス
// Gameオブジェクトのメンバとしての使用を想定
class Field {
private:
	int filled() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (state[i][j] == 0) return 0;
			}
		}
		return 1;
	}

public:
	int state[3][3];	// 0:None, 1:Black, -1:White
	int stone1;
	int stone2;
	int stone1_t;
	int stone2_t;

	Field() {
		initialize();
	}

	void initialize() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				state[i][j] = 0;
			}
		}
	}

	void setStoneGraphs(int src1, int src2, int src1_t, int src2_t) {
		stone1 = src1;
		stone2 = src2;
		stone1_t = src1_t;
		stone2_t = src2_t;
	}

	int victory() {
		for (int k = 0; k < 3; ++k) {
			if (state[0][k] == state[1][k] && state[0][k] == state[2][k]) {
				// cout << "縦" << k << endl;
				return state[0][k];
			}
		}
		for (int k = 0; k < 3; ++k) {
			if (state[k][0] == state[k][1] && state[k][0] == state[k][2]) {
				// cout << "横" << k << endl;
				return state[k][0];
			}
		}

		if (state[0][0] == state[1][1] && state[0][0] == state[2][2]) {
			// cout << "左斜め" << endl;
			return state[0][0];
		}
		if (state[2][0] == state[1][1] && state[2][0] == state[0][2]) {
			// cout << "右斜め" << endl;
			return state[2][0];
		}
		
		if (filled()) return VICTORY_DRAW;

		return VICTORY_NONE;
	}

	int update(int i, int j, int side) {
		if (victory() != 0) return -1;
		if (state[i][j] == 0) {
			state[i][j] = side;
			return 0;
		}
		return -1;
	}

	void draw(double baseX, double baseY, double width) {
		for (int k = 0; k < 3; ++k) {
			for (int l = 0; l < 3; ++l) {
				if (state[k][l] == 1) {
					DrawExtendGraph(baseX + width * k - 15, baseY + width * l - 15, baseX + width * k + 15, baseY + width * l + 15, stone1, TRUE);
					//DrawCircleAA(baseX + width * k, baseY + width * l, 13, 12, GetColor(50, 50, 50), TRUE);
				}
				else if (state[k][l] == -1) {
					DrawExtendGraph(baseX + width * k - 15, baseY + width * l - 15, baseX + width * k + 15, baseY + width * l + 15, stone2, TRUE);
					//DrawCircleAA(baseX + width * k, baseY + width * l, 13, 12, GetColor(255, 200, 100), TRUE);
				}
			}
		}
	}

	Field operator = (const Field& src) {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				state[i][j] = src.state[i][j];
			}
		}
		return src;
	}
};

