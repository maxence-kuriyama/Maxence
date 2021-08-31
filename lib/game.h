#pragma once

#include <time.h>
#include "lib/const.h"
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
	int nextField = -1;		// ���̔ՖʁA-1: anywhere
	int keyboardFlg = 0;	// 0: �}�E�X����, 1: �L�[�{�[�h����
	int debugFlg = 0;

	// ���������֘A
	long start = clock();	// ���������J�n����
	long fpsStart = clock();	// fps�v���J�n����
	int fps = 0;			// fps�o�͗p
	int fpsCnt = 0;			// fps�v���p

	// �Ֆʏ�̑���֘A
	int globalX = 1;
	int globalY = 1;
	int localX = 1;
	int localY = 1;			//�L�[�{�[�h���쎞�̍��W

	// �萔
	int bkColorBase = GetColor(255, 255, 245);
	int frColorBase = GetColor(0, 0, 0);
	int bkColorLastHist = GetColor(255, 160, 160);
	int bkColorStateWin = GetColor(130, 70, 70);
	int bkColorStateLose = GetColor(70, 70, 130);
	int bkColorStateDraw = GetColor(70, 130, 70);
	int frColorNextField = GetColor(255, 0, 0);
	int frColorCurrentCoord = GetColor(0, 0, 0);
	int strColorDebug = GetColor(0, 0, 255);

	int drawCnt = 0;		// �����������̋����I���̂��߂̃J�E���g
	string mode = "";

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
	Button lonely;
	Button vsHuman;
	Menu menu;

	Game() {
		lonely.initialize(TEXT1_X, TEXT1_Y, "�ڂ�����");
		vsHuman.initialize(TEXT2_X, TEXT2_Y, "�ׂ̐l��");
		initialize(-3);
	}

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
		menu.set(lonely, vsHuman);
	}

	// ��������
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

	// �i���ɏ��s�����Ȃ��ꍇ�̏���
	void stopDrawGame() {
		if (drawCnt > 10000) {
			flg = 2;
		}
		if (taijin == 2 || flg == 5) {
			drawCnt++;
		}
	}


	/*===========================*/
	//    Bool
	/*===========================*/
	bool isPlayTurn() {
		// �ΐl��A���邢�͐lvsCOM�̐l�̎��
		return (isVsHuman() || (isVsCOM() && cnt % 2 == teban));
	}

	bool isVsHuman() {
		return taijin == 0;
	}

	bool isVsCOM() {
		return taijin == 1;
	}


	/*===========================*/
	//    ���Z�b�g�{�^��
	/*===========================*/
	void drawLogo() {
		if (flg > 0) {
			if (mouse.onButton(logo.titleX, logo.titleY - 5, logo.titleX + 185, logo.titleY + 65)) {
				DrawBox(logo.titleX, logo.titleY - 5, logo.titleX + 185, logo.titleY + 65, GetColor(20, 150, 150), TRUE);
			}
			logo.draw();
			if (flg == 1) {
				logo.update();
			}
		}
	}

	int reset() {
		if (flg > 0) {
			if (mouse.onButton(logo.titleX, logo.titleY - 5, logo.titleX + 185, logo.titleY + 65)) {
				if (mouse.button[0] == 1) {
					mouse.set();
					flg = 0;
					taijin = 0;
					StopMusic();
					return 1;
				}
			}
		}
		return 0;
	}


	/*===========================*/
	//    ���j���[���
	/*===========================*/
	int menuChoose() {
		return menu.choose(keyboardFlg, mouse, key, option.strColor);
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
		if (debugFlg && isVsCOM()) {
			key.skipBattle(flg, sceneFlg);
		}
	}


	/*===========================*/
	//    �Ֆʕ\���֘A
	/*===========================*/
	void drawBase() {
		DrawBox(160, 80, 460, 380, bkColorBase, TRUE);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), frColorBase, FALSE);
			}
		}
	}

	void drawHistLast() {
		if (hist.canCancel()) {
			int upLeftX = 160 + 100 * hist.last[0] + 33 * hist.last[2] + 1;
			int upLeftY = 80 + 100 * hist.last[1] + 33 * hist.last[3] + 1;
			int lowRightX = 160 + 100 * hist.last[0] + 33 * (hist.last[2] + 1);
			int lowRightY = 80 + 100 * hist.last[1] + 33 * (hist.last[3] + 1);
			DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorLastHist, TRUE);
		}
	}

	void drawGlobalState() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				int upLeftX = 160 + 100 * i + 1;
				int upLeftY = 80 + 100 * j + 1;
				int lowRightX = 160 + 100 * (i + 1) - 1;
				int lowRightY = 80 + 100 * (j + 1) - 1;
				if (mother.state[i][j] == 1) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateWin, TRUE);
				}
				else if (mother.state[i][j] == -1) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateLose, TRUE);
				}
				else if (mother.state[i][j] != 0) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateDraw, TRUE);
				}
			}
		}
	}

	void drawNextField() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (nextField == 3 * i + j) {
					for (int w = -1; w <= 1; ++w) {
						int upLeftX = 160 + 100 * i + w;
						int upLeftY = 80 + 100 * j + w;
						int lowRightX = 160 + 100 * (i + 1) - w;
						int lowRightY = 80 + 100 * (j + 1) - w;
						DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorNextField, FALSE);
					}
				}
				else if (nextField == -1 && child[i][j].victory() == 0) {
					int upLeftX = 160 + 100 * i;
					int upLeftY = 80 + 100 * j;
					int lowRightX = 160 + 100 * (i + 1);
					int lowRightY = 80 + 100 * (j + 1);
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorNextField, FALSE);
				}
			}
		}
	}

	void drawLocalState() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				child[i][j].draw(176.5 + 100 * i, 96.5 + 100 * j, 33);
			}
		}
	}

	void drawCurrentCoord() {
		int upLeftX = 160 + 100 * globalX + 33 * localX;
		int upLeftY = 80 + 100 * globalY + 33 * localY;
		int lowRightX = 160 + 100 * globalX + 33 * (localX + 1);
		int lowRightY = 80 + 100 * globalY + 33 * (localY + 1);
		DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorCurrentCoord, FALSE);
	}

	void drawBattleMessage() {
		DrawFormatString(470, 80, option.strColor, "�E�N���b�N:");
		DrawFormatString(540, 100, option.strColor, "�΂�u��");
		DrawFormatString(470, 124, option.strColor, "z�L�[�iBkSp�L�[�j:");
		DrawFormatString(540, 144, option.strColor, "���߂�");
	}

	void drawComment() {
		// �R�����g�`��
		if (option.commentFlg) {
			comment.draw(option.strColor);
		}
		// �R�����g�����ւ�
		comment.update();
	}


	/*===========================*/
	//    �ՖʍX�V�֘A
	/*===========================*/
	void getMouseCoord() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						int upLeftX = 160 + 100 * i + 33 * k;
						int upLeftY = 80 + 100 * j + 33 * l;
						int lowRightX = 160 + 100 * i + 33 * (k + 1);
						int lowRightY = 80 + 100 * j + 33 * (l + 1);
						if (mouse.onButton(upLeftX, upLeftY, lowRightX, lowRightY)) {
							globalX = i;
							globalY = j;
							localX = k;
							localY = l;
						}
					}
				}
			}
		}
	}

	void moveCoordByKey() {
		if (key.onUp()) {
			localY--;
			if (localY < 0) {
				if (globalY > 0) {
					globalY--;
					localY += 3;
				}
				else {
					localY = 0;
				}
			}
		}
		if (key.onDown()) {
			localY++;
			if (localY > 2) {
				if (globalY < 2) {
					globalY++;
					localY -= 3;
				}
				else {
					localY = 2;
				}
			}
		}
		if (key.onLeft()) {
			localX--;
			if (localX < 0) {
				if (globalX > 0) {
					globalX--;
					localX += 3;
				}
				else {
					localX = 0;
				}
			}
		}
		if (key.onRight()) {
			localX++;
			if (localX > 2) {
				if (globalX < 2) {
					globalX++;
					localX -= 3;
				}
				else {
					localX = 2;
				}
			}
		}
	}

	bool playTurn() {
		moveCoordByKey();
		if (keyboardFlg) {
			if (key.onCheck()) {
				return true;
			}
		}
		else {
			getMouseCoord();
			if (mouse.clickRight()) {
				return true;
			}
		}
		return false;
	}

	double update(int side = 0) {
		return update(globalX, globalY, localX, localY, side);
	}

	double update(int global_x, int global_y, int local_x, int local_y, int side = 0) {
		// side���w�肳��Ă��Ȃ���΁Acnt����v�Z����
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

	bool goBackHist() {
		if (hist.canCancel() && isVsHuman()) {
			child[hist.last[0]][hist.last[1]].state[hist.last[2]][hist.last[3]] = 0;
			mother.state[hist.last[0]][hist.last[1]] = 0;
			mother.update(hist.last[0], hist.last[1], child[hist.last[0]][hist.last[1]].victory());
			nextField = hist.last[4];
			hist.goBack();
			cnt--;
			return true;
		}
		return false;
	}


	/*===========================*/
	//    �f�o�b�O���
	/*===========================*/
	void debugDump() {
		if (debugFlg) {
			int strColor = strColorDebug;
			// Game
			DrawFormatString(5, 25, strColor, "fps: %d", fps);
			DrawFormatString(5, 45, strColor, "gameFlg: %d", flg);
			DrawFormatString(5, 65, strColor, "taijin: %d", taijin);
			DrawFormatString(5, 85, strColor, "teban: %d", teban);
			DrawFormatString(5, 105, strColor, "cnt: %d", cnt);
			DrawFormatString(5, 125, strColor, "keyboardFlg: %d", keyboardFlg);
			// Option
			DrawFormatString(125, 25, strColor, "musicFlg: %d", option.musicFlg);
			DrawFormatString(125, 45, strColor, "soundFlg: %d", option.soundFlg);
			DrawFormatString(125, 65, strColor, "likeliFlg: %d", option.likeliFlg);
			DrawFormatString(125, 85, strColor, "commentFlg: %d", option.commentFlg);
			// Hist
			DrawFormatString(125, 145, strColor, "cancelCnt: %d", hist.cancelCnt);
			DrawFormatString(125, 165, strColor, "last[0]: %d", hist.last[0]);
			DrawFormatString(125, 185, strColor, "last[1]: %d", hist.last[1]);
			DrawFormatString(125, 205, strColor, "last[2]: %d", hist.last[2]);
			DrawFormatString(125, 225, strColor, "last[3]: %d", hist.last[3]);
			DrawFormatString(125, 245, strColor, "last[4]: %d", hist.last[4]);
			// Comment
			DrawFormatString(245, 25, strColor, "maxSize: %d", comment.texts.maxSize);
			DrawFormatString(245, 45, strColor, "size: %d", comment.texts.size);
			DrawFormatString(245, 65, strColor, "comX: %d", comment.x);
			DrawFormatString(245, 85, strColor, "comY: %d", comment.y);
			DrawFormatString(245, 105, strColor, "textId: %d", comment.textId);
			DrawFormatString(245, 125, strColor, "textSeq: %d", comment.textSeq);
			DrawFormatString(245, 145, strColor, "comCnt: %d", comment.cnt);
			// Menu
			DrawFormatString(365, 25, strColor, "menu.size: %d", menu.size);
			DrawFormatString(365, 45, strColor, "menu.id: %d", menu.id);
			DrawFormatString(365, 65, strColor, "btn1.upLX: %d", menu.button[0].upLeftX);
			DrawFormatString(365, 85, strColor, "btn1.upLY: %d", menu.button[0].upLeftY);
			DrawFormatString(365, 105, strColor, "btn1.lowRX: %d", menu.button[0].lowRightX);
			DrawFormatString(365, 125, strColor, "btn1.lowRY: %d", menu.button[0].lowRightY);
			DrawFormatString(365, 145, strColor, "btn2.upLX: %d", menu.button[1].upLeftX);
			DrawFormatString(365, 165, strColor, "btn2.upLY: %d", menu.button[1].upLeftY);
			DrawFormatString(365, 185, strColor, "btn2.lowRX: %d", menu.button[1].lowRightX);
			DrawFormatString(365, 205, strColor, "btn2.lowRY: %d", menu.button[1].lowRightY);
		}
	}
};
