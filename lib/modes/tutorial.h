#pragma once

#include "lib/modes/common/scenario_base.h"
#include "lib/modes/common/game.h"

// �`���[�g���A���̃V�i���I�N���X
class Tutorial : public ScenarioBase {

public:

	Tutorial() : ScenarioBase() {
		for (int i = 0; i < MAX_SCENE_NUM; i++) {
			sceneList[i] = scenes[i];
		}
	}

private:
	Game game;
	int strColor = GetColor(255, 255, 255);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�\�\���E�͂P�̕����ŏo���Ă���" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�`���[�g���A���e�X�g" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�܂�" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�Ƃɂ���" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�o�g�邩" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�Â��͂܂��Ȃ�" },
		{ -1,					-1,					"" },
	};

public:

	void initialize() {
		game.initialize();
		game.setVsCOM();
		game.setSenko();
	}

	int show(UserInput& ui, Music& music) {
		int res = ScenarioBase::show(ui, music);
		if (res == SCENE_RES_GO_BATTLE) {
			game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			showGame(ui);
		}

		return SCENE_RES_DEFAULT;
	}

private:

	int showGame(UserInput& ui) {
		int return_flg = FLAG_BATTLE;

		game.drawBeforePlay();
		playByPlayer(ui);
		game.drawAfterPlay();

		return return_flg;
	}

	bool playByPlayer(UserInput& ui) {
		if (game.isPlayTurn() && game.playTurn(ui)) {
			double res = game.update();
			if (game.isUpdated(res)) {
				return true;
			}
		}
		return false;
	}
};
