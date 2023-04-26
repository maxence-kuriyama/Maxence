#pragma once

#include "lib/const.h"
#include "lib/com.h"
#include "lib/menu.h"
#include "lib/battle/game.h"
#include "lib/battle/anime.h"
#include "lib/battle/comment.h"

class Battle {
private:
	Menu menu;
	Button btnSave;
	Button btnReset;
	Button btnAgain;

	Anime cutin;
	Camera camera;
	Comment comment;
	double theta = 0.3;

	Mouse* mouse;
	Key* key;

	int White = GetColor(255, 255, 255);
	int Black = GetColor(0, 0, 0);
	int strColor = White;
	int strColorDebug = GetColor(0, 0, 255);
	bool likelihoodFlg = false; // �w�K�@�B�̏o�̓t���O
	bool commentFlg = false;

public:
	Game game;

	Battle(Mouse* src_mouse, Key* src_key) {
		mouse = src_mouse;
		key = src_key;
		game.initialize(src_mouse, src_key);
		camera.initialize();
		comment.initialize();
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

	int show(COM& com, int& scene_flg, Music& bgm, bool debug) {
		int return_flg = FLAG_BATTLE;

		drawBeforePlay();
		playByPlayer(com);
		drawAfterPlay();

		// ���b�Z�[�W�̕`��
		drawMessage();

		// �w�K�@�B�̏o�͕`��
		if (likelihoodFlg) {
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
		drawComment();

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
		if (debug && skipBattle(scene_flg)) return_flg = FLAG_SCENARIO;

		return return_flg;
	}

	// �f�o�b�O�p
	bool skipBattle(int& sceneFlg) {
		if (game.isVsCOM()) {
			if (key->state[KEY_INPUT_B] == 1) {
				StopMusic();
				sceneFlg++;
				return true;
			}
		}
		return false;
	}

	void drawMessage() {
		DrawFormatString(470, 80, strColor, "�E�N���b�N:");
		DrawFormatString(540, 100, strColor, "�΂�u��");
		DrawFormatString(470, 124, strColor, "z�L�[�iBkSp�L�[�j:");
		DrawFormatString(540, 144, strColor, "���߂�");
	}

	void drawComment() {
		// �R�����g�`��
		if (commentFlg) {
			comment.draw(strColor);
		}
		// �R�����g�����ւ�
		comment.update(game.playCnt);
	}

	void updateCommentInBattle() {
		comment.forceUpdate(COMMENT_CHANGE_TYPE_PLAY_TURN, 0.40);
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
		btnAgain.display(*mouse, strColor);
		if (btnAgain.isClicked(*mouse) || key->onReturn()) {
			return_flg = FLAG_BATTLE;
			start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}

		// �R�����g�̕`��
		drawComment();

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
				updateCommentInBattle();
			}
		}
	}

	int menuChoose() {
		int keyboardFlg = 0;
		if (mouse->isUsed()) keyboardFlg = 0;
		if (key->isUsed()) keyboardFlg = 1;
		return menu.choose(keyboardFlg, *mouse, *key, strColor);
	}

	void toggleByKey(bool debug) {
		// �R�����g������
		if (key->state[KEY_INPUT_K] == 1) {
			commentFlg = !commentFlg;
		}

		// �����F�̕ύX
		if (key->state[KEY_INPUT_I] == 1) {
			toggleStrColor();
		}

		if (debug) {
			// �J�b�g�C��������
			if (key->state[KEY_INPUT_C] == 1) {
				cutin.flg = 1;
			}

			// �w�K�@�B�̏o�͂�����
			if (key->state[KEY_INPUT_V] == 1) {
				likelihoodFlg = !likelihoodFlg;
			}
		}
	}

	void toggleStrColor() {
		if (strColor == Black) {
			strColor = White;
		}
		else {
			strColor = Black;
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

		int strColor = strColorDebug;
		// Option
		DrawFormatString(125, 65, strColor, "likeliFlg: %d", likelihoodFlg);
		DrawFormatString(125, 85, strColor, "commentFlg: %d", commentFlg);
		// Comment
		comment.debugDump(strColor);
	}
};
