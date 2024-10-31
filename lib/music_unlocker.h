#pragma once

#include <iostream>
#include "lib/nlohmann/json.hpp"
#include "lib/const.h"
#include "lib/encrypter.h"

class MusicUnlocker {
private:
	string saveFilePath = SAVE_FILE_MUSIC_ROOM;

public:
	nlohmann::json unlockedIds = nlohmann::json::array();

	MusicUnlocker() {
		load();
	}

	void unlock(const string fileName) {
		int id = getIdFromFileName(fileName);
		if (isUnlocked(id)) return;

		unlockedIds.push_back(id);
		save();
	}

	bool isUnlocked(int id) {
		for (nlohmann::json::iterator it = unlockedIds.begin(); it != unlockedIds.end(); ++it) {
			if (*it == id) return true;
		}
		return false;
	}

private:
	int getIdFromFileName(const string fileName) {
		return 1;
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
