#pragma once

#include <string>
#include "lib/utils/user_input.h"
#include "lib/components/sprite.h"

using namespace std;

class State {
private:

	// 入力を保存する
	struct StickyKeyboard {
		int key;		// 上下左右のみ
		bool onOk;		// Enter or Space
	};

	string graph = "";		// 画像表示イベントのフラグ
	StickyKeyboard keyboard = { SPRITE_KEY_NONE, false };

public:

	State() {
		initialize();
	}

	void initialize() {
		graph = "";
		resetKeyboard();
	}

	string getGraph() {
		return graph;
	}

	void setGraph(string src) {
		graph = src;
	}

	int getKey() {
		return keyboard.key;
	}

	void setKey(int srcKey) {
		keyboard.key = srcKey;
	}

	void pushReturn() {
		keyboard.onOk = true;
	}

	void resetKeyboard() {
		keyboard.key = SPRITE_KEY_NONE;
		keyboard.onOk = false;
	}

	void updateKeyboard() {
		resetKeyboard();

		Key* keyboard = UserInput::getKey();
		if (keyboard->onGoingDown()) {
			setKey(SPRITE_KEY_DOWN);
		}
		else if (keyboard->onGoingRight()) {
			setKey(SPRITE_KEY_RIGHT);
		}
		else if (keyboard->onGoingUp()) {
			setKey(SPRITE_KEY_UP);
		}
		else if (keyboard->onGoingLeft()) {
			setKey(SPRITE_KEY_LEFT);
		}

		if (keyboard->onReturn()) {
			pushReturn();
		}
	}

	bool isOnReturnOrClicked() {
		Mouse* mouse = UserInput::getMouse();
		return (keyboard.onOk || mouse->click());
	}
};