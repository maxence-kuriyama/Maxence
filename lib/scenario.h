#pragma once

#include "lib/message.h"

void init_scene_text(string* scen_txt, int* scen_who);

class Scenario {
private:
	int strColorDebug = GetColor(150, 0, 0);

public:
	int flg = 0;	// �V�i���I�Ǘ��p�t���O
	int imgRoom;
	int imgCard;
	MrK mrK[4];
	MrK deer;
	Message msg;
	int textCnt = 0;	// �e�L�X�g�J�E���^
	int eqX = 0;
	int eqY = 0;		// eq = earthquake
	string text[50];
	int who[50];
	int cnt = 0;	// �t���[���J�E���^

	Scenario() {
		init_scene_text(text, who);
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120, "graph/stripe11.png", 1);
		mrK[1].set(480, 120, "graph/stripe12.png", 1);
		mrK[2].set(160, 240, "graph/stripe13.png", 1);
		mrK[3].set(480, 240, "graph/stripe14.png", 1);
		deer.set(270, 200, "graph/stripe15.png", 0);
		initialize();
		msg.initialize();
	}

	void initialize() {
		flg = 0;
		cnt = 0;
		textCnt = 0;
		msg.set(text[textCnt], who[textCnt]);
		mrK[0].exhibit();
		mrK[1].exhibit();
		mrK[2].exhibit();
		mrK[3].exhibit();
		deer.hide();
	}

	void msgSet() {
		msg.set(text[textCnt], who[textCnt]);
	}

	void msgLoad() {
		textCnt++;
		msgSet();
	}

	void happenEQ() {
		eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
	}

	void stopEQ() {
		eqX = 0;
	}

	// NEXT���o��܂Ń��b�Z�[�W��ǂ�
	void readMsg(Mouse& mouse) {
		if (mouse.click() && msg.skip()) {
			if (text[textCnt + 1] == "NEXT") {
				flg++;
				textCnt++;
			}
			else {
				textCnt = min(36, textCnt + 1);
				msgSet();
			}
		}
	}

	// ���̃��b�Z�[�W��ǂ݂Ȃ����ʓ]��
	void nextMsg(Mouse& mouse) {
		if (mouse.click()) {
			flg++;
			msgLoad();
		}
	}

	// ���b�Z�[�W�𑗂炸�ɏ�ʓ]��
	void waitClick(Mouse& mouse){
		if (mouse.click()) {
			flg++;
		}
	}

	void startMusic(const char* musicName) {
		if (CheckMusic() != 1) {
			PlayMusic(musicName, DX_PLAYTYPE_BACK);
		}
	}

	int show(Mouse& mouse) {
		// �w�i�E�l���̕`��
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		for (int i = 0; i < 4; ++i) {
			mrK[i].draw(eqX);
		}
		deer.draw();

		switch (flg) {
		case 0:
			startMusic("sound/bgm03.mp3");
			readMsg(mouse);
			break;
		case 1:
			// ���������
			deer.exhibit();
			nextMsg(mouse);
			break;
		case 2:
			// �J�[�h��������
			DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
			nextMsg(mouse);
			break;
		case 3:
			// ������
			deer.hide();
			mrK[1].hide();
			readMsg(mouse);
			break;
		case 4:
			// �n�k
			happenEQ();
			nextMsg(mouse);
			break;
		case 5:
			stopEQ();
			readMsg(mouse);
			break;
		case 6:
			// ����
			StopMusic();
			startMusic("sound/bgm04.mp3");
			msgLoad();
			return 1;
		case 7:
			startMusic("sound/bgm03.mp3");
			readMsg(mouse);
			break;
		case 8:
			// �Ԃ�����
			mrK[2].hide();
			waitClick(mouse);
			break;
		case 9:
			// �n�k
			happenEQ();
			nextMsg(mouse);
			break;
		case 10:
			stopEQ();
			readMsg(mouse);
			break;
		case 11:
			// ����
			StopMusic();
			startMusic("sound/bgm05.mp3");
			msgLoad();
			return 1;
		case 12:
			startMusic("sound/bgm03.mp3");
			readMsg(mouse);
			break;
		case 13:
			// �΂�����
			mrK[3].hide();
			waitClick(mouse);
			break;
		case 14:
			// �n�k
			happenEQ();
			waitClick(mouse);
			break;
		case 15:
			// ���o�Ă���
			stopEQ();
			mrK[1].exhibit();
			nextMsg(mouse);
			break;
		case 17:
			// ��O��
			StopMusic();
			startMusic("sound/bgm06.mp3");
			msgLoad();
			return 1;
		case 19:
			// ������
			mrK[1].hide();
			waitClick(mouse);
			break;
		case 20:
			// �n�k
			happenEQ();
			nextMsg(mouse);
			break;
		case 21:
			stopEQ();
			startMusic("sound/bgm07.mp3");
			readMsg(mouse);
			break;
		case 22:
			// ��l��
			StopMusic();
			startMusic("sound/bgm08.mp3");
			msgLoad();
			return 1;
		default:
			readMsg(mouse);
			break;
		}

		msg.draw();
		cnt = (cnt + 1) % 10000;

		return 0;
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
			DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
			DrawFormatString(245, 245, strColor, "textCnt: %d", textCnt);
			DrawFormatString(245, 265, strColor, "eqX: %d", eqX);
			DrawFormatString(245, 285, strColor, "textLen: %d", msg.textLen);
			DrawFormatString(245, 305, strColor, "charCnt: %d", msg.charCnt);
			DrawFormatString(245, 325, strColor, "who: %d", msg.who);
			DrawFormatString(245, 345, strColor, "mrK0.vis: %d", mrK[0].visible);
			DrawFormatString(245, 365, strColor, "mrK1.vis: %d", mrK[1].visible);
			DrawFormatString(245, 385, strColor, "mrK2.vis: %d", mrK[2].visible);
			DrawFormatString(245, 405, strColor, "mrK3.vis: %d", mrK[3].visible);
			DrawFormatString(245, 425, strColor, "deer.vis: %d", deer.visible);
		}
	}
};

