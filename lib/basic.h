#pragma once

//======================================
//   マウス関連
//======================================
class Mouse {
private:
	unsigned int button[8];

public:
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

	bool click() {
		return button[0] == 1;
	}

	bool onClick() {
		return button[0] > 1;
	}
	
	bool clickRight() {
		return button[1] == 1;
	}

	//矩形の上にマウスカーソルがいるか判定
	bool onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		return (x > LeftUpx && x < RightDownx && y > LeftUpy && y < RightDowny);
	}
};
