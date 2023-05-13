#pragma once

#include "lib/com.h"
#include "lib/modes/common/scenario_base.h"
#include "lib/modes/common/game.h"


const int SCENE_ACTION_PLAY(21);

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
	bool onGame = false;
	int strColor = GetColor(255, 255, 255);
	string trigger = "";

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�\�\���E�͂P�̕����ŏo���Ă���" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�`���[�g���A���e�X�g" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�܂�" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�Ƃɂ���" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�o�g�邩" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_YELLOW,	"hide_nowait" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_RED,		"hide_nowait" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_GREEN,	"hide_nowait" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�o�g����ʂ��" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"����̎��" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"���Ȃ��̎��" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�Â��͂܂��Ȃ�" },
		{ -1,					-1,					"" },
	};

public:

	void initialize() {
		onGame = false;
		game.initialize();
		trigger = "";
	}

	int show(UserInput& ui, Music& music) {
		if (onGame) showGame();

		int res = ScenarioBase::show(ui, music);
		if (res == SCENE_RES_GO_BATTLE) {
			game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			game.setTutorial();
			onGame = true;
			goNext();
		}

		return showAdditionalAction(ui);
	}

	void debugDump() {
		int strColor = strColorDebug;
		Scene scene = getCurrentScene();

		DrawFormatString(365, 405, strColor, "trigger: %s", trigger);
		DrawFormatString(365, 425, strColor, "tutoAction: %d", scene.action);

		game.debugDump();
	}

private:

	int showAdditionalAction(UserInput& ui) {
		Scene scene = getCurrentScene();

		switch (scene.action) {
		case SCENE_ACTION_PLAY:
			doGame(ui);
			break;
		default:
			break;
		}

		return SCENE_RES_DEFAULT;
	}

	// override
	void setTrigger(string src_trigger) {
		trigger = src_trigger;
		goNext();
	}

	// override
	bool isTriggered() {
		if (trigger == "") return true;

		if (trigger == "play_once") {
			// do nothing here (see doGame)
		}
		return false;
	}

	void solveTrigger() {
		trigger = "";
	}

	void showGame() {
		game.drawForPlay();
	}

	void doGame(UserInput& ui) {
		if (playByPlayer(ui) || playByCom()) {
			if (trigger == "play_once") solveTrigger();
		}

		// ��������
		if (game.victory() != 0) {
			ui.reset();
		}

		if (isTriggered()) goNext();
	}

	bool playByPlayer(UserInput& ui) {
		if (!game.isPlayTurn()) return false;

		if (!game.playTurn(ui)) return false;

		double res = game.update();
		return game.isUpdated(res);
	}

	bool playByCom() {
		if (game.isPlayTurn()) return false;

		MinMaxNode node(game.board, game.currentSide());
		int depth = 3;
		int index = node.search(depth);
		Coordinate choice = Board::coordinates(index);

		double res = game.update(choice);
		return game.isUpdated(res);
	}
};