void init_scene_text(string* scen_txt, int* scen_who) {

	scen_who[0] = 0;
	scen_txt[0] = "�\�\���E�͂P�̕����ŏo���Ă���B";
	scen_who[1] = 0;
	scen_txt[1] = "NEXT";
	scen_who[2] = 0;
	scen_txt[2] = "�uMr.K�����E��łڂ��v";
	scen_who[3] = 1;
	scen_txt[3] = "Mr.K: �Ȃɂ��I�H";
	scen_who[4] = 4;
	scen_txt[4] = "Mr.K: ����AMr.K�����Ȃ����A���Ȃ񂾁I";
	scen_who[5] = 0;
	scen_txt[5] = "NEXT";
	scen_who[6] = 3;
	scen_txt[6] = "Mr.K: ���E���łт�Ƃ����͖̂{���̎��̂悤���ȁB";
	scen_who[7] = 3;
	scen_txt[7] = "Mr.K: �������A��̉�X�̂����̒N���łڂ��Ƃ����̂��H �M�l���H Mr.K�I�I�I";
	scen_who[8] = 0;
	scen_txt[8] = "NEXT";
	scen_who[9] = 3;
	scen_txt[9] = "Mr.K: �n���ȁA����Ȃ͂��ł͂Ȃ������̂Ɂc�B";
	scen_who[10] = 3;
	scen_txt[10] = "Mr.K: �Ō�̍Ŋ��͌�����Ȃ��ƌ��߂Ă����̂ɁA����Ȏ��ɗl�Ƃ͂ȁB";
	scen_who[11] = 3;
	scen_txt[11] = "Mr.K: �����A�������˂ΐ��E�̕��󂪎~�܂�Ƃ����̂Ȃ�A���̎��ɂ��Ӗ�������������Ƃ������̂���Ȃ����c�c�c�c�B";
	scen_who[12] = 0;
	scen_txt[12] = "NEXT";
	scen_who[13] = 4;
	scen_txt[13] = "Mr.K: �M�l�AMr.K���E�����ȁI";
	scen_who[14] = 4;
	scen_txt[14] = "Mr.K: ���̂��������荇���Ȃ��̂��낤�B�ł����ꂪ�������̓��m���W�܂������̐��Ȃ̂��낤�B";
	scen_who[15] = 4;
	scen_txt[15] = "Mr.K: ������Mr.K��B�E���������B���̒��Ő������Ӗ������o��������Ȃ����B";
	scen_who[16] = 4;
	scen_txt[16] = "Mr.K: ���������Ă���B";
	scen_who[17] = 0;
	scen_txt[17] = "NEXT";
	scen_who[18] = 4;
	scen_txt[18] = "Mr.K: ���͌�������B";
	scen_who[19] = 4;
	scen_txt[19] = "Mr.K: �c�������O�ɂ����������鎞�����邾�낤�c�c�c�c�B��ɐ����Ă����B";
	scen_who[20] = 0;
	scen_txt[20] = "NEXT";
	scen_who[21] = 2;
	scen_txt[21] = "Mr.K: ���Ȃ�1�l�ɂȂ�܂ŉB��Ă��܂����B";
	scen_who[22] = 2;
	scen_txt[22] = "Mr.K: ����2�l�������Ă���Ă��肪�Ƃ��B�����Ȃ邱�Ƃ̓J�[�h�����������番�����Ă�������ˁB";
	scen_who[23] = 2;
	scen_txt[23] = "Mr.K: �����ĘA��̋M�����n������΁A�����͎��̐��E�ɂȂ�󂾁B�������낤�H";
	scen_who[24] = 2;
	scen_txt[24] = "Mr.K: ���͐��E��łڂ����肶��Ȃ��̂ł��B�M�����ז��Ȃ����Ȃ̂ł���B";
	scen_who[25] = 0;
	scen_txt[25] = "NEXT";
	scen_who[26] = 2;
	scen_txt[26] = "Mr.K: �n���ȁA����ȁA���E�̉��ɐ����`�����X�𓦂��Ă��܂��Ȃ�āA�L�蓾�Ȃ��B";
	scen_who[27] = 2;
	scen_txt[27] = "Mr.K: �c�c�c�c�����A�����Ă������Ǝ��̂��傫�ȃ`�����X�������̂��c�c�c�c���܂����ȁB";
	scen_who[28] = 0;
	scen_txt[28] = "NEXT";
	scen_who[29] = 1;
	scen_txt[29] = "Mr.K: Mr.K�͉��ɂȂ낤�Ƃ��Ă����̂��c�c�c�c�B";
	scen_who[30] = 1;
	scen_txt[30] = "Mr.K: ���A�������I";
	scen_who[31] = 1;
	scen_txt[31] = "Mr.K: K�Ƃ� king �̂��Ƃ������̂��B";
	scen_who[32] = 1;
	scen_txt[32] = "Mr.K: Mr.K�B�l����������B";
	scen_who[33] = 1;
	scen_txt[33] = "Mr.K: �����A��͂��̐��E�ɂ���͖̂l�������B";
	scen_who[34] = 1;
	scen_txt[34] = "Mr.K: �����炱�̉��ł���l���E���Đ��E���~���Ă���B";
	scen_who[35] = 0;
	scen_txt[35] = "NEXT";
	scen_who[36] = 1;
	scen_txt[36] = "Mr.K: ���肪�Ƃ��B";
	scen_who[37] = 5;
	scen_txt[37] = "��: �����ƌ��͂������B";
	scen_who[38] = 5;
	scen_txt[38] = "��: ���ƌ��͂������B";
	scen_who[39] = -1;
	scen_txt[39] = "���̃Q�[���̉������E��łڂ��Ƃ����Ȃ�c";
	scen_who[40] = -1;
	scen_txt[40] = "���ł���v���C���[���A���̐��E�ɋ���M�l��łڂ��̂��낤�H";
	scen_who[41] = 5;
	scen_txt[41] = "��: �C�Â��Ă��܂������B";
	scen_who[42] = 5;
	scen_txt[42] = "��: �Ȃ�Ύ��������B�B�A������ł��邱�Ƃ��m�F���邾�����B";
	scen_who[43] = 5;
	scen_txt[43] = "��: �������ė��Ȃ����B";
	scen_who[44] = 5;
	scen_txt[44] = "��: �c�c�c�c";

}
