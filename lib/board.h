#pragma once

#include "lib/board/field.h"
#include "lib/board/hist.h"
#include "lib/logger.h"

class Board {
private:
	Field global;
	Field local[3][3];
	History history;

public:
	int nextField = -1; // 次の盤面、-1: anywhere

	Board() { initialize(); }
	~Board() {}

	void initialize() {
		history.initialize();
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

	void drawLocal(int x, int y, double base_x, double base_y, double width) {
		local[x][y].draw(base_x, base_y, width);
	}

	static int* coordinates(int index) {
		int coord[4];
		coord[0] = (index / 27) % 3;
		coord[1] = (index / 9) % 3;
		coord[2] = (index / 3) % 3;
		coord[3] = index % 3;
		return coord;
	}

	Board operator = (const Board& src) {
		global = src.global;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				local[i][j] = src.local[i][j];
			}
		}
		nextField = src.nextField;
		return src;
	}


	/*===========================*/
	//    State getter
	/*===========================*/
	int globalState(int x, int y) {
		return global.state[x][y];
	}

	int victory() {
		return global.victory();
	}

	int localState(int global_x, int global_y, int x, int y) {
		return local[global_x][global_y].state[x][y];
	}

	int localState(int index) {
		int* coord = coordinates(index);
		return localState(coord[0], coord[1], coord[2], coord[3]);
	}

	int localVictory(int x, int y) {
		return local[x][y].victory();
	}

	int* last() {
		return history.last;
	}


	/*===========================*/
	//    Boolean
	/*===========================*/
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

	bool canPut(int index) {
		int* coord = coordinates(index);
		return canPut(coord[0], coord[1], coord[2], coord[3]);
	}

	bool canCancel() {
		return history.canCancel();
	}


	/*===========================*/
	//    Updation
	/*===========================*/
	double update(int global_x, int global_y, int local_x, int local_y, int side) {
		//盤面の更新
		if (isNext(global_x, global_y)) {
			if (local[global_x][global_y].update(local_x, local_y, side) == 0) {
				//全体の更新
				global.update(global_x, global_y, localVictory(global_x, global_y));
				loggingUpdate(global_x, global_y, local_x, local_y, side);

				if (localVictory(local_x, local_y) != 0) {
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

	double update(int index, int side) {
		int* coord = coordinates(index);
		return update(coord[0], coord[1], coord[2], coord[3], side);
	}

	void addHistory(int global_x, int global_y, int local_x, int local_y) {
		history.add(global_x, global_y, local_x, local_y, nextField);
	}

	void goBack() {
		int* prev_info = last();
		int global_x = prev_info[0];
		int global_y = prev_info[1];
		int x = prev_info[2];
		int y = prev_info[3];
		int last_field = prev_info[4];

		local[global_x][global_y].state[x][y] = 0;
		global.state[global_x][global_y] = 0;
		global.update(global_x, global_y, localVictory(global_x, global_y));
		nextField = last_field;
		loggingHistory(global_x, global_y, x, y);

		history.goBack();
	}


	/*===========================*/
	//    Logging
	/*===========================*/
	void loggingUpdate(int global_x, int global_y, int local_x, int local_y, int side) {
		stringstream ss;
		ss << "Update ==== "
			<< "G(" << global_x << "," << global_y << "), "
			<< "L(" << local_x << "," << local_y << "), "
			<< "side: " << side;
		Logger::log(ss.str());
		// loggingGlobalState();
	}

	void loggingGlobalState() {
		stringstream ss;
		ss << "Global State:" << endl;
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				ss << "  " << globalState(x, y);
			}
			ss << endl;
		}
		Logger::log(ss.str());
	}

	void loggingWholeState() {
		stringstream ss;
		ss << "Board State:" << endl;
		for (int global_y = 0; global_y < 3; global_y++) {
			for (int y = 0; y < 3; y++) {
				for (int global_x = 0; global_x < 3; global_x++) {
					for (int x = 0; x < 3; x++) {
						ss << setfill(' ') << setw(3)
							<< localState(global_x, global_y, x, y);
					}
					ss << " |";
				}
				ss << endl;
			}
			ss << "----------|----------|----------" << endl;
		}
		Logger::log(ss.str());
	}

	void loggingHistory(int global_x, int global_y, int local_x, int local_y) {
		stringstream ss;
		ss << "History ==== "
			<< "G(" << global_x << "," << global_y << "), "
			<< "L(" << local_x << "," << local_y << "), ";
		Logger::log(ss.str());
	}
};
