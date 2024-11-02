#pragma once

#include <iostream>
#include "lib/nlohmann/json.hpp"
#include "lib/const.h"
#include "lib/encrypter.h"

using namespace std;

class MusicUnlocker {
private:
	static MusicUnlocker* _instance; // singleton
	string saveFilePath = SAVE_FILE_MUSIC_ROOM;
	nlohmann::json unlockedIds = nlohmann::json::array();

	static MusicUnlocker* getInstance() {
		if (!_instance) {
			_instance = new MusicUnlocker();
		}
		return _instance;
	}

public:

	MusicUnlocker() {
		load();
	}

	~MusicUnlocker() {
		delete _instance;
	}

	static void unlock(const string fileName) {
		MusicUnlocker* unlocker = getInstance();
		unlocker->_unlock(fileName);
	}

	static bool isUnlocked(int id) {
		MusicUnlocker* unlocker = getInstance();
		return unlocker->_isUnlocked(id);
	}

private:

	void _unlock(const string fileName) {
		int id = getIdFromFileName(fileName);
		if (isUnlocked(id)) return;

		unlockedIds.push_back(id);
		save();
	}

	int getIdFromFileName(const string fileName) {
		return 1;
	}

	bool _isUnlocked(int id) {
		for (nlohmann::json::iterator it = unlockedIds.begin(); it != unlockedIds.end(); ++it) {
			if (*it == id) return true;
		}
		return false;
	}

	/*===========================*/
	//    Save and Load
	/*===========================*/
	void save() {
		Encrypter encrypter(saveFilePath);
		nlohmann::json data = {
			{"unlockedIds", unlockedIds},
		};
		encrypter.write(data);
	}

	void load() {
		Encrypter encrypter(saveFilePath);
		nlohmann::json res = encrypter.read();
		Logger::ss << "MusicRoom loaded: " << res.dump();
		Logger::log();

		unlockedIds = res["unlockedIds"];
	}
};

MusicUnlocker* MusicUnlocker::_instance = NULL;
