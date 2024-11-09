#pragma once

//#include "lib/const.h"
#include "lib/utils/user_input.h"

const int FLAG_MODE_OPENING(-1);
const int FLAG_MODE_TITLE(0);
const int FLAG_MODE_BATTLE(1);
const int FLAG_MODE_RESULT(2);
const int FLAG_MODE_RESULT_CANCEL(10);
const int FLAG_MODE_SCENARIO(-6);
const int FLAG_MODE_ENDING(-4);
const int FLAG_MODE_TUTORIAL(-2);
const int FLAG_MODE_MUSIC_ROOM(-8);

class FlagStore {
private:
	static FlagStore* _instance; // singleton
	bool debug = false;
	int mode = FLAG_MODE_OPENING;

	static FlagStore* getInstance() {
		if (!_instance) {
			_instance = new FlagStore();
		}
		return _instance;
	}

	FlagStore() {}
	~FlagStore() { delete _instance; }

public:

	/*===========================*/
	//    デバッグモード
	/*===========================*/
	static bool isDebug() {
		FlagStore* store = getInstance();
		return store->debug;
	}

	static void toggleDebug() {
		if (!UserInput::onKeyDebug()) return;

		FlagStore* store = getInstance();
		store->debug = !store->debug;
	}

	/*===========================*/
	//    モード管理
	/*===========================*/
	static int getMode() {
		FlagStore* store = getInstance();
		return store->mode;
	}

	static void goTitle() {
		FlagStore* store = getInstance();
		store->mode = FLAG_MODE_TITLE;
	}

	/*===========================*/
	//    boolean
	/*===========================*/
	static bool isOpening() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_OPENING);
	}

	static bool isTitle() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_TITLE);
	}

	static bool isTutorial() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_TUTORIAL);
	}

	static bool isBattle() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_BATTLE);
	}

	static bool isResult() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_RESULT);
	}

	static bool isEnding() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_ENDING);
	}

	static bool isScenario() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_SCENARIO);
	}

	static bool isMusicRoom() {
		FlagStore* store = getInstance();
		return (store->mode == FLAG_MODE_MUSIC_ROOM);
	}
};

FlagStore* FlagStore::_instance = NULL;
