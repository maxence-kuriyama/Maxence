#pragma once

#include "lib/logo.h"


// �Q�[���̃I�v�V����
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ă̎g�p��z��
// �L�[���͂ŕύX����@��������߁AKey�N���X�Ƃ܂Ƃ߂Ă���
class Option {
public:
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);

public:
	int strColor = White;
	int musicFlg = 0;
	int soundFlg = 0;
	int likeliFlg = 0;		// �w�K�@�B�̏o�̓t���O 0: ��\��, 1: �F�ŕ\��, 2: ���l���\��
	int commentFlg = 0;

	void toggleStrColor() {
		if (strColor == Black) {
			strColor = White;
		}
		else {
			strColor = Black;
		}
	}
};


// �L�[���͂ɑ΂��鏈�����s���N���X
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ďg�p�i�P�̂Ŏg�p���ׂ����H�j
class Key {
public:
	int state[256];
	int waitConst = 3;		// �Q�[�����̃L�[���̓E�F�C�g
	int waitCnt = 3;		// �L�[���̓E�F�C�g�̃J�E���^
	int use = 0;

	void setWait(int wait) {
		waitConst = wait;
	}

	void initWait() {
		waitCnt = waitConst;
	}

public:

	int update() {
		char temp[256];
		GetHitKeyStateAll(temp);
		for (int i = 0; i < 256; i++) {
			if (temp[i] != 0) {
				state[i]++;
			}
			else {
				state[i] = 0;
			}
		}
		if (waitCnt > 0) {
			waitCnt--;
		}
		return 0;
	}

	bool onReturn() {
		return (state[KEY_INPUT_RETURN] == 1 || state[KEY_INPUT_SPACE] == 1);
	}

	bool onBack() {
		return (state[KEY_INPUT_Z] == 1 || state[KEY_INPUT_BACK] == 1);
	}

	// �Q�[�����̃L�[����
	bool onCheck() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RETURN] == 1 || state[KEY_INPUT_SPACE] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}

	bool onUp() {
		if (waitCnt <= 0 && (state[KEY_INPUT_UP]== 1 || state[KEY_INPUT_W] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingUp() {
		return (state[KEY_INPUT_UP] > 0 || state[KEY_INPUT_W] > 0);
	}

	bool onDown() {
		if (waitCnt <= 0 && (state[KEY_INPUT_DOWN] == 1 || state[KEY_INPUT_S] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingDown() {
		return (state[KEY_INPUT_DOWN] > 0 || state[KEY_INPUT_S] > 0);
	}

	bool onLeft() {
		if (waitCnt <= 0 && (state[KEY_INPUT_LEFT] == 1 || state[KEY_INPUT_A] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingLeft() {
		return (state[KEY_INPUT_LEFT] > 0 || state[KEY_INPUT_A] > 0);
	}

	bool onRight() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RIGHT] == 1 || state[KEY_INPUT_D] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingRight() {
		return (state[KEY_INPUT_RIGHT] > 0 || state[KEY_INPUT_D] > 0);
	}

	bool isUsed() {
		if (use > 0) {
			use = 0;
			return true;
		}
		return false;
	}

	void toggleSetting(Option& option, Logo& logo) {
		// ���S�𓮂���
		if (state[KEY_INPUT_AT] == 1) {
			if (logo.acRate >= 0.5) {
				logo.initialize();
			}
			else {
				logo.acRate += 1.0;
			}
		}

		// �����F�̕ύX
		if (state[KEY_INPUT_I] == 1) {
			option.toggleStrColor();
		}

		//���y, SE�̗L��
		if (state[KEY_INPUT_P] == 1) {
			option.musicFlg = (option.musicFlg + 1) % 2;
			option.soundFlg = (option.soundFlg + 1) % 2;
		}

		// �R�����g������
		if (state[KEY_INPUT_K] == 1) {
			option.commentFlg = (option.commentFlg + 1) % 2;
		}
	}

	void configLearning() {
		// �f�[�^�̕ۑ�
		if (state[KEY_INPUT_O] == 1) {
			//save parameters
		}

		// AI�̃E�F�C�g��ύX����
		/*
		if (state[KEY_INPUT_M] == 1) {
			waitOnCOM++;
		}
		else if (state[KEY_INPUT_N] == 1) {
			waitOnCOM--;
		}
		*/
	}

	void toggleDebug(int& debugFlg) {
		//�f�o�b�O���[�h����
		if (state[KEY_INPUT_G] == 1) {
			debugFlg = (debugFlg + 1) % 2;
		}
	}

	void toggleForDebug(Option& option, int& cutinFlg, int& endingFlg) {
		// �J�b�g�C��������
		if (state[KEY_INPUT_C] == 1) {
			cutinFlg = 1;
		}

		// �w�K�@�B�̏o�͂�����
		if (state[KEY_INPUT_V] == 1) {
			option.likeliFlg = (option.likeliFlg + 1) % 3;
		}

		// �G���f�B���O���[�h
		if (state[KEY_INPUT_MINUS] == 1) {
			endingFlg = 1;
		}
	}

	void skipBattle(int& gameFlg, int& sceneFlg) {
		if (state[KEY_INPUT_B] == 1) {
			StopMusic();
			sceneFlg++;
			gameFlg = -6;
		}
	}

	int skipMovie(int movieHandle) {
		if (state[KEY_INPUT_W] == 1) {
			PauseMovieToGraph(movieHandle);
			return 1;
		}
		return 0;
	}
};
