#pragma once

//======================================
//   マウス関連
//======================================
class Mouse {
private:
	int setX = 0;
	int setY = 0;		// ドラッグ時の始点
	int preX = 0;
	int preY = 0;		// 直前のカーソル位置
	unsigned int button[8];

public:
	int x;
	int y;
	int wheel;

	//マウス状態の取得
	int update() {
		preX = x;
		preY = y;
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
		wheel = GetMouseWheelRotVol();
		return 0;
	}

	void set() {
		setX = x;
		setY = y;
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

	bool isUsed() {
		return (x != preX || y != preY);
	}

	int distDragX() {
		return x - setX;
	}

	int distDragY() {
		return y - setY;
	}
};


class Button {
private:
	unsigned int Red = GetColor(255, 0, 0);
	int textX;
	int textY;
	string text = "";

public:
	int upLeftX;
	int upLeftY;
	int lowRightX;
	int lowRightY;

	Button(int tx, int ty, int ulx, int uly, int lrx, int lry, string label = "") {
		initialize(tx, ty, ulx, uly, lrx, lry, label);
	}

	Button(int tx, int ty, string label = "") {
		initialize(tx, ty, label);
	}

	Button() {
		initialize(0, 0, 0, 0, 0, 0);
	}

	void initialize(int tx, int ty, int ulx, int uly, int lrx, int lry, string label = "") {
		textX = tx;
		textY = ty;
		upLeftX = ulx;
		upLeftY = uly;
		lowRightX = lrx;
		lowRightY = lry;
		text = label;
	}

	void initialize(int tx, int ty, string label = "") {
		initialize(tx, ty, tx - 16, ty - 16, tx + 80, ty + 24, label);
	}

	void display(Mouse& mouse, int strColor) {
		DrawFormatString(textX, textY, strColor, text.c_str());
		if (isOn(mouse)) {
			DrawFormatString(textX, textY, Red, text.c_str());
		}
	}

	// active = 0/1: inactive/active
	void display(int active, int strColor) {
		DrawFormatString(textX, textY, strColor, text.c_str());
		if (active == 1) {
			DrawFormatString(textX, textY, Red, text.c_str());
		}
	}

	bool isOn(Mouse& mouse) {
		return (mouse.x > upLeftX && mouse.x < lowRightX && mouse.y > upLeftY && mouse.y < lowRightY);
	}

	bool isClicked(Mouse& mouse) {
		if (mouse.x > upLeftX && mouse.x < lowRightX && mouse.y > upLeftY && mouse.y < lowRightY) {
			if (mouse.click()) {
				return true;
			}
		}
		return false;
	}

	Button operator = (Button& src) {
		initialize(src.textX, src.textY, src.upLeftX, src.upLeftY, src.lowRightX, src.lowRightY, src.text);
		return src;
	}
};
