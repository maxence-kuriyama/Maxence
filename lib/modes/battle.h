#pragma once

#include <fstream>
#include "lib/const.h"
#include "lib/mode.h"
#include "lib/utils/flag_store.h"
#include "lib/utils/user_input.h"
#include "lib/utils/com.h"
#include "lib/components/menu.h"
#include "lib/components/game.h"
#include "lib/components/logo.h"
#include "lib/components/anime/slide_cutin.h"
#include "./battle/comment.h"
#include "./battle/result.h"

const int BATTLE_MODE_BATTLE(0);
const int BATTLE_MODE_RESULT(1);

// �o�g�����[�h
// ���Game�̑���ƕ`��
// Game�̕`���Ending��BattleResult�ł��s�����߁A���J����K�v������
class Battle {
private:
	Menu menu;
	Button btnSave;
	Button btnReset;

	SlideCutin cutin;
	Comment comment;
	Logo logo;
	Result result;
	double theta = 0.3;

	int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(50, 50, 200);
	const string saveFilePath = SAVE_FILE_BATTLE_GAME;

	bool likelihoodFlg = false; // �w�K�@�B�̏o�̓t���O
	bool commentFlg = false;
	int playCnt = 0;			// 1�^�[���ɔ�₵���J�E���g
	int innerMode = BATTLE_MODE_BATTLE;

public:
	Game game;

	Battle() {
		UserInput::reset();

		game.initialize();
		comment.initialize();

		// �{�^��������
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "���f");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "�^�C�g��");
		menu.set(btnSave, btnReset);
	}

	~Battle() {}

	void start(int player1, int player2, bool init = true) {
		UserInput::reset();
		game.prepare(player1, player2, init);
	}

	void startVsHuman() {
		game.setVsHuman();
		start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
	}

	void startNewVsHuman() {
		if (hasSaveFile()) return;

		startVsHuman();
	}

	void resume(int player1, int player2) {
		start(player1, player2, false);
	}

	/*===========================*/
	//    Battle Mode
	/*===========================*/
	int show() {
		SetBackgroundColor(0, 0, 0);

		if (innerMode == BATTLE_MODE_RESULT) return showResult();

		toggleByKey();
		tick();
		game.drawBeforePlay();

		if (playByPlayer()) {
			if (game.isVsCOM()) COM::setWait();
		}
		
		VectorXd input = game.stateToInput();

		// �w�K�@�B�̏o�͕`��
		if (likelihoodFlg) {
			COM::visualize(input);
		}

		// COM�̎��
		if (!game.isPlayTurn()) {
			COM::play(input, game.board, game.currentSide());
			game.update(COM::choice);
		}

		game.drawAfterPlay();
		drawMessage();
		drawComment();
		cutin.update();
		logo.draw(true);

		// ��������
		if (game.victory() != 0) {
			UserInput::reset();
			innerMode = BATTLE_MODE_RESULT;
		}

		if (cancelPlay()) COM::setWait();
	
		if (saveOrReset()) return MODE_TITLE;

		return MODE_BATTLE;
	}

	void route(Mode& mode, int res) {
		switch (res) {
		case MODE_TITLE:
			// initialize();
			mode.goTitle();
			break;
		}
	}

	/*===========================*/
	//    Save and Load
	/*===========================*/
	bool hasSaveFile() {
		ifstream file(saveFilePath);
		if (file) {
			file.close();
			return true;
		}
		else {
			file.close();
			return false;
		}
	}

	void save() {
		game.save(saveFilePath);
	}

	void load() {
		start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
		game.load(saveFilePath);
	}

	/*===========================*/
	//    �f�o�b�O���
	/*===========================*/
	void debugDump() {
		game.debugDump();

		int strColor = strColorDebug;
		// Option
		DrawFormatString(5, 185, strColor, "playCnt: %d", playCnt);
		DrawFormatString(5, 205, strColor, "likeliFlg: %d", likelihoodFlg);
		DrawFormatString(5, 225, strColor, "commentFlg: %d", commentFlg);
		// Comment
		comment.debugDump(strColor);
	}

private:

	int showResult() {
		int res = result.show(game);

		if (res == MODE_BATTLE) {
			innerMode = BATTLE_MODE_BATTLE;
			start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}
		else if (res == MODE_RESULT_CANCEL) {
			innerMode = BATTLE_MODE_BATTLE;
			resume(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}

		return MODE_BATTLE;
	}

	void tick() {
		playCnt = (playCnt + 1) % 10000000;
	}

	bool playByPlayer() {
		if (game.isPlayTurn() && game.playTurn()) {
			double res = game.update();
			if (game.isUpdated(res)) {
				updateCommentInBattle();
				playCnt = 0;
				return true;
			}
		}
		return false;
	}

	bool cancelPlay() {
		return (UserInput::onBack() && game.goBackHist());
	}

	bool saveOrReset() {
		bool no_keyboard = true;
		int choice = menu.choose(strColor, no_keyboard);

		// save
		if (choice == 0) save();

		//reset
		if (choice == 0 || choice == 1) {
			UserInput::reset();
			game.reset(true);
			return true;
		}

		return false;
	}

	void toggleByKey() {
		// �R�����g������
		if (UserInput::onKeyComment()) {
			commentFlg = !commentFlg;
		}

		if (FlagStore::isDebug()) {
			// �J�b�g�C��������
			if (UserInput::onKeyCutinDebug()) {
				cutin.start();
			}

			// �w�K�@�B�̏o�͂�����
			if (UserInput::onKeyVisualizeDebug()) {
				likelihoodFlg = !likelihoodFlg;
			}
		}
	}

	/*===========================*/
	//    �`��n
	/*===========================*/
	void drawMessage() {
		DrawFormatString(470, 80, strColor, "�E�N���b�N:");
		DrawFormatString(540, 100, strColor, "�΂�u��");
		DrawFormatString(470, 124, strColor, "z�L�[�iBkSp�L�[�j:");
		DrawFormatString(540, 144, strColor, "���߂�");
	}

	void drawComment() {
		// �R�����g�`��
		if (commentFlg) comment.draw(strColor);
		// �R�����g�����ւ�
		comment.update(playCnt);
	}

	void updateCommentInBattle() {
		comment.forceUpdate(COMMENT_CHANGE_TYPE_PLAY_TURN, 0.40);
	}
};
