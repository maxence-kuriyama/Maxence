#pragma once

#include "lib/board/field.h"
#include "lib/board/hist.h"
#include "lib/const.h"
#include "lib/logger.h"


const int DUMMY_LAST_FIELD(-1);

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

	static Coordinate coordinates(int index) {
		Coordinate c;
		c.global_x = (index / 27) % 3;
		c.global_y = (index / 9) % 3;
		c.x = (index / 3) % 3;
		c.y = index % 3;
		return c;
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

	int localState(Coordinate& c) {
		return localState(c.global_x, c.global_y, c.x, c.y);
	}

	int localState(int index) {
		Coordinate c = coordinates(index);
		return localState(c);
	}

	int localVictory(int x, int y) {
		return local[x][y].victory();
	}

	Coordinate last() {
		return history.last();
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

	bool canPut(Coordinate& c) {
		return canPut(c.global_x, c.global_y, c.x, c.y);
	}

	bool canPut(int index) {
		Coordinate c = coordinates(index);
		return canPut(c);
	}

	bool canCancel() {
		return history.canCancel();
	}


	/*===========================*/
	//    Updation
	/*===========================*/
	double update(int global_x, int global_y, int local_x, int local_y, int side, bool logging = true) {
		//盤面の更新
		if (isNext(global_x, global_y)) {
			if (local[global_x][global_y].update(local_x, local_y, side) == 0) {
				//全体の更新
				global.update(global_x, global_y, localVictory(global_x, global_y));
				if (logging) loggingUpdate(global_x, global_y, local_x, local_y, side);
				//履歴を残す
				addHistory(global_x, global_y, local_x, local_y);

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

	double update(Coordinate& c, int side, bool logging = true) {
		return update(c.global_x, c.global_y, c.x, c.y, side, logging);
	}

	double update(int index, int side, bool logging = true) {
		Coordinate c = coordinates(index);
		return update(c, side, logging);
	}

	void addHistory(int global_x, int global_y, int local_x, int local_y) {
		history.add(global_x, global_y, local_x, local_y, nextField);
	}

	void goBack() {
		Coordinate c = last();

		local[c.global_x][c.global_y].state[c.x][c.y] = 0;
		global.state[c.global_x][c.global_y] = 0;
		global.update(c.global_x, c.global_y, localVictory(c.global_x, c.global_y));
		nextField = c.last_field;
		loggingHistory(c);

		history.goBack();
	}


	/*===========================*/
	//    Logging
	/*===========================*/
	void loggingUpdate(int global_x, int global_y, int local_x, int local_y, int side) {
		Logger::ss << "Update ==== "
			<< "G(" << global_x << "," << global_y << "), "
			<< "L(" << local_x << "," << local_y << "), "
			<< "side: " << side;
		Logger::log();
		// loggingGlobalState();
	}

	void loggingGlobalState() {
		Logger::ss << "Global State:" << endl;
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				Logger::ss << "  " << globalState(x, y);
			}
			Logger::ss << endl;
		}
		Logger::log();
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

	void loggingHistory(Coordinate& c) {
		Logger::ss << "History ==== "
			<< "G(" << c.global_x << "," << c.global_y << "), "
			<< "L(" << c.x << "," << c.y << "), "
			<< "last_field: " << c.last_field;
		Logger::log();
	}
};
