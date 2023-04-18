#pragma once

#include "lib/field.h"

class Board {
private:
	Field global;
	Field local[3][3];

public:
	int nextField = -1; // 次の盤面、-1: anywhere

	Board() { initialize(); }
	~Board() {}

	void initialize() {
		global.initialize();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				local[i][j].initialize();
			}
		}
		nextField = -1;
	}
	
	void setStoneGraphs(int stone1, int stone2, int stone1_t, int stone2_t) {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				local[i][j].setStoneGraphs(stone1, stone2, stone1_t, stone2_t);
			}
		}
	}

	int globalState(int x, int y) {
		return global.state[x][y];
	}

	int victory() {
		return global.victory();
	}

	int localState(int global_x, int global_y, int x, int y) {
		return local[global_x][global_y].state[x][y];
	}

	int localVictory(int x, int y) {
		return local[x][y].victory();
	}

	void drawLocal(int x, int y, double base_x, double base_y, double width) {
		local[x][y].draw(base_x, base_y, width);
	}

	bool isNext(int x, int y) {
		return (isStrictNext(x, y) || isNextAny());
	}

	bool isStrictNext(int x, int y) {
		return (nextField == x * 3 + y);
	}

	bool isNextAny() {
		return (nextField == -1);
	}

	bool canPut(int global_x, int global_y, int x, int y) {
		return (isNext(global_x, global_y) && localState(global_x, global_y, x, y) == 0 && localVictory(global_x, global_y) == 0);
	}

	double update(int global_x, int global_y, int local_x, int local_y, int side) {
		//盤面の更新
		if (isNext(global_x, global_y)) {
			if (local[global_x][global_y].update(local_x, local_y, side) == 0) {
				//全体の更新
				global.update(global_x, global_y, localVictory(global_x, global_y));

				if (local[local_x][local_y].victory() != 0) {
					nextField = -1;
					return RWD_DOMINANT;
				}
				else {
					nextField = local_x * 3 + local_y;
					return RWD_PUT;
				}
			}
		}
		return RWD_NOT_UPDATED;
	}

	double update(int index, int side = 0) {

	}

	void goBack(int prev_info[5]) {
		int global_x = prev_info[0];
		int global_y = prev_info[1];
		int x = prev_info[2];
		int y = prev_info[3];
		int last_field = prev_info[4];

		local[global_x][global_y].state[x][y] = 0;
		global.state[global_x][global_y] = 0;
		global.update(global_x, global_y, localVictory(global_x, global_y));
		nextField = last_field;
	}
};
