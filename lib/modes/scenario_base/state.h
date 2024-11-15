#pragma once

#include <string>
#include "lib/components/sprite.h"

using namespace std;

class State {
private:

	// ���͂�ۑ�����
	struct StickyKeyboard {
		int key;		// �㉺���E�̂�
		bool onOk;		// Enter or Space
	};

	string graph = "";		// �摜�\���C�x���g�̃t���O
	bool talking = false;	// NPC�Ɖ�b�����ۂ�
	StickyKeyboard keyboard = { SPRITE_KEY_NONE, false };

public:

	State() {
		initialize();
	}

	void initialize() {
		graph = "";
		talking = false;
		resetKeyboard();
	}

	string getGraph() {
		return graph;
	}

	void setGraph(string src) {
		graph = src;
	}

	bool isTalking() {
		return talking;
	}

	void finishTalking() {
		talking = false;
	}

	void talk() {
		talking = true;
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