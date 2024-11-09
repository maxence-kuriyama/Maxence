#pragma once

//#include "lib/const.h"
#include "lib/utils/user_input.h"

class FlagStore {
private:
	static FlagStore* _instance; // singleton
	bool debug = false;

	static FlagStore* getInstance() {
		if (!_instance) {
			_instance = new FlagStore();
		}
		return _instance;
	}

	FlagStore() {}
	~FlagStore() { delete _instance; }

public:

	static bool isDebug() {
		FlagStore* store = getInstance();
		return store->debug;
	}

	static void toggleDebug() {
		if (!UserInput::onKeyDebug()) return;

		FlagStore* store = getInstance();
		store->debug = !store->debug;
	}
};

FlagStore* FlagStore::_instance = NULL;
