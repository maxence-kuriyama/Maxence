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

	void toggleMouseOrKeyboard() {
		if (mouse->isUsed()) keyboard = false;
		if (key->isUsed()) keyboard = true;
	}

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

	static bool onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		UserInput* ui = getInstance();
		return ui->mouse->onButton(LeftUpx, LeftUpy, RightDownx, RightDowny);
	}

	static bool isClicked() {
		UserInput* ui = getInstance();
		return ui->mouse->click();
	}

	static bool onBack() {
		UserInput* ui = getInstance();
		return ui->key->onBack();
	}

	static bool onReturn() {
		UserInput* ui = getInstance();
		return ui->key->onReturn();
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

	static bool onKeyComment() {
		UserInput* ui = getInstance();
		return (ui->key->state[KEY_INPUT_K] == 1);
	}

	/*===========================*/
	//    Debug—p
	/*===========================*/
	bool onKeyDebug() {
		return (key->state[KEY_INPUT_G] == 1);
	}

	bool onKeyEndingDebug() {
		return (key->state[KEY_INPUT_MINUS] == 1);
	}

	static bool onKeySkipDebug() {
		UserInput* ui = getInstance();
		return (ui->key->state[KEY_INPUT_B] == 1);
	}

	static bool onKeyCutinDebug() {
		UserInput* ui = getInstance();
		return (ui->key->state[KEY_INPUT_C] == 1);
	}

	static bool onKeyVisualizeDebug() {
		UserInput* ui = getInstance();
		return (ui->key->state[KEY_INPUT_V] == 1);
	}
};

UserInput* UserInput::_instance = NULL;
