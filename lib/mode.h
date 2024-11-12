#pragma once

const int MODE_OPENING(-1);
const int MODE_TITLE(0);
const int MODE_BATTLE(1);
const int MODE_RESULT(2);
const int MODE_RESULT_CANCEL(10);
const int MODE_SCENARIO(-6);
const int MODE_ENDING(-4);
const int MODE_TUTORIAL(-2);
const int MODE_MUSIC_ROOM(-8);

class Mode {
private:
	int flg = MODE_OPENING;

public:
	int current() {
		return flg;
	}

	void goTitle() {
		flg = MODE_TITLE;
	}

	void goBattle() {
		flg = MODE_BATTLE;
	}

	void goResult() {
		flg = MODE_RESULT;
	}

	void goScenario() {
		flg = MODE_SCENARIO;
	}

	void goEnding() {
		flg = MODE_ENDING;
	}

	void goTutorial() {
		flg = MODE_TUTORIAL;
	}

	void goMusicRoom() {
		flg = MODE_MUSIC_ROOM;
	}
};