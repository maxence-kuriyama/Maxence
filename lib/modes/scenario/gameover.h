#pragma once

#include "lib/components/menu.h"
#include "../scenario_base.h"

const int GAMEOVER_SAVE_X(200);
const int GAMEOVER_SAVE_Y(TEXT_SAVE_Y);
const int GAMEOVER_RESET_X(360);
const int GAMEOVER_RESET_Y(TEXT_RESET_Y);

// シナリオ管理用クラス
class GameOver : public ScenarioBase {

public:

	GameOver() : ScenarioBase() {
		imgBack = LoadGraph("graph/gameover.png");

		mrK[0].set(-100, -100, "graph/sprite11.png", 0);
		mrK[1].set(-100, -100, "graph/sprite12.png", 0);
		mrK[2].set(-100, -100, "graph/sprite13.png", 0);
		mrK[3].set(-100, -100, "graph/sprite14.png", 0);

		mrK[0].setSayings(sayings0);
		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		sceneList.initialize(scenes);

		initialize();
		msg.initialize();
	}

private:
	int who = MESSAGE_WHO_DESC; // 誰に負けたか
	bool continueFlag = false;
	int imgBack;
	unsigned int Black = GetColor(0, 0, 0);

	Menu menu;
	Button btnSave;
	Button btnReset;
	int strColorMenu = GetColor(255, 255, 255);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"unload" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_DESC,		"sound/bgm15.ogg" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"10" },
		{ SCENE_ACTION_STOP,	MESSAGE_WHO_DESC,		"" },
		{ -1,					-1,						"" },
	};
	struct Saying sayings0[8] = {
		{ "10",		MESSAGE_WHO_YELLOW,		"仕方ない" },
		{ "10",		MESSAGE_WHO_YELLOW,		"僕はこの世界と共に滅びるだけだ" },
		{ "10",		MESSAGE_WHO_YELLOW,		"また会おう" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings1[8] = {
		{ "10",		MESSAGE_WHO_BLUE,		"作戦どおり" },
		{ "10",		MESSAGE_WHO_BLUE,		"これで世界は私のもの" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings2[8] = {
		{ "10",		MESSAGE_WHO_RED,		"俺が…" },
		{ "10",		MESSAGE_WHO_RED,		"俺が正義ダァーーー！！" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings3[8] = {
		{ "10",		MESSAGE_WHO_GREEN,		"…おやすみなさい" },
		{ "10",		MESSAGE_WHO_GREEN,		"貴方の犠牲は無駄にはしませんよ…" },
		{ "999",	-1,						"" },
	};

public:

	void initialize() {
		ScenarioBase::initialize();
		who = MESSAGE_WHO_DESC;
		continueFlag = false;
		// ボタン初期化
		btnSave.initializeUsingLabelLen(GAMEOVER_SAVE_X, GAMEOVER_SAVE_Y, "コンティニュー");
		btnReset.initialize(GAMEOVER_RESET_X, GAMEOVER_RESET_Y, "タイトル");
		menu.set(btnSave, btnReset);
	}

	int show() {
		// 親の出力を上書き
		SetBackgroundColor(0, 0, 0);
		DrawBox(-1, -1, 641, 481, Black, TRUE);
		DrawExtendGraph(0, -20, 640, 460, imgBack, FALSE);

		Scene scene = sceneList.get();
		bool is_reset = (!hasMsg(scene) && continueOrReset());
		if (goingContinue()) return MODE_SCENARIO;

		int res = ScenarioBase::show();

		if (is_reset) {
			return MODE_TITLE;
		}
		return res;
	}

	void debugDump() {}

	void activate(int srcWho) {
		who = srcWho;
		startTalkMrK(who);
	}

	bool isActivated() {
		return (who != MESSAGE_WHO_DESC);
	}

	bool goingContinue() {
		return continueFlag;
	}

private:

	// override
	int checkMrK() {
		return who;
	}

	bool continueOrReset() {
		bool no_keyboard = true;
		int choice = menu.choose(strColorMenu, no_keyboard);

		// continue
		// コンティニューフラグを立てて、シナリオ側で検知
		if (choice == 0) goContinue();

		//reset
		return (choice == 1);
	}

	void goContinue() {
		continueFlag = true;
	}
};
