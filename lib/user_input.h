#pragma once

#include "lib/user_input/mouse.h"
#include "lib/user_input/keyboard.h"

class UserInput {
public:
	Key* key;
	Mouse* mouse;
	bool keyboard = false;

	void update() {
		key->update();
		mouse->update();
		toggleMouseOrKeyboard();
	}

	void reset() {
		mouse->set();
		key->initWait();
	}

	void toggleMouseOrKeyboard() {
		if (mouse->isUsed()) keyboard = false;
		if (key->isUsed()) keyboard = true;
	}

	bool onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		return mouse->onButton(LeftUpx, LeftUpy, RightDownx, RightDowny);
	}

	bool onBack() {
		return key->onBack();
	}

	bool onReturn() {
		return key->onReturn();
	}

	bool check() {
		if (keyboard) {
			if (key->onCheck()) {
				return true;
			}
		}
		else {
			if (mouse->clickRight()) {
				return true;
			}
		}
		return false;
	}

	bool clickOrReturn() {
		return (key->onReturn() || mouse->click());
	}

	bool onKeySound() {
		return (key->state[KEY_INPUT_P] == 1);
	}

	bool onKeyComment() {
		return (key->state[KEY_INPUT_K] == 1);
	}

	bool onKeyDebug() {
		return (key->state[KEY_INPUT_G] == 1);
	}

	bool onKeyEndingDebug() {
		return (key->state[KEY_INPUT_MINUS] == 1);
	}

	bool onKeySkipDebug() {
		return (key->state[KEY_INPUT_B] == 1);
	}

	bool onKeyCutinDebug() {
		return (key->state[KEY_INPUT_C] == 1);
	}

	bool onKeyVisualizeDebug() {
		return (key->state[KEY_INPUT_V] == 1);
	}
};