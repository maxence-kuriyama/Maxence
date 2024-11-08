#pragma once

#include "lib/user_input/mouse.h"
#include "lib/user_input/keyboard.h"

class UserInput {
private:
	static UserInput* _instance; // singleton
	bool keyboard = false;
	Key* key;
	Mouse* mouse;

	static UserInput* getInstance() {
		if (!_instance) {
			_instance = new UserInput();
		}
		return _instance;
	}

	UserInput() {}
	~UserInput() { delete _instance; }

public:

	static Key* getKey() {
		UserInput* ui = getInstance();
		return ui->key;
	}

	static Mouse* getMouse() {
		UserInput* ui = getInstance();
		return ui->mouse;
	}

	static bool usingKeyboard() {
		UserInput* ui = getInstance();
		return ui->keyboard;
	}

	void update() {
		key->update();
		mouse->update();
		toggleMouseOrKeyboard();
	}

	static void reset() {
		UserInput* ui = getInstance();
		ui->mouse->set();
		ui->key->initWait();
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

	static bool check() {
		UserInput* ui = getInstance();
		if (ui->keyboard) {
			return ui->key->onCheck();
		}
		else {
			return ui->mouse->clickRight();
		}
		return false;
	}

	static bool clickOrReturn() {
		UserInput* ui = getInstance();
		return (ui->key->onReturn() || ui->mouse->click());
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

UserInput* UserInput::_instance = NULL;
