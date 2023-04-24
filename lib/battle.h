#pragma once

#include "lib/const.h"
#include "lib/game.h"
#include "lib/game/anime.h"
#include "lib/com.h"
#include "lib/menu.h"

class Battle {
private:
	Menu menu;
	Button btnSave;
	Button btnReset;
	Button btnAgain;

	Anime cutin;
	Camera camera;
	double theta = 0.3;

	Mouse* mouse;
	Key* key;

	int strColorDebug = GetColor(0, 0, 255);

public:
	Game game;
	Option option;
	int debugFlg = 0;

	Battle(Mouse* src_mouse, Key* src_key) {
		mouse = src_mouse;
		key = src_key;
		game.initialize(src_mouse, src_key);
		camera.initialize();
		// �{�^��������
		btnAgain.initialize(44, 44, 44 - 8, 44 - 8, 44 + 88, 44 + 24, "�������");
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

	void start(int player1, int player2, bool init = true) {
		game.goBattle(player1, player2, init);
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
		game.teban = TEBAN_SENKO;
	}

	void setKoko() {
		game.teban = TEBAN_KOKO;
	}


	/*===========================*/
	//    Battle Mode
	/*===========================*/
	void drawBeforePlay() {
		game.drawBase();
		game.drawGlobalState();
		game.drawHistLast();
		game.drawNextField();
	}

	void drawAfterPlay() {
		game.drawLocalState();
		game.drawCurrentCoord();
		game.drawPlayers();
	}

	int show(COM& com, int& scene_flg, Music& bgm) {
		int return_flg = FLAG_BATTLE;

		drawBeforePlay();
		playByPlayer(com);
		drawAfterPlay();

		// ���b�Z�[�W�̕`��
		drawMessage();

		// �w�K�@�B�̏o�͕`��
		if (option.likeliFlg >= 1) {
			com.visualize();
		}

		// COM�̎��
		if (!game.isPlayTurn()) {
			// VectorXd input = game.stateToInput();
			// com.play(input);
			com.playMinMax(game.board, game.currentSide());
			// �Ֆʂ̍X�V
			game.update(com.choice);
		}

		// �R�����g�̕`��
		game.drawComment(option.commentFlg, option.strColor);

		// �J�b�g�C���A�j���[�V����
		cutin.update();

		// ��������
		int victory = game.victory();
		if (victory != 0) {
			return_flg = FLAG_RESULT;
			key->initWait();
		}

		// ����̎�����
		cancelPlay(com);

		// �Z�[�u or ���Z�b�g
		int choice = menuChoose();
		if (choice == 0 || choice == 1) {
			game.reset(bgm);
			return_flg = FLAG_TITLE;
		}

		// �J��������
		moveCamera();

		// �ΐ�X�L�b�v�i��l�p�f�o�b�O�j
		if (skipBattle(scene_flg)) return_flg = FLAG_SCENARIO;

		return return_flg;
	}

	// �f�o�b�O�p
	bool skipBattle(int& sceneFlg) {
		if (debugFlg && game.isVsCOM()) {
			if (key->skipBattle()) {
				sceneFlg++;
				return true;
			}
		}
		return false;
	}

	void drawMessage() {
		DrawFormatString(470, 80, option.strColor, "�E�N���b�N:");
		DrawFormatString(540, 100, option.strColor, "�΂�u��");
		DrawFormatString(470, 124, option.strColor, "z�L�[�iBkSp�L�[�j:");
		DrawFormatString(540, 144, option.strColor, "���߂�");
	}


	/*===========================*/
	//    Result Mode
	/*===========================*/
	void drawForResult() {
		game.drawBase();
		game.drawGlobalState();
		game.drawHistLast();
		game.drawNextField();
		game.drawLocalState();
		game.drawWinner();
	}

	int showResult() {
		int return_flg = FLAG_RESULT;

		drawForResult();

		// ���b�Z�[�W�̕`��
		btnAgain.display(*mouse, option.strColor);
		if (btnAgain.isClicked(*mouse) || key->onReturn()) {
			return_flg = FLAG_BATTLE;
			start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}

		// ����̎�����
		if (cancelResult()) return_flg = FLAG_BATTLE;

		// �J��������
		moveCameraResult();

		// TODO: �V�i���I�̏ꍇ�̏I�� + �J�ڏ���

		return return_flg;
	}


	/*===========================*/
	//    Ending Mode
	/*===========================*/
	void drawForEnding() {
		game.drawBase();
		game.drawGlobalState();
		game.drawHistLast();
		game.drawNextField();
		game.drawLocalState();
		game.drawCurrentCoord();
	}


	/*===========================*/
	//    ����֘A
	/*===========================*/
	void playByPlayer(COM& com) {
		if (game.isPlayTurn() && game.playTurn()) {
			double res = game.update();
			if (game.isUpdated(res)) {
				if (game.isVsCOM()) {
					com.setWait();
				}
				game.updateCommentInBattle();
			}
		}
	}

	int menuChoose() {
		int keyboardFlg = 0;
		if (mouse->isUsed()) keyboardFlg = 0;
		if (key->isUsed()) keyboardFlg = 1;
		return menu.choose(keyboardFlg, *mouse, *key, option.strColor);
	}

	// TODO: �����Ȃ�Ƃ����܂��傤
	void toggleByKey(Music& music) {
		key->toggleSetting(option);
		key->toggleDebug(game.debugFlg);
		key->toggleDebug(debugFlg);
		if (debugFlg) {
			key->toggleForDebug(option, cutin.flg, game.debugEndingFlg);
		}
	}

	void moveCamera() {
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

	void moveCameraResult() {
		if (mouse->click()) {
			mouse->set();
		}
		else if (mouse->onClick()) {
			camera.move(mouse->distDragX(), mouse->distDragY());
			mouse->set();
		}
		camera.zoom(mouse->wheel);
	}

	void cancelPlay(COM& com) {
		if (key->onBack() && game.goBackHist()) {
			com.setWait();
		}
	}

	bool cancelResult() {
		if (key->onBack() && game.goBackHist()) {
			start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE, false);
			return true;
		}
		return false;
	}


	/*===========================*/
	//    �f�o�b�O���
	/*===========================*/
	void debugDump() {
		game.debugDump();

		if (debugFlg) {
			int strColor = strColorDebug;
			// Option
			DrawFormatString(125, 25, strColor, "musicFlg: %d", option.musicFlg);
			DrawFormatString(125, 45, strColor, "soundFlg: %d", option.soundFlg);
			DrawFormatString(125, 65, strColor, "likeliFlg: %d", option.likeliFlg);
			DrawFormatString(125, 85, strColor, "commentFlg: %d", option.commentFlg);
		}
	}
};
