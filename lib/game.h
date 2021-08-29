#pragma once

#include <time.h>
#include "lib/field.h"
#include "lib/hist.h"
#include "lib/basic.h"
#include "lib/anime.h"
#include "lib/keyboard.h"
#include "lib/comment.h"


//int trainCnt = 0;
//int comHistt[100];
void initializeTrain() {
	//trainCnt = 0;
	//for (int i = 0; i < 100; i++) {
		//comHistt[i] = 0;
	//}
}

class Game {
public:
	int flg = -3;	// -3,..,-1: Demo
					// 0: Menu, 1: Game, 2: Result
					// -4: Ending
					// -6: Story
					// 5: High-speed Learning
	int taijin = 0;		// 0: vsHuman, 1: vsCOM, 2: AutoLearning
	int teban = 0;		// 0: senko, 1: koko
	int cnt = 0;		// �^�[����
	int keyboardFlg = 0;	// 0: �}�E�X����, 1: �L�[�{�[�h����
	int debugFlg = 0;

	int nextField = -1;		// -1: anywhere
	int drawCnt = 0;		// �����������̋����I���̂��߂̃J�E���g
	string mode = "";

	// ���������֘A
	long start = clock();	// ���������J�n����
	long fpsStart = clock();	// fps�v���J�n����
	int fps = 0;			// fps�o�͗p
	int fpsCnt = 0;			// fps�v���p

	Option option;
	Field mother;
	Field child[3][3];
	History hist;
	Camera camera;
	Mouse mouse;
	Key key;
	Logo logo;
	Anime cutin;
	Comment comment;

	void initialize(int f = 1) {
		flg = f;
		cnt = 0;
		drawCnt = 0;
		nextField = -1;
		camera.initialize();
		mother.initialize();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				child[i][j].initialize();
			}
		}
		hist.initialize();
		mouse.set();
		key.initWait();
	}

	double update(int global_x, int global_y, int local_x, int local_y, int side = 0) {
		if (side == 0) {
			side = 1 - 2 * (cnt % 2);
		}
		//�Ֆʂ̍X�V
		if (nextField == 3 * global_x + global_y || nextField == -1) {
			if (child[global_x][global_y].update(local_x, local_y, side) == 0) {
				cnt++;
				//�������c��
				hist.add(global_x, global_y, local_x, local_y, nextField);
				//�S�̂̍X�V
				mother.update(global_x, global_y, child[global_x][global_y].victory());
				if (child[local_x][local_y].victory() != 0) {
					nextField = -1;
					return RWD_DOM;
				}
				else {
					nextField = local_x * 3 + local_y;
					return RWD_PUT;
				}
			}
		}
		return -100.0;
	}

	void sync() {
		// ����fps�v��
		fpsCnt++;
		if (clock() - fpsStart > 1000.0) {
			fps = fpsCnt;
			fpsCnt = 0;
			fpsStart = clock();
		}
		// ��������
		while (clock() - start < 1000.0 / FPS && flg != 5) {
			WaitTimer(1);
		}
		start = clock();
	}

	//�i���ɏ��s�����Ȃ��ꍇ�̏���
	void stopDrawGame() {
		if (drawCnt > 10000) {
			flg = 2;
		}
		if (taijin == 2 || flg == 5) {
			drawCnt++;
		}
	}


	/*===========================*/
	//    bool
	/*===========================*/
	bool isPlayTurn() {
		// �ΐl��A���邢�͐lvsCOM�̐l�̎��
		return (vsHuman() || (vsCOM() && cnt % 2 == teban));
	}

	bool vsHuman() {
		return taijin == 0;
	}

	bool vsCOM() {
		return taijin == 1;
	}


	/*===========================*/
	//    �L�[�{�[�h���͊֘A
	/*===========================*/
	void toggleByKey() {
		key.toggleSetting(option, logo);
		//key.configLearning();
		key.toggleDebug(debugFlg);
		if (debugFlg) {
			key.toggleForDebug(option, cutin.flg);
		}
	}

	void toggleHighSpeedLearning() {
		if (key.toggleHighSpeedLearning(flg) == 1) {
			mode = "�I�[�g";
			initialize(flg);
			initializeTrain();
		}
	}

	void toggleAutoLearning() {
		if (key.toggleAutoLearning(flg, taijin) == 1 || key.toggleHighSpeedLearning(flg) == 1) {
			mode = "�I�[�g";
			initialize(flg);
			initializeTrain();
		}
	}

	void skipBattle(int& sceneFlg) {
		if (debugFlg) {
			key.skipBattle(flg, sceneFlg);
		}
	}


	/*===========================*/
	//    �f�o�b�O���
	/*===========================*/
	void debugDump() {
		if (debugFlg) {
			int strColor = option.strColor;
			DrawFormatString(5, 5, strColor, "fps:%d", fps);
			DrawFormatString(5, 25, strColor, "keyboardFlg:%d", keyboardFlg);
		}
	}
};
