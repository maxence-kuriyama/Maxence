#include "DxLib.h"

using namespace DxLib;

class Mouse {
public:
	unsigned int button[8];
	int x;
	int y;

	//マウス状態の取得
	int update() {
		if (GetMousePoint(&x, &y) == -1) {
			return -1;
		}
		int MouseInput = GetMouseInput();
		for (int i = 0; i < 8; i++) {
			if (MouseInput & (1 << i)) {
				button[i]++;
			}
			else {
				button[i] = 0;
			}
		}
		return 0;
	}

	bool clickRight() {
		return button[1] == 1;
	}

	//矩形の上にマウスカーソルがいるか判定
	bool onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		return (x > LeftUpx && x < RightDownx&& y > LeftUpy && y < RightDowny);
	}
};


class Field {
public:
	int state[3][3];	// 0:None, 1:Black, -1:White

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

	int update(int i, int j, int side) {
		if (state[i][j] == 0) {
			state[i][j] = side;
			return 0;
		}
		return -1;
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
				nextField = local_x * 3 + local_y;
				return 0;
			}
		}
		return -100.0;
	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("Maxence 0.4.0");
	SetWindowIconID(101);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);


	Game game;

	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);

	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		game.mouse.update();

		game.drawBase();
		game.drawNextField();
		// プレイヤーの操作
		if (game.playTurn()) {
			game.update();
		}
		game.drawCurrentCoord();
	}

	InitGraph();
	DxLib_End();

	return 0;
}

