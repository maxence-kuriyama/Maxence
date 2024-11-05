#pragma once

#include "lib/user_input.h"


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

	void initializeUsingLabelLen(int tx, int ty, string label) {
		initialize(tx, ty, tx - 16, ty - 16, tx + 8 * label.length() + 32, ty + 24, label);
	}

	void display(Mouse& mouse, int strColor) {
		DrawFormatString(textX, textY, strColor, text.c_str());
		if (isOn(mouse)) {
			DrawFormatString(textX, textY, Red, text.c_str());
		}
	}

	void display(UserInput& ui, int strColor) {
		display(*ui.mouse, strColor);
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

	bool isClicked(UserInput& ui) {
		return isClicked(*ui.mouse);
	}

	Button operator = (Button& src) {
		initialize(src.textX, src.textY, src.upLeftX, src.upLeftY, src.lowRightX, src.lowRightY, src.text);
		return src;
	}
};


const int MENU_BUTTON_MAX_SIZE(30);

class Menu {
private:
	Button button[MENU_BUTTON_MAX_SIZE];
	int size = 0;	// ボタンの個数 (<= 4)
	int id = 0;		// どのボタンを指定しているか
	int step = 1;	// 左右ボタンのidステップ値

public:

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

	void set(Button* buttons, int menuSize) {
		for (int i = 0; i < menuSize; ++i) {
			button[i] = buttons[i];
		}
		id = 0;
		size = menuSize;
	}

	void setStep(int srcStep) {
		step = srcStep;
	}

	void display(int strColor) {
		for (int i = 0; i < size; ++i) {
			button[i].display(0, strColor);
		}
		button[id].display(1, strColor);
	}

	void toggle(Key& key) {
		if (key.onLeft()) {
			id = (id - step + size) % size;
		}
		if (key.onRight()) {
			id = (id + step) % size;
		}
		if (key.onUp()) {
			id = (id - 1 + size) % size;
		}
		if (key.onDown()) {
			id = (id + 1) % size;
		}
	}

	int choose(UserInput& ui, int strColor, bool noKeyboard = false) {
		if (!noKeyboard && ui.keyboard) {
			display(strColor);
			if (ui.key->onReturn()) {
				return id;
			}
		}
		else {
			for (int i = 0; i < size; ++i) {
				button[i].display(ui, strColor);
			}
			for (int i = 0; i < size; ++i) {
				if (button[i].isClicked(ui)) {
					return i;
				}
			}
		}
		toggle(*ui.key);
		return -1;
	}
};
