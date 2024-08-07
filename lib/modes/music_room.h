#pragma once

#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"

// âπäyé∫ÉÇÅ[Éh
class MusicRoom {
public:

	MusicRoom() {}

private:
	int strColorMenu = GetColor(255, 255, 255);
	int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 255, 255);

public:

	void debugDump() {
		int strColor = strColorDebug;

		//DrawFormatString(365, 405, strColor, "tutoTrigger: %s", battle_trigger);
		//DrawFormatString(365, 425, strColor, "tutoAction: %d", scene.action);
	}

private:

	bool reset(UserInput& ui) {}
};
