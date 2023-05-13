#pragma once

#include "lib/const.h"
#include "lib/com.h"
#include "lib/menu.h"
#include "lib/modes/common/game.h"
#include "lib/modes/battle/camera.h"
#include "lib/modes/battle/anime.h"
#include "lib/modes/battle/comment.h"

// �o�g�����[�h
// ���Game�̑���ƕ`��
// Game�̕`���Ending��BattleResult�ł��s�����߁A���J����K�v������
class Battle {
private:
	Menu menu;
	Button btnSave;
	Button btnReset;

	Anime cutin;
	Camera camera;
	Comment comment;
	UserInput* ui;
	double theta = 0.3;

	int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(50, 50, 200);
	bool likelihoodFlg = false; // �w�K�@�B�̏o�̓t���O
	bool commentFlg = false;
	int playCnt = 0;			// 1�^�[���ɔ�₵���J�E���g

public:
	Game game;

	Battle(UserInput* src_ui) {
		ui = src_ui;
		ui->reset();
		game.initialize();
		camera.initialize();
		comment.initialize();
		// �{�^��������
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "���f");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "�^�C�g��");
		// �J�b�g�C���摜������
		int Cutin1 = LoadGraph("graph/cutin1.png");
		int Cutin10 = LoadGraph("graph/cutin10.png");
		GraphBlend(Cutin1, Cutin10, 255, DX_GRAPH_BLEND_MULTIPLE);
		cutin.image0 = Cutin1;
		cutin.image1 = Cutin10;
	}

	~Battle() {}

	void tick() {
		playCnt = (playCnt + 1) % 10000000;
	}

	void start(int player1, int player2, bool init = true) {
		ui->reset();
		game.prepare(player1, player2, init);
		menu.set(btnSave, btnReset);
	}

	void startVsHuman() {
		game.setVsHuman();
		start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
	}

	void setVsCOM() {
		game.setVsCOM();
	}

	void setSenko() {
		game.setSenko();
	}

	void setKoko() {
		game.setKoko();
	}


	/*===========================*/
	//    Battle Mode
	/*===========================*/
	int show(COM& com, Music& bgm, bool debug) {
		int return_flg = FLAG_BATTLE;

		game.drawBeforePlay();

		if (playByPlayer()) {
			if (game.isVsCOM()) com.setWait();
		}

		// �w�K�@�B�̏o�͕`��
		if (likelihoodFlg) {
			VectorXd input = game.stateToInput();
			com.visualize(input);
		}

		// COM�̎��
		if (!game.isPlayTurn()) {
			VectorXd input = game.stateToInput();
			com.play(input);
			//com.playMinMax(game.board, game.currentSide());
			game.update(com.choice);
		}

		game.drawAfterPlay();
		drawMessage();
		drawComment();
		cutin.update();

		// ��������
		if (game.victory() != 0) {
			ui->reset();
			return_flg = FLAG_RESULT;
		}

		cancelPlay(com);

		// �Z�[�u or ���Z�b�g
		int choice = menuChoose();
		if (choice == 0 || choice == 1) {
			ui->reset();
			game.reset(bgm);
			return_flg = FLAG_TITLE;
		}

		moveCamera();

		// �ΐ�X�L�b�v�i��l�p�f�o�b�O�j
		if (debug && skipBattle()) return_flg = FLAG_SCENARIO;

		return return_flg;
	}

	// �f�o�b�O�p
	bool skipBattle() {
		return (game.isVsCOM() && ui->onKeySkipDebug());
	}

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


	/*===========================*/
	//    ����֘A
	/*===========================*/
	bool playByPlayer() {
		if (game.isPlayTurn() && game.playTurn(*ui)) {
			double res = game.update();
			if (game.isUpdated(res)) {
				updateCommentInBattle();
				playCnt = 0;
				return true;
			}
		}
		return false;
	}

	int menuChoose() {
		bool no_keyboard = true;
		return menu.choose(*ui, strColor, no_keyboard);
	}

	void toggleByKey(bool debug) {
		// �R�����g������
		if (ui->onKeyComment()) {
			commentFlg = !commentFlg;
		}

		if (debug) {
			// �J�b�g�C��������
			if (ui->onKeyCutinDebug()) {
				cutin.flg = 1;
			}

			// �w�K�@�B�̏o�͂�����
			if (ui->onKeyVisualizeDebug()) {
				likelihoodFlg = !likelihoodFlg;
			}
		}
	}

	void moveCamera() {
		Mouse* mouse = ui->mouse;
		camera.set();
		if (mouse->click()) {
			mouse->set();
		}
		if (mouse->onClick()) {
			theta -= (mouse->distDragX()) * 0.05;
			mouse->set();
		}
		//MV1SetRotationXYZ(ModelHandle, VGet(0.0, theta + DX_PI_F, 0.0));
	}

	void cancelPlay(COM& com) {
		if (ui->onBack() && game.goBackHist()) {
			com.setWait();
		}
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
};
