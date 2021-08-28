#pragma once


#include "lib/logo.h"
unsigned int White = GetColor(255, 255, 255);
unsigned int Black = GetColor(0, 0, 0);

class Option {
public:
	int strColor = White;
	int musicFlg = 1;
	int soundFlg = 1;
	int likeliFlg = 0;		// �w�K�@�B�̏o�̓t���O 0: ��\��, 1: �F�ŕ\��, 2: ���l���\��
	int commentFlg = 0;
};

class Key {
public:
	int state[256];
	int debugFlg = 0;
	int waitConst = 5;		// �Q�[�����̃L�[���̓E�F�C�g
	int waitCnt = 20;		// �L�[���̓E�F�C�g�̃J�E���^

	void setWait(int wait) {
		waitConst = wait;
	}

	void initWait() {
		waitCnt = waitConst;
	}

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
		if (waitCnt <= 0 && state[KEY_INPUT_RETURN] == 1) {
			initWait();
			return true;
		}
		return false;
	}

	bool onBack() {
		return (state[KEY_INPUT_Z] == 1 || state[KEY_INPUT_BACK] == 1);
	}

	// �Q�[�����̃L�[����
	bool onCheck() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RETURN] == 1 || state[KEY_INPUT_SPACE] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onUp() {
		if (waitCnt <= 0 && (state[KEY_INPUT_UP]== 1 || state[KEY_INPUT_W] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onDown() {
		if (waitCnt <= 0 && (state[KEY_INPUT_DOWN] == 1 || state[KEY_INPUT_S] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onLeft() {
		if (waitCnt <= 0 && (state[KEY_INPUT_LEFT] == 1 || state[KEY_INPUT_A] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onRight() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RIGHT] == 1 || state[KEY_INPUT_D] == 1)) {
			initWait();
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
			if (option.strColor == Black) {
				option.strColor = White;
			}
			else {
				option.strColor = Black;
			}
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

	// �Ԃ�l1�ŃQ�[��������������
	int toggleHighSpeedLearning(int& gameFlg) {
		// ���������w�K���[�h
		if (state[KEY_INPUT_H] == 1) {
			gameFlg = 5;
			return 1;
		}
		return 0;
	}

	// �Ԃ�l1�ŃQ�[��������������
	int toggleAutoLearning(int& gameFlg, int& taijin) {
		// �����w�K���[�h
		if (state[KEY_INPUT_A] == 1) {
			taijin = 2;
			gameFlg = 1;
			return 1;
		}
		return 0;
	}

	void toggleDebug(int& debug) {
		//�f�o�b�O���[�h����
		if (state[KEY_INPUT_G] == 1) {
			debug = (debug + 1) % 2;
			debugFlg = debug;
		}
	}

	void toggleForDebug(Option& option, int& cutinFlg) {
		if (debugFlg) {
			// �J�b�g�C��������
			if (state[KEY_INPUT_C] == 1) {
				cutinFlg = 1;
			}

			// �w�K�@�B�̏o�͂�����
			if (state[KEY_INPUT_V] == 1) {
				option.likeliFlg = (option.likeliFlg + 1) % 3;
			}

			/*
			// �G���f�B���O���[�h
			if (state[KEY_INPUT_MINUS] == 1) {
				if (game.flg != -4) {
					game.flg = -4;
				}
				else {
					StopMusic();
					game.flg = 0;
				}
				end_cnt = 0;
			}
			*/
		}
	}

	void skipBattle(int& gameFlg, int& sceneFlg) {
		if (debugFlg) {
			if (state[KEY_INPUT_B] == 1) {
				StopMusic();
				sceneFlg++;
				gameFlg = -6;
			}
		}
	}
};
