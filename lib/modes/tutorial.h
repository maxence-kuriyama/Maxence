#pragma once

#include "lib/components/menu.h"
#include "./scenario_base.h"

// チュートリアルのシナリオクラス
class Tutorial : public ScenarioBase {

public:

	Tutorial() : ScenarioBase() {
		for (int i = 0; i < MAX_SCENE_NUM; i++) {
			sceneList[i] = scenes[i];
		}
		btnReset.initialize(260, 440, "タイトル");
	}

private:
	Button btnReset;
	int strColorMenu = GetColor(255, 255, 255);

	int strColor = GetColor(255, 255, 255);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"――世界は１つの部屋で出来ている" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"チュートリアルテスト" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"まあ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"とにかく" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"バトるか" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,	"start" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"バトル画面やで" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"相手の手番" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"あなたの手番" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"local_victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"終わりまで続けましょう" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"終わり" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,	"end" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_YELLOW,	"exibit_nowait" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"おかえり" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"つづきはまだない" },
		{ -1,					-1,					"" },
	};

public:

	int show() {
		int res = ScenarioBase::show();

		return (res != SCENE_RES_DEFAULT) ? res : FLAG_TUTORIAL;
	}

	void debugDump() {
		int strColor = strColorDebug;
		Scene scene = getCurrentScene();

		DrawFormatString(365, 405, strColor, "tutoTrigger: %s", battle_trigger);
		DrawFormatString(365, 425, strColor, "tutoAction: %d", scene.action);

		// game.debugDump();
	}

private:

	// override
	void setBattle(string how) {
		if (how == "start") {
			game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			game.setTutorial();
			onBattle = true;
		}
		else if (how == "end") {
			initializeBattle();
		}
		goNext();
	}

	// override
	int doBattle(COM& com) {
		ScenarioBase::doBattle(com);

		if (reset()) return FLAG_TITLE;

		return FLAG_TUTORIAL;
	}

	bool reset() {
		btnReset.display(strColorMenu);
		if (btnReset.isClicked()) {
			UserInput::reset();
			game.reset();
			return true;
		}
		return false;
	}
};
