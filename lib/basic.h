#pragma once

#include "lib/keyboard.h"

//======================================
//   マウス関連
//======================================

class Mouse {
public:
	int x;
	int y;
	int setX = 0;
	int setY = 0;		// ドラッグ時の始点
	int preX = 0;
	int preY = 0;		// 直前のカーソル位置
	int wheel;
	unsigned int button[8];

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
public:
	int textX;
	int textY;
	int upLeftX;
	int upLeftY;
	int lowRightX;
	int lowRightY;
	string text = "";
	unsigned int Red = GetColor(255, 0, 0);

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


// MouseとKeyの入力を統合
class Menu {
public:
	Button button[4];
	int size = 0;	// ボタンの個数 (<= 4)
	int id = 0;		// どのボタンを指定しているか

	Menu() {
		size = 0;
		id = 0;
	}

	Menu(Button& button0, Button& button1) {
		set(button0, button1);
	}

	void set(Button& button0, Button& button1) {
		button[0] = button0;
		button[1] = button1;
		id = 0;
		size = 2;
	}

	void display(int strColor) {
		for (int i = 0; i < size; ++i) {
			button[i].display(0, strColor);
		}
		button[id].display(1, strColor);
	}

	void toggle(Key& key) {
		if (key.onLeft()) {
			id = (id - 1 + size) % size;
		}
		if (key.onRight()) {
			id = (id + 1) % size;
		}
	}

	int choose(int keyboardFlg, Mouse& mouse, Key& key, int strColor) {
		if (keyboardFlg) {
			display(strColor);
			if (key.onReturn()) {
				return id;
			}
		}
		else {
			for (int i = 0; i < size; ++i) {
				button[i].display(mouse, strColor);
				if (button[i].isClicked(mouse)) {
					return i;
				}
			}
		}
		toggle(key);
		return -1;
	}
};



//======================================
//   カメラ関連
//======================================


class Camera {
public:
	VECTOR pos;
	VECTOR origin = VGet(320.0, 240.0, 0.0);

	void initialize() {
		pos.x = 320.0;
		pos.y = 240.0;
		pos.z = -300.0;
	}

	void move(double dx, double dy) {
		VECTOR temp1 = VSub(pos, origin);
		VECTOR temp2 = VNorm(temp1);
		pos.y += 2.0 * dy / cosh((double)temp2.y);
		pos.x = origin.x + temp1.x * cos(dx / 100.0) + temp1.z * sin(dx / 100.0);
		pos.z = origin.z - temp1.x * sin(dx / 100.0) + temp1.z * cos(dx / 100.0);
		set();
	}

	void set() {
		SetCameraPositionAndTarget_UpVecY(pos, origin);
	}

	VECTOR direction() {
		return VNorm(VSub(pos, origin));
	}

	void zoom(int wheel) {
		VECTOR temp = direction();
		pos.x -= temp.x * wheel * 5.0;
		pos.z -= temp.z * wheel * 5.0;
		set();
	}
};

