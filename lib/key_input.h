#pragma once

class Key {
public:
	int state[256];

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
		return 0;
	}

	bool onReturn() {
		return state[KEY_INPUT_RETURN] == 1;
	}

	bool onBack() {
		return (state[KEY_INPUT_Z] == 1 || state[KEY_INPUT_BACK] == 1);
	}

	void toggleSetting(Logo logo, int musicFlg, int soundFlg) {

		//���S�𓮂���
		if (state[KEY_INPUT_AT] == 1) {
			if (logo.acRate >= 0.5) {
				logo.initialize();
			}
			else {
				logo.acRate += 1.0;
			}
		}

		//�����F�̕ύX
		/*
		if (state[KEY_INPUT_I] == 1) {
			if (StringColor == Black) {
				StringColor = White;
			}
			else {
				StringColor = Black;
			}
		}
		*/

		//���y, SE�̗L��
		if (state[KEY_INPUT_P] == 1) {
			musicFlg = (musicFlg + 1) % 2;
			soundFlg = (soundFlg + 1) % 2;
		}

	}

	void configLearn() {

		//�f�[�^�̕ۑ�
		if (state[KEY_INPUT_O] == 1) {
			//save parameters
		}

		//AI�̃E�F�C�g��ύX����
		if (state[KEY_INPUT_M] == 1) {
			waitOnCOM++;
		}
		else if (state[KEY_INPUT_N] == 1) {
			waitOnCOM--;
		}

	}

	void toggleDebug(int &debug) {

		//�f�o�b�O���[�h����
		if (state[KEY_INPUT_D] == 1) {
			debug = (debug + 1) % 2;
		}

	}

	void toggleForDebug(int debug) {
		if (debug) {
			//�J�b�g�C��������
			if (state[KEY_INPUT_C] == 1) {
				cutinFlg = 1;
			}

			//COM�̏o�͂�����
			if (state[KEY_INPUT_V] == 1) {
				likeliFlg = (likeliFlg + 1) % 3;
			}

			//�R�����g������
			if (state[KEY_INPUT_K] == 1) {
				commentFlg = (commentFlg + 1) % 2;
			}

			//�G���f�B���O���[�h
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
		}
	}
};
