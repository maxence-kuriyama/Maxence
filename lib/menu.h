#pragma once

#include "lib/basic.h"


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

	int choose(int keyboardFlg, Mouse& mouse, int strColor) {
		for (int i = 0; i < size; ++i) {
			button[i].display(mouse, strColor);
		}
		for (int i = 0; i < size; ++i) {
			if (button[i].isClicked(mouse)) {
				return i;
			}
		}
		return -1;
	}
};
