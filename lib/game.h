#pragma once

#include "lib/basic.h"
#include "lib/field.h"


class Game {
private:
	// 定数
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);

public:
	int cnt = 0;		// ターン数
	int nextField = -1;		// 次の盤面、-1: anywhere

	// 盤面上の操作関連
	int globalX = 1;
	int globalY = 1;
	int localX = 1;
	int localY = 1;			//キーボード操作時の座標

	Field mother;
	Field child[3][3];
	Mouse mouse;

	Game() {
		initialize();
	}

	void initialize() {
		cnt = 0;
		nextField = -1;
		mother.initialize();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				child[i][j].initialize();
			}
		}
	}


	/*===========================*/
	//    盤面表示関連
	/*===========================*/
	void drawBase() {
		DrawBox(160, 80, 460, 380, White, TRUE);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Black, FALSE);
			}
		}
	}

	void drawNextField() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (nextField == 3 * i + j) {
					for (int w = -1; w <= 1; ++w) {
						int upLeftX = 160 + 100 * i + w;
						int upLeftY = 80 + 100 * j + w;
						int lowRightX = 160 + 100 * (i + 1) - w;
						int lowRightY = 80 + 100 * (j + 1) - w;
						DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, Black, FALSE);
					}
				}
				else if (nextField == -1 && child[i][j].victory() == 0) {
					int upLeftX = 160 + 100 * i;
					int upLeftY = 80 + 100 * j;
					int lowRightX = 160 + 100 * (i + 1);
					int lowRightY = 80 + 100 * (j + 1);
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, Black, FALSE);
				}
			}
		}
	}

	void drawCurrentCoord() {
		int upLeftX = 160 + 100 * globalX + 33 * localX;
		int upLeftY = 80 + 100 * globalY + 33 * localY;
		int lowRightX = 160 + 100 * globalX + 33 * (localX + 1) + 1;
		int lowRightY = 80 + 100 * globalY + 33 * (localY + 1) + 1;
		DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, Black, FALSE);
	}


	/*===========================*/
	//    盤面更新関連
	/*===========================*/
	void getMouseCoord() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						int upLeftX = 160 + 100 * i + 33 * k;
						int upLeftY = 80 + 100 * j + 33 * l;
						int lowRightX = 160 + 100 * i + 33 * (k + 1);
						int lowRightY = 80 + 100 * j + 33 * (l + 1);
						if (mouse.onButton(upLeftX, upLeftY, lowRightX, lowRightY)) {
							globalX = i;
							globalY = j;
							localX = k;
							localY = l;
						}
					}
				}
			}
		}
	}

	bool playTurn() {
		getMouseCoord();
		if (mouse.clickRight()) {
			return true;
		}
		return false;
	}

	double update(int side = 0) {
		return update(globalX, globalY, localX, localY, side);
	}

	double update(int global_x, int global_y, int local_x, int local_y, int side = 0) {
		// sideが指定されていなければ、cntから計算する
		if (side == 0) {
			side = 1 - 2 * (cnt % 2);
		}
		//盤面の更新
		if (nextField == 3 * global_x + global_y || nextField == -1) {
			if (child[global_x][global_y].update(local_x, local_y, side) == 0) {
				cnt++;
				//全体の更新
				mother.update(global_x, global_y, child[global_x][global_y].victory());
				if (child[local_x][local_y].victory() != 0) {
					nextField = -1;
					return 0;
				}
				else {
					nextField = local_x * 3 + local_y;
					return 0;
				}
			}
		}
		return -100.0;
	}
};
