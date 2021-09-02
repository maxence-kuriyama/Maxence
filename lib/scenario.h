#pragma once

#include "lib/message.h"

void init_scene_text(string* scen_txt, int* scen_who);

class Scenario {
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
	string text[40];
	int who[40];
	int cnt = 0;	// �t���[���J�E���^

	Scenario() {
		init_scene_text(text, who);
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120, "graph/stripe11.png", 1);
		mrK[1].set(480, 120, "graph/stripe12.png", 1);
		mrK[2].set(160, 240, "graph/stripe13.png", 1);
		mrK[3].set(480, 240, "graph/stripe14.png", 1);
		deer.set(0, 0, "graph/stripe15.png", 0);
		initialize();
		msg.initialize();
	}

	void initialize() {
		flg = 0;
		textCnt = 0;
		msg.set(text[textCnt], who[textCnt]);
	}

	int show(Mouse& mouse, int strColor) {
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		for (int i = 0; i < 4; ++i) {
			if (mrK[i].visible) {
				DrawGraph(mrK[i].x + eqX, mrK[i].y, mrK[i].img, TRUE);
			}
		}
		if (flg == 2) DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);

		msg.draw();

		if (CheckMusic() != 1) {
			if (flg == 0 || flg == 7 || flg == 12) {
				PlayMusic("sound/bgm03.mp3", DX_PLAYTYPE_BACK);
			}
			if (flg == 21) {
				PlayMusic("sound/bgm07.mp3", DX_PLAYTYPE_BACK);
			}
		}

		cnt++;

		DrawFormatString(5, 5, strColor, "flg:%d", flg);
		//DrawFormatString(5, 25, StringColor, "char_cnt:%d", char_cnt);

		switch (flg) {
		case 1:
			//���������
			DrawGraph(480, 120, mrK[1].img, TRUE);
			DrawGraph(270, 200, deer.img, TRUE);
			if (mouse.click()) {
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 2:
			//�J�[�h��������A������
			msg.textLen = MultiByteLength(text[textCnt].c_str());
			if (mouse.click() && msg.skip()) {
				mrK[1].hide();
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 4:
			//�n�k
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 6:
			//����
			PlayMusic("sound/bgm04.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		case 8:
			//�Ԃ�����
			mrK[2].hide();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 9:
			//�n�k
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 11:
			//����
			PlayMusic("sound/bgm05.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		case 13:
			//�΂�����
			mrK[3].hide();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 14:
			//�n�k
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 15:
			//���o�Ă���
			mrK[1].exhibit();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 17:
			//��O��
			PlayMusic("sound/bgm06.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		case 19:
			//������
			mrK[1].hide();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 20:
			//�n�k
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 22:
			//��l��
			PlayMusic("sound/bgm08.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		default:
			msg.textLen = MultiByteLength(text[textCnt].c_str());
			if (mouse.click() && msg.skip()) {
				if (textCnt == 0 || textCnt == 3 || textCnt == 5 || textCnt == 8 || textCnt == 12 || textCnt == 14 || textCnt == 18 || textCnt == 20 || textCnt == 26) {
					flg++;
				}
				else {
					textCnt = min(27, textCnt + 1);
					msg.set(text[textCnt], who[textCnt]);
				}
			}
			break;
		}

		return 0;
	}
};

void init_scene_text(string* scen_txt, int* scen_who) {

	scen_who[0] = 0;
	scen_txt[0] = "�\�\���E�͂P�̕����ŏo���Ă���B";
	scen_who[1] = 0;
	scen_txt[1] = "�uMr.K�����E��łڂ��v";
	scen_who[2] = 1;
	scen_txt[2] = "Mr.K: �Ȃɂ��I�H";
	scen_who[3] = 4;
	scen_txt[3] = "Mr.K: ����AMr.K�����Ȃ����A���Ȃ񂾁I";
	scen_who[4] = 3;
	scen_txt[4] = "Mr.K: ���E���łт�Ƃ����͖̂{���̎��̂悤���ȁB";
	scen_who[5] = 3;
	scen_txt[5] = "Mr.K: �������A��̉�X�̂����̒N���łڂ��Ƃ����̂��H �M�l���H Mr.K�I�I�I";
	scen_who[6] = 3;
	scen_txt[6] = "Mr.K: �n���ȁA����Ȃ͂��ł͂Ȃ������̂Ɂc�B";
	scen_who[7] = 3;
	scen_txt[7] = "Mr.K: �Ō�̍Ŋ��͌�����Ȃ��ƌ��߂Ă����̂ɁA����Ȏ��ɗl�Ƃ͂ȁB";
	scen_who[8] = 3;
	scen_txt[8] = "Mr.K: �����A�������˂ΐ��E�̕��󂪎~�܂�Ƃ����̂Ȃ�A���̎��ɂ��Ӗ�������������Ƃ������̂���Ȃ����c�c�c�c�B";
	scen_who[9] = 4;
	scen_txt[9] = "Mr.K: �M�l�AMr.K���E�����ȁI";
	scen_who[10] = 4;
	scen_txt[10] = "Mr.K: ���̂��������荇���Ȃ��̂��낤�B�ł����ꂪ�������̓��m���W�܂������̐��Ȃ̂��낤�B";
	scen_who[11] = 4;
	scen_txt[11] = "Mr.K: ������Mr.K��B�E���������B���̒��Ő������Ӗ������o��������Ȃ����B";
	scen_who[12] = 4;
	scen_txt[12] = "Mr.K: ���������Ă���B";
	scen_who[13] = 4;
	scen_txt[13] = "Mr.K: ���͌�������B";
	scen_who[14] = 4;
	scen_txt[14] = "Mr.K: �c�������O�ɂ����������鎞�����邾�낤�c�c�c�c�B��ɐ����Ă����B";
	scen_who[15] = 2;
	scen_txt[15] = "Mr.K: ���Ȃ�1�l�ɂȂ�܂ŉB��Ă��܂����B";
	scen_who[16] = 2;
	scen_txt[16] = "Mr.K: ����2�l�������Ă���Ă��肪�Ƃ��B�����Ȃ邱�Ƃ̓J�[�h�����������番�����Ă�������ˁB";
	scen_who[17] = 2;
	scen_txt[17] = "Mr.K: �����ĘA��̋M�����n������΁A�����͎��̐��E�ɂȂ�󂾁B�������낤�H";
	scen_who[18] = 2;
	scen_txt[18] = "Mr.K: ���͐��E��łڂ����肶��Ȃ��̂ł��B�M�����ז��Ȃ����Ȃ̂ł���B";
	scen_who[19] = 2;
	scen_txt[19] = "Mr.K: �n���ȁA����ȁA���E�̉��ɐ����`�����X�𓦂��Ă��܂��Ȃ�āA�L�蓾�Ȃ��B";
	scen_who[20] = 2;
	scen_txt[20] = "Mr.K: �c�c�c�c�����A�����Ă������Ǝ��̂��傫�ȃ`�����X�������̂��c�c�c�c���܂����ȁB";
	scen_who[21] = 1;
	scen_txt[21] = "Mr.K: Mr.K�͉��ɂȂ낤�Ƃ��Ă����̂��c�c�c�c�B";
	scen_who[22] = 1;
	scen_txt[22] = "Mr.K: ���A�������I";
	scen_who[23] = 1;
	scen_txt[23] = "Mr.K: K�Ƃ� king �̂��Ƃ������̂��B";
	scen_who[24] = 1;
	scen_txt[24] = "Mr.K: Mr.K�B�l����������B";
	scen_who[25] = 1;
	scen_txt[25] = "Mr.K: �����A��͂��̐��E�ɂ���͖̂l�������B";
	scen_who[26] = 1;
	scen_txt[26] = "Mr.K: �����炱�̉��ł���l���E���Đ��E���~���Ă���B";
	scen_who[27] = 1;
	scen_txt[27] = "Mr.K: ���肪�Ƃ��B";
	scen_who[28] = 5;
	scen_txt[28] = "��: �����ƌ��͂������B";
	scen_who[29] = 5;
	scen_txt[29] = "��: ���ƌ��͂������B";
	scen_who[30] = -1;
	scen_txt[30] = "���̃Q�[���̉������E��łڂ��Ƃ����Ȃ�c";
	scen_who[31] = -1;
	scen_txt[31] = "���ł���v���C���[���A���̐��E�ɋ���M�l��łڂ��̂��낤�H";
	scen_who[32] = 5;
	scen_txt[32] = "��: �C�Â��Ă��܂������B";
	scen_who[33] = 5;
	scen_txt[33] = "��: �Ȃ�Ύ��������B�B�A������ł��邱�Ƃ��m�F���邾�����B";
	scen_who[34] = 5;
	scen_txt[34] = "��: �������ė��Ȃ����B";
	scen_who[35] = 5;
	scen_txt[35] = "��: �c�c�c�c";

}
