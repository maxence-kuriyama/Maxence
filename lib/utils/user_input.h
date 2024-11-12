#pragma once

#include "lib/const.h"
#include "./user_input/mouse.h"
#include "./user_input/keyboard.h"

const int UI_KEY_SOUND(KEY_INPUT_P);
const int UI_KEY_COMMENT(KEY_INPUT_K);

const int UI_KEY_DEBUG_MODE(KEY_INPUT_G);
const int UI_KEY_ENDING_DEBUG(KEY_INPUT_MINUS);
const int UI_KEY_SKIP_DEBUG(KEY_INPUT_B);
const int UI_KEY_CUTIN_DEBUG(KEY_INPUT_C);
const int UI_KEY_VISUALIZE_DEBUG(KEY_INPUT_V);

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

	UserInput() {
		key = new Key();
		mouse = new Mouse();
	}

	~UserInput() {
		delete key;
		delete mouse;
	}

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

	static void update() {
		UserInput* ui = getInstance();
		ui->key->update();
		ui->mouse->update();
		ui->toggleMouseOrKeyboard();
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

	static bool onKeySound() {
		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_SOUND] == 1);
	}

	static bool onKeyComment() {
		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_COMMENT] == 1);
	}

	/*===========================*/
	//    Debug—p
	/*===========================*/
	static bool onKeyDebug() {
		if (!DEBUG) return false;

		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_DEBUG_MODE] == 1);
	}

	static bool onKeyEndingDebug() {
		if (!DEBUG) return false;

		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_ENDING_DEBUG] == 1);
	}

	static bool onKeySkipDebug() {
		if (!DEBUG) return false;

		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_SKIP_DEBUG] == 1);
	}

	static bool onKeyCutinDebug() {
		if (!DEBUG) return false;

		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_CUTIN_DEBUG] == 1);
	}

	static bool onKeyVisualizeDebug() {
		if (!DEBUG) return false;

		UserInput* ui = getInstance();
		return (ui->key->state[UI_KEY_VISUALIZE_DEBUG] == 1);
	}
};

UserInput* UserInput::_instance = NULL;
