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
		int target = 0;

		for (int k = 0; k < 3; ++k) {
			target = state[0][k];
			if (target != 0 && state[1][k] == target && state[2][k] == target) {
				return target;
			}
		}
		for (int k = 0; k < 3; ++k) {
			target = state[k][0];
			if (target != 0 && state[k][1] == target && state[k][2] == target) {
				return target;
			}
		}

		target = state[0][0];
		if (target != 0 && state[1][1] == target && state[2][2] == target) {
			return target;
		}

		target = state[2][0];
		if (target != 0 && state[1][1] == target && state[0][2] == target) {
			return target;
		}
		
		if (filled()) {
			return VICTORY_DRAW;
		}

		return VICTORY_NONE;
	}

	// リーチの個数
	int waitingCount(int side) {
		int count = 0;
		for (int k = 0; k < 3; k++) {
			int col[3] = { state[0][k], state[1][k], state[2][k] };
			count += isWaiting(col, side);
		}
		for (int k = 0; k < 3; k++) {
			int row[3] = { state[k][0], state[k][1], state[k][2] };
			count += isWaiting(row, side);
		}

		int slash1[3] = { state[0][0], state[1][1], state[2][2] };
		count += isWaiting(slash1, side);

		int slash2[3] = { state[2][0], state[1][1], state[0][2] };
		count += isWaiting(slash2, side);

		return count;
	}

	int isWaiting(int src[3], int side) {
		int seq[3][3] = {
			{ 1, 1, 0 },
			{ 1, 0, 1 },
			{ 0, 1, 1 }
		};
		for (int i = 0; i < 3; i++) {
			int degree = 0;
			for (int j = 0; j < 3; j++) {
				if (src[j] * side == seq[i][j]) degree++;
			}
			if (degree == 3) return 1;
		}
		return 0;
	}

	int update(int i, int j, int side) {
		if (victory() != 0) return -1;
		if (state[i][j] != 0) return -1;

		state[i][j] = side;
		return 0;
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

