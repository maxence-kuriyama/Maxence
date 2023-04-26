#pragma once

#include "lib/mouse.h"
#include "lib/keyboard.h"


// メニュー画面管理用クラス
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
		return (mouse.x > upLeftX && mouse.x < lowRightX&& mouse.y > upLeftY && mouse.y < lowRightY);
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
			}
			for (int i = 0; i < size; ++i) {
				if (button[i].isClicked(mouse)) {
					return i;
				}
			}
		}
		toggle(key);
		return -1;
	}
};
