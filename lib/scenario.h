#pragma once

#include "lib/basic.h"

void init_scene_text(string* scen_txt, int* scen_who);
void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
int MultiByteLength(const char* String);

class Scenario {
public:
	int flg = 0;	// �V�i���I�Ǘ��p�t���O
	int imgRoom;
	int imgCard;
	int imgStripe[15];
	int Font0 = CreateFontToHandle("HG���ȏ���", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	int charCnt = 0;	// �����`��J�E���^ (<= textLen)
	int textCnt = 0;	// �e�L�X�g�J�E���^
	int textLen = 0;	// �e�L�X�g��
	int eqx = 0;
	int eqy = 0;
	int visible[5] = { 1, 1, 1, 1, 0 };
	string text[40];
	int who[40];
	int cnt = 0;	// �t���[���J�E���^

	Scenario() {
		init_scene_text(text, who);
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		for (int i = 1; i <= 15; ++i) {
			string pict_name;
			pict_name = "graph/stripe" + to_string(i) + ".png";
			imgStripe[i - 1] = LoadGraph(pict_name.c_str());
		}
		initialize();
	}

	void initialize() {
		flg = 0;
	}

	int display(Mouse& mouse, int strColor) {
		DrawExtendGraph(0 + eqx, -50, 640 + eqx, 380, imgRoom, FALSE);
		if (visible[0]) DrawGraph(160 + eqx, 120, imgStripe[10], TRUE);
		if (visible[1]) DrawGraph(480 + eqx, 120, imgStripe[11], TRUE);
		if (visible[2]) DrawGraph(160 + eqx, 240, imgStripe[12], TRUE);
		if (visible[3]) DrawGraph(480 + eqx, 240, imgStripe[13], TRUE);
		if (flg == 2) DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
		DrawRoundBox(15, 380, 10, 609, 89, GetColor(250, 250, 150));
		DrawMessage(charCnt, 110, 390, 600, GetFontSize(), text[textCnt].c_str(), strColor, Font0, GetColor(250, 250, 150));

		switch (who[textCnt]) {
		case 1:
			DrawGraph(30, 380, imgStripe[10], TRUE);
			break;
		case 2:
			DrawGraph(30, 380, imgStripe[11], TRUE);
			break;
		case 3:
			DrawGraph(30, 380, imgStripe[12], TRUE);
			break;
		case 4:
			DrawGraph(30, 380, imgStripe[13], TRUE);
			break;
		case 5:
			DrawGraph(30, 380, imgStripe[14], TRUE);
			break;
		}

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
			DrawGraph(480, 120, imgStripe[11], TRUE);
			DrawGraph(270, 200, imgStripe[14], TRUE);
			if (mouse.click()) {
				flg++;
				textCnt++;
				charCnt = 0;
			}
			break;
		case 2:
			//�J�[�h��������A������
			textLen = MultiByteLength(text[textCnt].c_str());
			if (cnt % 2 == 0 && charCnt < textLen) {
				charCnt++;
			}
			if (mouse.click()) {
				if (charCnt < textLen) {
					charCnt = textLen;
				}
				else {
					visible[1] = 0;
					flg++;
					textCnt++;
					charCnt = 0;
				}
			}
			break;
		case 4:
			//�n�k
			eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqx = 0;
				flg++;
				textCnt++;
				charCnt = 0;
			}
			break;
		case 6:
			//����
			PlayMusic("sound/bgm04.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			charCnt = 0;
			return 1;
		case 8:
			//�Ԃ�����
			visible[2] = 0;
			if (mouse.click()) {
				flg++;
			}
			break;
		case 9:
			//�n�k
			eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqx = 0;
				flg++;
				textCnt++;
				charCnt = 0;
			}
			break;
		case 11:
			//����
			PlayMusic("sound/bgm05.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			charCnt = 0;
			return 1;
		case 13:
			//�΂�����
			visible[3] = 0;
			if (mouse.click()) {
				flg++;
			}
			break;
		case 14:
			//�n�k
			eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqx = 0;
				flg++; textCnt++; charCnt = 0;
			}
			break;
		case 15:
			//���o�Ă���
			visible[1] = 1;
			if (mouse.click()) {
				flg++; ;
			}
			break;
		case 17:
			//��O��
			PlayMusic("sound/bgm06.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			charCnt = 0;
			return 1;
		case 19:
			//������
			visible[1] = 0;
			if (mouse.click()) {
				flg++;
			}
			break;
		case 20:
			//�n�k
			eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqx = 0;
				flg++;
				textCnt++;
				charCnt = 0;
			}
			break;
		case 22:
			//��l��
			PlayMusic("sound/bgm08.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			charCnt = 0;
			return 1;
		default:
			textLen = MultiByteLength(text[textCnt].c_str());
			if (cnt % 2 == 0 && charCnt < textLen) {
				charCnt++;
			}
			if (mouse.click()) {
				if (charCnt < textLen) {
					charCnt = textLen;
				}
				else {
					if (textCnt == 0 || textCnt == 3 || textCnt == 5 || textCnt == 8 || textCnt == 12 || textCnt == 14 || textCnt == 18 || textCnt == 20 || textCnt == 26) {
						flg++;
					}
					else {
						textCnt = min(27, textCnt + 1);
						charCnt = 0;
					}
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

void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor) {
	char TempStr[3];
	int StrLen;
	int i;
	int DrawX;
	int DrawY;
	int CharLen;
	int DrawWidth;
	int BoxWidth;
	int BoxHeight;
	double rate = 1.2;	//�f�t�H���g�̃t�H���g�T�C�Y20�ɑ΂��āA���̓t�H���g�T�C�Y24�ɂ��邽��1.2�{
	int Counter = cnt;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		// �w�肳�ꂽ�������ŕ`�����߂�
		if (Counter <= 0) break;

		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
			Counter--;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
			Counter--;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (DrawX + DrawWidth > RightX)
		{
			DrawX = x;
			DrawY += AddY * rate;
		}
		// �P�����`��
		DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
		// �`����W�����炷
		DrawX += DrawWidth;
		// �`�悷�镶����i�߂�
		i += CharLen;
	}
}

int MultiByteLength(const char* String) {
	int StrLen;
	int i;
	int CharLen;
	int Counter = 0;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			CharLen = 2;
			Counter++;
		}
		else {
			CharLen = 1;
			Counter++;
		}
		i += CharLen;
	}
	return Counter;
}